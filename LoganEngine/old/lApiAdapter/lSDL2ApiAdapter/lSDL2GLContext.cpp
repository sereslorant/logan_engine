
#include "lSDL2GLContext.h"

using namespace std;

int lSDL2GLContext::GetError()
{
    return Error;
}

void lSDL2GLContext::PrintStatus(ostream &out)
{
    if(Error == ERR_OK)
    {out << "GL Context successfully initialized\nGL Version: " << GL_MajorVersion << '.' << GL_MinorVersion << "\n";}
    else
    {out << "Error: Couldn't create GL context\n" << "\n";}

    GLExtLoader.PrintStatus(out);
}

void lSDL2GLContext::CreateRenderer(liRenderer **Target)
{
    *Target = CreateGL3ForwardRenderer();//new lrGL3_3ForwardRenderer;
}

void lSDL2GLContext::Enable(SDL_Window *Window)
{
    SDL_GL_MakeCurrent(Window,GLContext);
}

void lSDL2GLContext::SwapBuffers(SDL_Window *Window)
{
    SDL_GL_SwapWindow(Window);
}

void lSDL2GLContext::SetAttributes()
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,GL_MajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,GL_MinorVersion);

    if(GL_ES)
    {SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);}
    else
    {SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);}

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

}


extern "C" {

liRenderer *lrCreateGL3ForwardRenderer();

lGLExt *lGLGetGLExt();

}

void lSDL2GLContext::CreateContext(SDL_Window *Window)
{
    GLContext = SDL_GL_CreateContext(Window);
    if(GLContext == nullptr)
    {
        Error |= ERR_COULDNT_CREATE_CONTEXT;
        return;
    }

    //GL_RendererLib = SDL_LoadObject(GL_RENDERER_LIB_NAME.c_str());
    //GetGLExt = (lGLExt* (*) ())SDL_LoadFunction(GL_RendererLib,GET_GL_EXT_NAME.c_str());

    GetGLExt = lGLGetGLExt;

    lGLExt *GLExt = GetGLExt();
    GLExtLoader.LoadGLExtensions(GLExt);

    GLExtLoader.PrintStatus(std::cout);

    std::cout << std::endl;

    //CreateGL3ForwardRenderer = (liRenderer* (*) ())SDL_LoadFunction(GL_RendererLib,CREATE_GL3_FORWARD_RENDERER_NAME.c_str());
    CreateGL3ForwardRenderer = lrCreateGL3ForwardRenderer;
}

lSDL2GLContext::lSDL2GLContext(unsigned int gl_MajorVersion,unsigned int gl_MinorVersion,bool gl_ES)
    :GL_MajorVersion(gl_MajorVersion),GL_MinorVersion(gl_MinorVersion),GL_ES(gl_ES)
{
    Error = ERR_OK;
}

lSDL2GLContext::~lSDL2GLContext()
{
	//SDL_UnloadObject(GL_RendererLib);

    SDL_GL_DeleteContext(GLContext);
}

