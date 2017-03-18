
#include "lGL3_RendererTestInstantiator.h"


#include <lTest/TestRenderer/RendererTest.h>

class RendererTestGame : public liGame
{
private:
	liController *Controller;
	RendererTest *test;
	//
	bool Up = false;
	bool Down = false;
	bool Left = false;
	bool Right = false;
	//
public:
	//
	virtual void Step() override
	{
		Up = false;
		Down = false;
		Left = false;
		Right = false;
		if(Controller != nullptr)
		{
			auto Axis0 = Controller->GetAxis(0);
			auto Axis1 = Controller->GetAxis(1);
			//
			if(Axis0 != nullptr && Axis0->GetValue() < 0.0)
			{
				Left = true;
			}
			if(Axis0 != nullptr && Axis0->GetValue() > 0.0)
			{
				Right = true;
			}
			if(Axis1 != nullptr && Axis1->GetValue() < 0.0)
			{
				Up = true;
			}
			if(Axis1 != nullptr && Axis1->GetValue() > 0.0)
			{
				Down = true;
			}
		}
		//
		test->Input(Up,Down,Left,Right);
		test->Loop();
	}
	//
	RendererTestGame(liController *controller,liRenderer *renderer)
	{
		Controller = controller;
		//
		test = new RendererFunctionalityTest(renderer);
		test->Initialize();
	}
	virtual ~RendererTestGame() override
	{
		delete test;
	}
};

liGame* lGL3_RendererTestInstantiator::CreateGame(liApiAdapter& api_adapter, liResourceManager& resource_manager, liRenderer& renderer, liConsole& console)
{
	renderer.SetResourceManager(Dummy);
	RendererTestGame *NewGame = new RendererTestGame(api_adapter.GetInput().GetController(0),&renderer);
	//
	return NewGame;
}


liGameInstantiator *CreateGameInstantiator()
{
    return new lGL3_RendererTestInstantiator;
}