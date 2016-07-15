#ifndef LP2_GAME_H
#define LP2_GAME_H

#include "../../lInterfaces/lRenderer/li2DRenderer.h"
#include "../../lGame/lGame.h"
#include "liWorld2D.h"

#include <list>

class lP2ProtoGameMode : public liGameMode
{
protected:
	liInput &Input;
	//
	liWorld2D &World;
	li2DScene &GameScene;
	//li2DScene &GuiScene;
	//
	constexpr static lmScalar RADIUS = 25.0;
	//
	class lAgent : public liAgent
	{
	protected:
		//
		bool Removable = false;
		//
		liBody2D *Body;
		li2DElement *Element;
		//
	public:
		//
		virtual void MarkRemovable() override
		{
			Removable = true;
		}
		//
		virtual bool IsRemovable() override
		{
			return Removable;
		}
		//
		virtual void Update(double dt) override
		{
			Element->SetPosition(Body->GetPosition() - lmVector2D({RADIUS,RADIUS}));
		}
		//
		lAgent(liBody2D *body,li2DElement *element)
			:Body(body),Element(element)
		{
			lGetCircle GetCircle;
			Body->GetCollisionShape()->Accept(&GetCircle);
			//
			liCircle *Circle_I = GetCircle.GetCircle();
			Circle_I->SetRadius(RADIUS);
		}
		//
		virtual ~lAgent(){}
	};
	//
	class lPlayerAgent : public lAgent
	{
	protected:
		liController *Controller;
		li2DCamera *Camera;
		float Speed = 100.0;
		//
	public:
		//
		virtual void Update(double dt) override
		{
			lmVector2D NewVelocity = {0.0,0.0};
			if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) > 0.01)
			{
				NewVelocity += {0.0, Speed};
			}
			//
			if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) < -0.01)
			{
				NewVelocity += {0.0,-Speed};
			}
			//
			if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) > 0.01)
			{
				NewVelocity += { Speed,0.0};
			}
			//
			if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) < -0.01)
			{
				NewVelocity += {-Speed,0.0};
			}
			//
			Body->SetVelocity(NewVelocity);

			lAgent::Update(dt);
			Camera->SetPosition(Body->GetPosition() - lmVector2D({Camera->GetWidth()/2.0,Camera->GetHeight()/2.0}));
		}
		//
		lPlayerAgent(liBody2D *body,li2DElement *element,liController *controller,li2DCamera *camera)
			:lAgent(body,element),Controller(controller),Camera(camera)
		{}
		//
		virtual ~lPlayerAgent() override
		{}
	};
	//
	std::list<liAgent *> Agents;
	//
public:
	//
	virtual void Logic(double dt) override
	{
		for(auto I = Agents.begin();I != Agents.end();)
		{
			if((*I)->IsRemovable())
			{
				auto J = I;
				I++;
				Agents.erase(J);
			}
			else
			{
				//liAgent *Agent = *I;
				(*I)->Update(dt);
				I++;
			}
		}
	}
	//
	lP2ProtoGameMode(liInput &input,liWorld2D &world,li2DScene &game_scene,li2DCamera &game_camera)
		:Input(input),World(world),GameScene(game_scene)
	{
		if(Input.GetNumControllers() != 0)
		{
			lmVector2D Position = {0.0,0.0};
			lmVector2D Velocity = {0.0,0.0};
			Agents.push_back(new lPlayerAgent(World.CreateBody(Position,Velocity),GameScene.GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0),Input.GetController(0),&game_camera));
		}
		//
		for(unsigned int i=0;i < 5;i++)
		{
			lmVector2D Position = {100 + i*80.0,100.0};
			lmVector2D Velocity = {0.0,0.0};
			Agents.push_back(new lAgent(World.CreateBody(Position,Velocity),GameScene.GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0)));
		}
	}
	//
	virtual ~lP2ProtoGameMode() override
	{
		for(liAgent *Agent : Agents)
		{
			delete Agent;
		}
	}
};

class lP2ProtoGame : public lGame
{
protected:
	//
	liWorld2D *World;
	bool SimulationPaused = false;
	//
public:
	//
	virtual void Step() override
	{
		if(!SimulationPaused)
		{
			World->Step(dt);
		}
		//
		GameMode->Logic(dt);
	}
	//
	lP2ProtoGame(liConsole &console,liGameMode *game_mode,li2DScene *scene,li2DCamera *camera,liWorld2D *world)
		:lGame(console,game_mode,scene,camera),World(world)
	{
		//
	}
	//
	virtual ~lP2ProtoGame() override
	{
		delete World;
	}
};


#endif //LP2_GAME_H

