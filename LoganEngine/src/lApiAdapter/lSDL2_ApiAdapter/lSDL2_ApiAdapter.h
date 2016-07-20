#ifndef L_SDL2_INTERFACE_H
#define L_SDL2_INTERFACE_H

#include "../../lInterfaces/liApiAdapter.h"

#include <SDL2/SDL.h>

#include "lSDL2_Input/lSDL2_Input.h"
#include "lSDL2_FrameLimiter.h"
#include "lSDL2_Thread/lSDL2_Thread.h"

#include <fstream>
#include <string>

struct lSDL2_Settings
{
public:
	//
	enum lRenderingApi
	{
		OPENGL    = 0,
		//OPENGL_ES = 1,
	};
	//
	std::string  Title;
	bool         Fullscreen = false;
	unsigned int Width = 640;
	unsigned int Height = 480;
	//
	lRenderingApi RenderingApi = OPENGL;
	//
	//Az SDL ablakkészítő függvényének szánt flageket állítja be a beállítások alapján.
	void SetFlags(int *flags) const
	{
		if(Fullscreen)
		{
			*flags |= SDL_WINDOW_FULLSCREEN;
		}
		//
		if(RenderingApi == OPENGL /*|| RenderingApi == OPENGL_ES*/)
		{
			*flags |= SDL_WINDOW_OPENGL;
		}
	}
	//
	lSDL2_Settings()
	{}
	//
	~lSDL2_Settings()
	{}
};

class lSDL2_ApiAdapter : public liApiAdapter
{
public:
	//
    enum LSDL2_ERROR_BITS
    {
        ERR_OK = 0,
        ERR_COULDNT_CREATE_WINDOW               = 0x00000001,
        ERR_COULDNT_CREATE_RENDERING_CONTEXT    = 0x00000002,
    };
	//
private:
    lSDL2_Input			*Input = nullptr;			/**< Az inputkezelő példányára mutató pointer */
    lSDL2_FrameLimiter	FrameLimiter;	/**< A frame limiter példányára mutató pointer */
	lSDL2_ThreadFactory ThreadFactory;
    //
    int Error; //Integer, amelynek az egyes bitjeit 1-esbe állítjuk, ha valami hiba van.
	//
    SDL_Window *Window; //Az SDL Window
    SDL_GLContext GLContext;
	//
public:

    /** \brief Getter függvény, visszaadja az inputkezelőre mutató pointert.
     *
     * \return lInput *: Az inputkezelőre mutató pointer
     *
     */
    virtual liInput &GetInput() override
	{
		return *Input;
	}

    /** \brief Getter függvény, visszaadja a frame limiterre mutató pointert.
     *
     * \return liFrameLimiter *: A frame limiterre mutató pointer
     *
     */
    virtual liFrameLimiter &GetFrameLimiter() override
    {
		return FrameLimiter;
	}

    /** \brief Getter függvény, visszaadja a rendererre mutató pointert.
     *
     * \return lRenderer *: A rendererre mutató pointer
     *
     */
    /*
    virtual liRenderer *GetRenderer() override
    {
		return Renderer;
	}

    virtual liGuiRenderer *GetGuiRenderer() override
    {
		return GuiRenderer;
    }
    */
    /** \brief Getter függvény, visszaadja a resource managerre mutató pointert.
     *
     * \return lrm_RM *: A resource managerre mutató pointer
     *
     */
    /*
    virtual liResourceManager *GetResourceManager() override
    {
		return ResourceManager;
	}
	*/
	//
    //int GetError();
    //
    virtual liThreadFactory &GetThreadFactory() override
    {
    	return ThreadFactory;
    }

	virtual void PollInput() override
	{
		Input->PollInput();
	}

    virtual void SwapBuffers() override
    {
		SDL_GL_SwapWindow(Window);
	}
    //
    void PrintStatus(std::ostream &out)
    {
		if(Error == ERR_OK)
		{
			out << "Api adapter successfully initialized\n";
		}
		//
		if(Error & ERR_COULDNT_CREATE_WINDOW)
		{
			out << "Error: Couldn't create Window\n";
		}
		//
		if(Error & ERR_COULDNT_CREATE_RENDERING_CONTEXT)
		{
			out << "Error: Couldn't create rendering context\n";
		}
		/*
		if(RenderingContext != nullptr)
		{
			RenderingContext->PrintStatus(out);
		}
		*/
		out << std::endl;
	}


    lSDL2_ApiAdapter(const lSDL2_Settings &settings)
		:Error(0x0)
    {
    	SDL_Init(SDL_INIT_EVERYTHING);
		//
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    	//
    	int Flags = 0x0;
    	settings.SetFlags(&Flags);
    	//
    	Window = SDL_CreateWindow(settings.Title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,settings.Width,settings.Height,Flags);
		if(Window == nullptr)
        {
        	Error |= ERR_COULDNT_CREATE_WINDOW;
            return;
        }
		//
		GLContext = SDL_GL_CreateContext(Window);
		if(GLContext == nullptr)
		{
			Error |= ERR_COULDNT_CREATE_RENDERING_CONTEXT;
			return;
		}
		//
		SDL_ShowWindow(Window);
		//
		Input = new lSDL2_Input;
    }

    virtual ~lSDL2_ApiAdapter() override
    {
    	delete Input;
    	//
		SDL_GL_DeleteContext(GLContext);
		SDL_DestroyWindow(Window);
    }
};

#endif
