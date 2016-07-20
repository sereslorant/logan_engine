
#include "../lGame/lConsole.h"

#include "lGameModes/lPacMan/lPacMan.h"
#include "lGame/lP2Game.h"
#include "lGame/lP2World2D.h"

#include "../lInterfaces/lRenderer/li2DRenderer.h"
#include "lffGLRenderer.h"


#include "../lInterfaces/liApiAdapter.h"

/*
 * A konzol miatt kell.
 */
#include <iostream>

class lP2_Program
{
private:
	liApiAdapter &ApiAdapter;

	liConsole *Console;
	liThread *ConsoleThread = nullptr;

	liGame *Game;
	liRenderer *Renderer;
	lrmResourceManager ResourceManager;

public:

	void MainLoop()
	{
		if(ConsoleThread != nullptr)
			{ConsoleThread->Start();}
		//
		ApiAdapter.GetFrameLimiter().SetFPS(60);
		//
		bool IsRunning = true;
		while(IsRunning)
		{
			ApiAdapter.GetFrameLimiter().StartFrameLimiter();
			//
			ApiAdapter.PollInput();
			if(ApiAdapter.GetInput().GetQuit())
				{IsRunning = false;}
			//
			Game->Step();
			//
			Renderer->Render();
			ApiAdapter.SwapBuffers();
			//
			ApiAdapter.GetFrameLimiter().CheckFrameLimiter();
		}
		//
		//ConsoleThread->Kill();
	}

    lP2_Program(liApiAdapter &api_adapter,bool has_console = false)
		:ApiAdapter(api_adapter)
    {
		if(has_console)
		{
			lConsole *RunnableConsole = new lConsole(std::cin,std::cout,std::cerr);
			Console = RunnableConsole;
			//
			ConsoleThread = ApiAdapter.GetThreadFactory().CreateThread();
			ConsoleThread->SetRunnable(RunnableConsole);
		}
		else
		{
			Console = new lConsoleNullObject;
		}
		//
		Renderer = new lffGLRenderer(ResourceManager);
		//
		#ifdef PAC_MAN
			liGameMode *GameMode = new lPM_Game(ApiAdapter.GetInput(),*Renderer,0.125);//new lGameMode();
			Game = new lGame(*Console,GameMode,*Renderer);
		#else
			liWorld2D *World = new lP2World2D;
			liGameMode *GameMode = new lP2ProtoGameMode(ApiAdapter.GetInput(),*World,*Renderer);
			Game = new lSimulationGame<liWorld2D>(*Console,GameMode,*Renderer,World);
		#endif
    }

    ~lP2_Program()
    {
    	delete Game;
		delete Renderer;
		//
		if(ConsoleThread != nullptr)
			{delete ConsoleThread;}
		//
		delete Console;
    }
};

#include "../lApiAdapter/lSDL2_ApiAdapter/lSDL2_ApiAdapter.h"

static const std::string DEF_TITLE = "A kurva anyadat";
static const bool DEF_FULLSCREEN = false;
static const unsigned int DEF_WIDTH = 800;
static const unsigned int DEF_HEIGHT = 600;
static const lSDL2_Settings::lRenderingApi DEF_RENDERING_API = lSDL2_Settings::OPENGL;

int main(int argc, char *argv[])
{
	lSDL2_Settings Settings;
	//
	Settings.Title = DEF_TITLE;
	Settings.Fullscreen = DEF_FULLSCREEN;
	Settings.Width = DEF_WIDTH;
	Settings.Height = DEF_HEIGHT;
	Settings.RenderingApi = DEF_RENDERING_API;

	lSDL2_ApiAdapter ApiAdapter(Settings);

	lP2_Program Program(ApiAdapter);

	Program.MainLoop();
	//fclose(stdin);

	return 0;
}
