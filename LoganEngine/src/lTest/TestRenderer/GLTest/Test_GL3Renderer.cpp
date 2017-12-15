
#include <lTest/Utils/lSDL2GLTestExtLoader.h>
#include <lTest/Utils/lSDL2TestFramework.h>

/*
 * STL include-ok
 */

#include "../RendererTest.h"
#include "../DummyRM.h"

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

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3Renderer.h>

int main(int argc, char *argv[])
{
	lSDL2GLTestFramework GLTestFramework(3,3,600,600);

	lSDL2ExtensionLoader ExtensionLoader;
	liGLExtLoader &ExtLoader = lGetGLExtLoader();

	ExtLoader.SetExtFunctionLoader(&ExtensionLoader);
	ExtLoader.LoadExtensions();

	ExtLoader.PrintExtStatus(std::cout);

	dummyRM = new DummyRM;
	Renderer = new lrGL3Renderer(600,600);
	Renderer->SetResourceManager(dummyRM);
	test = new RendererFunctionalityTest(Renderer);

	test->Initialize();

	atexit(AtExit);

	bool IsRunning = true;
	while(IsRunning)
	{
		GetInput(IsRunning);
		
		test->Input(Up,Down,Left,Right);
		test->Loop();
		test->Draw();
		
		GLTestFramework.SwapWindow();
	}
	
	return 0;
}
