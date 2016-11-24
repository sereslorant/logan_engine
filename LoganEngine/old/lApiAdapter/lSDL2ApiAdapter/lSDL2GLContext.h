#ifndef L_SDL2_GL_CONTEXT
#define L_SDL2_GL_CONTEXT

#include "lSDL2ApiAdapter.h"
#include "lSDL2GLExtLoader.h"

class lSDL2GLContext final : public lSDL2ApiAdapter::lSDL2RenderingContext
{
public:
    enum ERROR_BITS
    {
        ERR_OK = 0,
        ERR_COULDNT_CREATE_CONTEXT = 0x00000001,
    };
private:
    int Error; //Integer, amelynek az egyes bitjeit 1-esbe állítjuk, ha valami hiba van.

    unsigned int GL_MajorVersion;
    unsigned int GL_MinorVersion;
    bool GL_ES;

    SDL_GLContext GLContext;

	const std::string GL_RENDERER_LIB_NAME = "liblrGLRenderer.so";
	const std::string CREATE_GL3_FORWARD_RENDERER_NAME = "lrCreateGL3ForwardRenderer";
	const std::string GET_GL_EXT_NAME = "GetGLExt";
	void *GL_RendererLib;

	liRenderer* (*CreateGL3ForwardRenderer) ();
	lGLExt* (*GetGLExt)();

    lSDL2GLExtLoader GLExtLoader;
public:
    int GetError();
    virtual void CreateRenderer(liRenderer **Target) final;

    virtual void PrintStatus(std::ostream &out) final;
    virtual void Enable(SDL_Window *Window) final;
    virtual void SwapBuffers(SDL_Window *Window) final;

    void SetAttributes();
    void CreateContext(SDL_Window *Window);

    lSDL2GLContext(unsigned int gl_MajorVersion,unsigned int gl_MinorVersion,bool gl_ES);
    ~lSDL2GLContext();
};

#endif
