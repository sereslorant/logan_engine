
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <iostream>

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


#include <EGL/egl.h>
#include <EGL/eglext.h>

class EGL_Display
{
private:
	EGLDisplay Display;
	EGLContext Context;
	bool HasSurface = false;
	EGLSurface Surface;
	//
public:
	//
	void *GetProcAddress(const char *proc_name)
	{
		return (void *) eglGetProcAddress(proc_name);
	}
	//
	void Prepare()
	{
		if(HasSurface)
		{
			eglMakeCurrent(Display,Surface,Surface,Context);
		}
		else
		{
			eglMakeCurrent(Display,EGL_NO_SURFACE,EGL_NO_SURFACE,Context);
		}
	}
	//
	void SwapBuffers()
	{
        eglSwapBuffers(Display, Surface);
	}
	//
	template<class Display_T,class Window_T>
	EGL_Display(Display_T native_display,Window_T native_window)
	{
		
		
		const char *ClientExtensions = eglQueryString(EGL_NO_DISPLAY,EGL_EXTENSIONS);
		//
		std::cout << "Supported client extensions: " << std::endl;
		std::cout << ClientExtensions << std::endl;
		//
		Display = eglGetDisplay(native_display);
		//
		EGLint MajorVersion = -1;
		EGLint MinorVersion = -1;
		EGLBoolean Ret;
		//
		Ret = eglInitialize(Display,&MajorVersion,&MinorVersion);
		//
		if(Ret == EGL_FALSE)
		{
			std::cout << "Failed to initialize" << std::endl;
		}
		//
		const char *DisplayExtensions = eglQueryString(Display,EGL_EXTENSIONS);
		//
		std::cout << "Supported display extensions: " << std::endl;
		std::cout << DisplayExtensions << std::endl;
		//
		std::cout << "Egl version: " << MajorVersion << "." << MinorVersion << std::endl;
		//
		Ret = eglBindAPI(EGL_OPENGL_API);
		if(Ret == EGL_FALSE)
		{
			std::cout << "Failed to use OpenGL" << std::endl;
		}
		//
		const EGLint ConfigAttributeList[] = {
				EGL_RED_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE, 8,
				EGL_ALPHA_SIZE, 8,
				EGL_BUFFER_SIZE, 32,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
				EGL_NONE
		};
		//
		EGLint NumConfig;
		EGLConfig Config;
		Ret = eglChooseConfig(Display,ConfigAttributeList,&Config,1,&NumConfig);
		//
		std::cout << "Egl config count: " << NumConfig << std::endl;
		//
		const EGLint ContextAttributeList[] = {
				EGL_CONTEXT_MAJOR_VERSION/*_KHR*/, 3,
				EGL_CONTEXT_MINOR_VERSION/*_KHR*/, 3,
				EGL_CONTEXT_OPENGL_PROFILE_MASK/*_KHR*/, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT/*_KHR*/,
				EGL_NONE
		};
		//
		Context = eglCreateContext(Display,Config,EGL_NO_CONTEXT,ContextAttributeList);
		//
		if(Context == EGL_NO_CONTEXT)
		{
			std::cout << "Error: couldn't create EGL context" << std::endl;
		}
		//
		if(native_window != 0)
		{
			HasSurface = true;
			Surface = eglCreateWindowSurface(Display,Config,native_window,nullptr);
		}
	}
	//
	~EGL_Display()
	{
		eglDestroySurface(Display,Surface);
		eglDestroyContext(Display,Context);
	}
};

#include <GL/gl.h>

const char *TestVertexShader = R"(
#version 330

uniform mat4 MvpMatrix;

in vec2 Position;
in vec2 TexCoord;

out vec2 fTexCoord;

void main()
{
	fTexCoord = TexCoord;
	vec4 TransformedPosition = MvpMatrix * vec4(Position,0.0,1.0);
	//
	gl_Position = vec4(TransformedPosition.xy,0.0,1.0);
}
)";

const char *FragmentShaderSource = R"(
#version 330

in vec2 fTexCoord;

out vec4 FragColor;

void main()  
{
	FragColor = vec4(0.0, fTexCoord.x, fTexCoord.y, 1.0);  
}
)";

/*
 * Shader
 */

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLDELETESHADERPROC glDeleteShader;

PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

/*
 * Program
 */

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix4fv;

void InitShaderExtensions(EGL_Display &display)
{
	glCreateShader = (PFNGLCREATESHADERPROC) display.GetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) display.GetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC) display.GetProcAddress("glCompileShader");
	glDeleteShader = (PFNGLDELETESHADERPROC) display.GetProcAddress("glDeleteShader");
	//
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) display.GetProcAddress("glGetObjectParameterivARB");
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) display.GetProcAddress("glGetInfoLogARB");
	//
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) display.GetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC) display.GetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC) display.GetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC) display.GetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC) display.GetProcAddress("glUseProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) display.GetProcAddress("glDeleteProgram");
	
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) display.GetProcAddress("glGetUniformLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) display.GetProcAddress("glGetAttribLocation");
	
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) display.GetProcAddress("glUniformMatrix4fv");
}

void PrintShaderCompileError(GLuint shader)
{
	GLint Compiled;
	//
	glGetObjectParameterivARB(shader,GL_COMPILE_STATUS,&Compiled);
	if(!Compiled)
	{
		GLint RetSize;
		char Log[256];
		glGetInfoLogARB(shader,sizeof(Log),&RetSize,Log);
		//
		std::cout << Log << std::endl;
	}
}

#include "GL_Sphere.h"

/*
 * Buffer object
 */

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;

/*
 * Vertex array object
 */

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

void InitBufferExtensions(EGL_Display &display)
{
	glGenBuffers = (PFNGLGENBUFFERSPROC) display.GetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC) display.GetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC) display.GetProcAddress("glBufferData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) display.GetProcAddress("glDeleteBuffers");

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) display.GetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) display.GetProcAddress("glBindVertexArray");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) display.GetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) display.GetProcAddress("glVertexAttribPointer");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) display.GetProcAddress("glDeleteVertexArrays");
}

#include "lPhys3/lPhys3.h"


#include <GL/glx.h>

typedef const char * (*PfnQueryRendererStringMESA) (Display *dpy, int screen,
                                           int renderer, int attribute);

#include <stdlib.h>

int main(int argc, char **argv)
{
	setenv("DRI_PRIME","0",1);
	Display *display = XOpenDisplay(nullptr);
	//
	PfnQueryRendererStringMESA glXQueryRendererStringMESA = (PfnQueryRendererStringMESA)glXGetProcAddress((const unsigned char *)"glXQueryRendererStringMESA");
	//
	const char *RendererString0 = glXQueryRendererStringMESA(display,0,0,GLX_RENDERER_DEVICE_ID_MESA);
	//
	std::cout << "Display " << display << " " << RendererString0 << std::endl;
	//
    if(display == nullptr)
    {
        std::cerr << "Failed to create X display" << std::endl;
        return 1;
    }
    //
    Window window = CreateWindow(display);
	//
	EGL_Display eglDisplay(display,window);
	//
	setenv("DRI_PRIME","1",1);
	Display *display2 = XOpenDisplay(nullptr);
	//
	const char *RendererString1 = glXQueryRendererStringMESA(display2,0,0,GLX_RENDERER_DEVICE_ID_MESA);
	std::cout << "Display " << display2 << " "  << RendererString1 << std::endl;
	//
	EGL_Display eglDisplay2(display2,(Window)nullptr);
	//
	int Waiting = 1;
    while(Waiting)
    {
        XEvent Event;
	    XNextEvent(display, &Event);
	    if (Event.type == MapNotify)
        {
            Waiting = 0;
        }
    }
    //
    eglDisplay2.Prepare();
	std::cout << glGetString(GL_RENDERER) << std::endl;
	GLint UniformLocations;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&UniformLocations);
	std::cout << "Num: " << UniformLocations << std::endl;
    //
    eglDisplay.Prepare();
	std::cout << glGetString(GL_RENDERER) << std::endl;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&UniformLocations);
	std::cout << "Num: " << UniformLocations << std::endl;
	//
	InitBufferExtensions(eglDisplay);
	InitShaderExtensions(eglDisplay);
	//
	GLint MajorVersion;
	GLint MinorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion); 
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
		std::cout << "gl version: " << MajorVersion << "." << MinorVersion << std::endl;
	//
	GL_Shader Shader;
	//
	GL_Sphere Sphere(10,Shader);
	//
	lpWorld2D World;
	float SmallRadius = 2.0;
	float BigRadius = 30.0;
	//
	std::list<lpBody2D *> Bodies;
	//
	{
		lpBody2D &NewBody = World.GetBodyFactory().CreateBody().SetState({lmVector2D({-40,-40}),lmVector2D({0.0,0.0})}).SetCollisionResponse(lpBody2D::IMMOVABLE).CreateCircle().SetRadius(BigRadius).Construct().Construct();
		Bodies.push_back(&NewBody);
	}
	//
	{
		lpBody2D &NewBody = World.GetBodyFactory().CreateBody().SetState({lmVector2D({40,-50}),lmVector2D({0.0,0.0})}).SetCollisionResponse(lpBody2D::IMMOVABLE).CreateCircle().SetRadius(BigRadius).Construct().Construct();
		Bodies.push_back(&NewBody);
	}
	//
	{
		lpBody2D &NewBody = World.GetBodyFactory().CreateBody().SetState({lmVector2D({80,-20}),lmVector2D({0.0,0.0})}).SetCollisionResponse(lpBody2D::IMMOVABLE).CreateCircle().SetRadius(BigRadius).Construct().Construct();
		Bodies.push_back(&NewBody);
	}
	//
	{
		lpBody2D &NewBody = World.GetBodyFactory().CreateBody().SetState({lmVector2D({0,-60}),lmVector2D({0.0,0.0})}).SetCollisionResponse(lpBody2D::IMMOVABLE).CreateCircle().SetRadius(BigRadius).Construct().Construct();
		Bodies.push_back(&NewBody);
	}
	//
	{
		lpBody2D &NewBody = World.GetBodyFactory().CreateBody().SetState({lmVector2D({0,60}),lmVector2D({0.0,0.0})}).SetCollisionResponse(lpBody2D::IMMOVABLE).CreateCircle().SetRadius(BigRadius).Construct().Construct();
		Bodies.push_back(&NewBody);
	}
	//
	bool Running = true;
	int j=0;
	const int NUM_SAMPLES = 10;
	int Next = 0;
	float dAngle = PI / NUM_SAMPLES;
	while(Running)
	{
		//Input
		XEvent Event;
		for(int i=0;i < XPending(display);i++)
        {
			XNextEvent(display,&Event);
			if(Event.type == KeyPress)
			{
				if(XLookupKeysym(&(Event.xkey),0) == XK_Escape)
				{
					Running = false;
				}
			}
		}
		//Logic
		//
		if(j == 20)
		{
			lmVector2D NewPos = lmVector2D({std::cos(Next * dAngle),std::sin(Next * dAngle)}) * 20.0f;
			lpBody2D &NewBody = World.GetBodyFactory().CreateBody().SetState({NewPos,lmVector2D({0.0,0.0})}).CreateCircle().SetRadius(SmallRadius).Construct().Construct();
			Bodies.push_back(&NewBody);
			Next++;
			j = 0;
			//
			if(Next > NUM_SAMPLES)
			{
				Next = 0;
			}
		}
		else
		{
			j++;
		}
		//
		World.Simulate(1.0/60.0);
		//
		//Rendering
		glClearColor(1.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
		//
		Shader.UseProgram();
		//
		for(lpBody2D *Body : Bodies)
		{
			lShapeGetter CircleVisitor;
			Body->GetCollisionShape().Accept(CircleVisitor);
			float Radius = CircleVisitor.GetCircle()->GetRadius();
			//
			lmMatrix4x4 ProjectionMatrix(lmMatrix4x4::IDENTITY);
			lmScale4x4(ProjectionMatrix,lmVector3D({1.0f/80.0f,1.0f/60.0f,0.0f}));
			//
			lmMatrix4x4 MvpMatrix = ProjectionMatrix;
			lmTranslate4x4(MvpMatrix,lmVector3D({Body->GetPosition()[0],Body->GetPosition()[1],0.0}));
			lmScale4x4(MvpMatrix,lmVector3D({Radius,Radius,0.0}));
			//
			Shader.UploadMvpMatrix(MvpMatrix);
			//
			Sphere.Draw();
		}
		//
		Shader.DisableProgram();
		//
		eglDisplay.SwapBuffers();
	}
	//
	XCloseDisplay(display);
	XCloseDisplay(display2);
    //
    return 0;
}
