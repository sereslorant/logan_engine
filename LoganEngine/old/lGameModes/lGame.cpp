
#include "lGame.h"

#include "../lGooey/lGooObjectFactory.h"





void lGame::SetNextMap(const std::string &next_map)
{
    NextMap = next_map;
}

void lGame::QuitGame()
{
    IsRunning = false;
    RetValue = 0;
}

void lGame::QuitProgram()
{
    IsRunning = false;
    RetValue = -1;
}

void lGame::SetPaused(bool is_paused)
{
    IsPaused = is_paused;
}

int lGame::MainLoop()
{
    HUD->GetDocBeginEvent().Fire();

    while(IsRunning)
    {
        FrameLimiter->StartFrameLimiter();
        //Input
        Input->CheckInput();
        if(Input->GetQuit())
        {
            QuitProgram();
        }

        //Logic
        Logic();

        if(!IsPaused)
        {
            SimulationWorld->Simulate(dt);

             /*
            SimulationWorld->Simulate(dt/4.0);
            SimulationWorld->Simulate(dt/4.0);
            SimulationWorld->Simulate(dt/4.0);
            SimulationWorld->Simulate(dt/4.0);
            // */

            for(liAgent *Agent : RunningAgents)
			{
				Agent->Update(dt);
			}
        }
        else
		{
            for(liAgent *Agent : PausedAgents)
			{
				Agent->Update(dt);
			}
		}

        HUD->Step(dt);

        Renderer->SetScene(Frustum,Camera,BgColor);
        Renderer->Draw(Renderer->GetScreen());

        GuiRenderer->SetRenderer(HUD->GetDisplayList());
        GuiRenderer->Draw(Renderer->GetScreen());
        //GuiRenderer->ClearDL();

        ApiAdapter->SwapBuffers();
        FrameLimiter->CheckFrameLimiter();
    }

    return RetValue;
}

lGame::lGame(liApiAdapter *api_adapter,std::string &next_map)
    :NextMap(next_map),IsRunning(true),RetValue(0),IsPaused(false),ApiAdapter(api_adapter),BgColor(0.0,0.0,0.0,1.0)
{
    //Making a few shortcuts to certain objects so we can write shorter code
    Input          = ApiAdapter->GetInput();
    FrameLimiter   = ApiAdapter->GetFrameLimiter();
    Renderer       = ApiAdapter->GetRenderer();
    GuiRenderer    = ApiAdapter->GetGuiRenderer();
    RM             = ApiAdapter->GetResourceManager();

    GooObjectFactory = new lGooObjectFactory;
    GooObjectFactory->SetRenderer(GuiRenderer);

    Frustum	= Renderer->CreateFrustum(0.7,-0.7,-0.7,0.7,-500.0,1.0);
    Camera	= Renderer->CreateCamera({0.0,0.0,0.0},0.0,0.0);

    //Initializing physics engine
    lmVector3D GravitationalAcceleration = {0.0,-9.81,0.0};
    lmScalar AirDensity = 0.012;
    lmScalar CoefficientOfFriction = 0.5;

    SimulationWorld = CreateSimulationWorld(/*Integrator*/);

    SimulationWorld->GetInteractionModel()->SetGravAcceleration(GravitationalAcceleration);
    SimulationWorld->GetInteractionModel()->SetAirDensity(AirDensity);
    SimulationWorld->GetInteractionModel()->SetCoefficientOfFriction(CoefficientOfFriction);

    //Initializing HUD
    HUD = GooObjectFactory->NewGUI()->ToGUI();//new lGooGUI({0.0,0.0,0.0},{1.0,1.0},0.0,800.0,600.0);
    HUD->SetResX(800.0);
    HUD->SetResY(600.0);
}

lGame::~lGame()
{
    Renderer->ClearDL();

    //Freeing physically simulated objects
    /*for(auto &I : SimulatedSceneNodes)
    {
        delete I;
    }*/

    for(auto I : RunningAgents)
	{
		delete I;
	}

	for(auto I : PausedAgents)
	{
		delete I;
	}

    delete SimulationWorld;
    delete GooObjectFactory;

    //Freeing HUD
    delete HUD;

    //Freeing resources required for this game
	Renderer->FreePointLights();
    RM->Clean();
}
