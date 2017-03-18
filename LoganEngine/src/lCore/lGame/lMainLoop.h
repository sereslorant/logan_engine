#ifndef LP2_PROGRAM_H
#define LP2_PROGRAM_H

#include <lGame/liGameInstantiator.h>

#include <lApiAdapter/lConsole.h>

class lMainLoop
{
private:
	//
	liApiAdapter &ApiAdapter;
	liResourceManager &ResourceManager;
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
    lMainLoop(liApiAdapter &api_adapter,liThreadFactory &thread_factory,liResourceManager &resource_manager,liRenderer &renderer,liGameInstantiator &game_instantiator,bool has_console = false)
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
    ~lMainLoop()
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
