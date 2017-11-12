#ifndef LSDL2_TEST_FRAMEWORK_H
#define LSDL2_TEST_FRAMEWORK_H

#include <SDL2/SDL.h>

class lSDL2GLTestFramework
{
private:
	SDL_Window *Window;
	SDL_GLContext GLContext;
	
public:
	
	void SwapWindow()
	{
		SDL_GL_SwapWindow(Window);
	}
	
	lSDL2GLTestFramework(int gl_major_version,int gl_minor_version,unsigned int width,unsigned int height)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version);
		
		Window = SDL_CreateWindow("Renderer test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_OPENGL);
		GLContext = SDL_GL_CreateContext(Window);
		
		SDL_ShowWindow(Window);
	}
	
	~lSDL2GLTestFramework()
	{
		SDL_GL_DeleteContext(GLContext);
		SDL_DestroyWindow(Window);
		SDL_Quit();
	}
};

#endif // LSDL2_TEST_FRAMEWORK_H
