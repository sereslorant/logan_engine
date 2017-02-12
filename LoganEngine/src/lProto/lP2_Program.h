#ifndef LP2_PROGRAM_H
#define LP2_PROGRAM_H

#include "liGameInstantiator.h"

#include "../lGame/lConsole.h"

class lP2_Program
{
private:
	//
	liApiAdapter &ApiAdapter;
	lrmResourceManager &ResourceManager;
	liRenderer &Renderer;
	liGameInstantiator &GameInstantiator;
	//
	liConsole *Console;
	liThread *ConsoleThread = nullptr;
	//
	liGame *Game = nullptr;
	//
public:
	//
	void MainLoop()
	{
		if(ConsoleThread != nullptr)
			{ConsoleThread->Start();}
		//
		Game = GameInstantiator.CreateGame(ApiAdapter,ResourceManager,Renderer,*Console);
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
			Renderer.Render();
			ApiAdapter.SwapBuffers();
			//
			ApiAdapter.GetFrameLimiter().CheckFrameLimiter();
		}
		//
		//ConsoleThread->Kill();
	}
	//
    lP2_Program(liApiAdapter &api_adapter,liThreadFactory &thread_factory,lrmResourceManager &resource_manager,liRenderer &renderer,liGameInstantiator &game_instantiator,bool has_console = false)
		:ApiAdapter(api_adapter),ResourceManager(resource_manager),Renderer(renderer),GameInstantiator(game_instantiator)
    {
		if(has_console)
		{
			lConsole *RunnableConsole = new lConsole(std::cin,std::cout,std::cerr);
			Console = RunnableConsole;
			//
			ConsoleThread = thread_factory.NewThread();
			ConsoleThread->SetRunnable(RunnableConsole);
		}
		else
		{
			Console = new lConsoleNullObject;
		}
		//
    }
    //
    ~lP2_Program()
    {
    	delete Game;
		//
		if(ConsoleThread != nullptr)
			{delete ConsoleThread;}
		//
		delete Console;
    }
};

#endif //LP2_PROGRAM_H
