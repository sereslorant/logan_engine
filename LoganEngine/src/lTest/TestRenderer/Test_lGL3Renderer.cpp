
/*
 * SDL include-ok
 */

#include <SDL2/SDL.h>

/*
 * STL include-ok
 */
#include <iostream>

#include "RendererTest.h"
#include "DummyRM.h"

RendererTest *test;
DummyRM *dummyRM;
liRenderer *Renderer;

bool Up = false;
bool Down = false;
bool Left = false;
bool Right = false;

void GetInput(bool &is_running)
{
	SDL_Event Event;
	while(SDL_PollEvent(&Event))
	{
		if(Event.type == SDL_QUIT)
		{
			is_running = false;
		}

		if(Event.type == SDL_KEYDOWN)
		{
			if(Event.key.keysym.sym == SDLK_UP)
			{Up = true;}
			//
			if(Event.key.keysym.sym == SDLK_DOWN)
			{Down = true;}
			//
			if(Event.key.keysym.sym == SDLK_LEFT)
			{Left = true;}
			//
			if(Event.key.keysym.sym == SDLK_RIGHT)
			{Right = true;}
		}
		else if(Event.type == SDL_KEYUP)
		{
			if(Event.key.keysym.sym == SDLK_UP)
			{Up = false;}
			//
			if(Event.key.keysym.sym == SDLK_DOWN)
			{Down = false;}
			//
			if(Event.key.keysym.sym == SDLK_LEFT)
			{Left = false;}
			//
			if(Event.key.keysym.sym == SDLK_RIGHT)
			{Right = false;}
		}
	}
}

void AtExit()
{
	//std::cout << "Exiting program" << std::endl;
	delete test;
	delete Renderer;
	delete dummyRM;
}

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

class lGLFW3ExtensionLoader : public liGLExtLoader::liExtFunctionLoader
{
private:

public:

	virtual void *LoadFunction(const std::string &function_name) override
	{
		return (void *)SDL_GL_GetProcAddress(function_name.c_str());
	}

	lGLFW3ExtensionLoader()
	{}

	virtual ~lGLFW3ExtensionLoader() override
	{}
};

//#include "../../lTest/lffGLRenderer/lffGLRenderer.h"
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3Renderer.h>

int main(int argc, char *argv[])
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//
	SDL_Window *Window = SDL_CreateWindow("Renderer test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,600,SDL_WINDOW_OPENGL);
	SDL_GLContext GLContext = SDL_GL_CreateContext(Window);
	//
	SDL_ShowWindow(Window);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	lGLFW3ExtensionLoader ExtensionLoader;
	liGLExtLoader &ExtLoader = lGetGLExtLoader();

	ExtLoader.SetExtFunctionLoader(&ExtensionLoader);
	ExtLoader.LoadExtensions();

	ExtLoader.PrintExtStatus(std::cout);

	dummyRM = new DummyRM;
	//Renderer = new lffGLRenderer(600,600,*dummyRM);
	Renderer = new lrGL3Renderer(600,600);
	Renderer->SetResourceManager(dummyRM);
	test = new RendererFunctionalityTest(Renderer);

	test->Initialize();

	atexit(AtExit);

	bool IsRunning = true;
	while(IsRunning)
	{
		GetInput(IsRunning);
		//
		test->Input(Up,Down,Left,Right);
		test->Loop();
		test->Draw();
		//
		SDL_GL_SwapWindow(Window);
	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
