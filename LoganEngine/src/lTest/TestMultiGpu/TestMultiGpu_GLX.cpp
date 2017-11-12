
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

/*
 * TODO:
 * https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
 * https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glXIntro.xml
 * https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glXMakeCurrent.xml
 * https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glXCreatePixmap.xml
 */

Window CreateWindow(Display *display)
{
	unsigned int X=0;
    unsigned int Y=0;
    unsigned int Width  = 800;
    unsigned int Height = 600;
	//
	unsigned int BorderColor	= BlackPixel(display,DefaultScreen(display));
    unsigned int BgColor		= BlackPixel(display,DefaultScreen(display));
	//
	Window window;
	window = XCreateSimpleWindow(display,DefaultRootWindow(display),X,Y,Width,Height,0,BorderColor,BgColor);
	//
	XSelectInput(display,window,StructureNotifyMask | KeyPressMask | KeyReleaseMask);
    XMapWindow(display,window);
	//
	return window;
}

#include <GL/glx.h>

typedef const char * (*PfnQueryRendererStringMESA) (Display *dpy, int screen,
                                           int renderer, int attribute);

PfnQueryRendererStringMESA glXQueryRendererStringMESA;

void InitGlxExt()
{
	glXQueryRendererStringMESA = (PfnQueryRendererStringMESA)glXGetProcAddress((const unsigned char *)"glXQueryRendererStringMESA");
}

#include <iostream>

class GlxFbConfigArray
{
private:
	bool Stale = false;
	int NumFbConfigs;
	GLXFBConfig *FbConfigs;
	
public:
	
	bool IsStale()
	{
		return Stale;
	}
	
	unsigned int GetSize()
	{
		return NumFbConfigs;
	}
	
	GLXFBConfig &GetFbConfig(unsigned int id)
	{
		return FbConfigs[id];
	}
	
	GlxFbConfigArray(GlxFbConfigArray &&array)
	{
		NumFbConfigs = array.NumFbConfigs;
		FbConfigs = array.FbConfigs;
		array.Stale = true;
	}
	
	GlxFbConfigArray(int num_fb_configs,GLXFBConfig *fb_configs)
		:NumFbConfigs(num_fb_configs),FbConfigs(fb_configs)
	{
		if(fb_configs == nullptr)
		{
			Stale = true;
		}
	}
	
	~GlxFbConfigArray()
	{
		if(!Stale)
		{
			XFree(FbConfigs);
		}
	}
};

class XDisplay
{
private:
	Display *display;
	
public:
	
	Display *GetHandle()
	{
		return display;
	}
	
	void PrintRenderer()
	{
		const char *RendererString = glXQueryRendererStringMESA(display,0,0,GLX_RENDERER_DEVICE_ID_MESA);
		if(RendererString != nullptr)
		{
			std::cout << "Display " << display << " " << RendererString << std::endl;
		}
	}
	
	GlxFbConfigArray GetFbConfigs(int framebuffer_config[])
	{
		int NumFbConfigs;
		GLXFBConfig *FbConfigs = glXChooseFBConfig(display,
									  DefaultScreen(display),
									  framebuffer_config,
									  &NumFbConfigs
									 );
		return GlxFbConfigArray(NumFbConfigs,FbConfigs);
	}
	
	XDisplay()
	{
		display = XOpenDisplay(nullptr);
	}
	
	~XDisplay()
	{
		XCloseDisplay(display);
	}
};

class GlxContext
{
	XDisplay Display;
	GLXContext Context;
	
public:
	
	void PrintRenderer()
	{
		Display.PrintRenderer();
	}
	
	GlxContext(int framebuffer_config[])
	{
		GlxFbConfigArray FbConfigs = Display.GetFbConfigs(framebuffer_config);
		
		if(!FbConfigs.IsStale())
		{
			Context = glXCreateNewContext(Display.GetHandle(),
										  FbConfigs.GetFbConfig(0),
										  GLX_RGBA_TYPE,
										  nullptr,
										  True
										 );
		}
	}
	
	~GlxContext()
	{
		glXDestroyContext(Display.GetHandle(),Context);
	}
};

int WindowFramebufferConfig[] =
{
	GLX_X_RENDERABLE    , True,
	GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
	GLX_RENDER_TYPE     , GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
	GLX_RED_SIZE        , 8,
	GLX_GREEN_SIZE      , 8,
	GLX_BLUE_SIZE       , 8,
	GLX_ALPHA_SIZE      , 8,
	GLX_DEPTH_SIZE      , 24,
	GLX_STENCIL_SIZE    , 8,
	GLX_DOUBLEBUFFER    , True,
	//GLX_SAMPLE_BUFFERS  , 1,
	//GLX_SAMPLES         , 4,
	None
};

#include <stdlib.h>

int main(int argc,char *argv[])
{
	InitGlxExt();
	
	setenv("DRI_PRIME","0",1);
	GlxContext Context0(WindowFramebufferConfig);
	
	Context0.PrintRenderer();
	
	setenv("DRI_PRIME","1",1);
	GlxContext Context1(WindowFramebufferConfig);
	
	Context1.PrintRenderer();
	
	return 0;
}
