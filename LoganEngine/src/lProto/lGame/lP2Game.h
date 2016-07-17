#ifndef LP2_GAME_H
#define LP2_GAME_H

#include "../../lRenderer/lr2DScene/lr2DScene.h"
#include "../../lInterfaces/lRenderer/li2DRenderer.h"
#include "../../lGame/lGame.h"
#include "liWorld2D.h"

#include <list>

class lP2ProtoGameMode : public liGameMode
{
protected:
	class Set2DLayer : public liLayerVisitor
	{
	private:
		li2DScene *Scene;
		li2DCamera *Camera;

	public:

		virtual void Visit2DLayer(li2DLayer &layer) override
		{
			layer.SetScene(Scene);
			layer.SetCamera(Camera);
		}

		Set2DLayer(li2DScene *scene,li2DCamera *camera)
			:Scene(scene),Camera(camera)
		{}

		virtual ~Set2DLayer() override {}
	};

	liInput &Input;
	//
	liWorld2D &World;
	li2DRenderer &Renderer;
	//
	liViewport *Viewport;
	//
	liLayer *GameLayer;
	liLayer *GuiLayer;
	//
	lr2DScene *GameScene;
	lr2DCamera *GameCamera;
	//
	lr2DScene *GuiScene;
	lr2DCamera *GuiCamera;
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
	lP2ProtoGameMode(liInput &input,liWorld2D &world,li2DRenderer &renderer)
		:Input(input),World(world),Renderer(renderer)
	{
		unsigned int Width = Renderer.GetMainFramebuffer().GetWidth();
		unsigned int Height = Renderer.GetMainFramebuffer().GetHeight();
		//
		Viewport = Renderer.GetMainFramebuffer().CreateViewport(0,0,Width,Height);
		//
		//
		GameScene = new lr2DScene;
		GameScene->SetBackgroundColor(lrColor(1.0,1.0,1.0,1.0));
		GameCamera = new lr2DCamera({0.0,0.0},Width,Height);
		//
		GuiScene = new lr2DScene;
		GuiCamera = new lr2DCamera({0.0,0.0},Width,Height);
		//
		Set2DLayer SetGameLayer(GameScene,GameCamera);
		Set2DLayer SetGuiLayer(GuiScene,GuiCamera);
		//
		GameLayer = Viewport->Create2DLayer();
		GuiLayer = Viewport->Create2DLayer();
		//
		GameLayer->Accept(SetGameLayer);
		GuiLayer->Accept(SetGuiLayer);
		//
		GameLayer->Enable();
		//GuiLayer->Enable();
		//
		if(Input.GetNumControllers() != 0)
		{
			lmVector2D Position = {0.0,0.0};
			lmVector2D Velocity = {0.0,0.0};
			Agents.push_back(new lPlayerAgent(World.CreateBody(Position,Velocity),GameScene->GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0),Input.GetController(0),GameCamera));
		}
		//
		for(unsigned int i=0;i < 5;i++)
		{
			lmVector2D Position = {100 + i*80.0,100.0};
			lmVector2D Velocity = {0.0,0.0};
			Agents.push_back(new lAgent(World.CreateBody(Position,Velocity),GameScene->GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0)));
		}
	}
	//
	virtual ~lP2ProtoGameMode() override
	{
		for(liAgent *Agent : Agents)
		{
			delete Agent;
		}
		//
		delete GameScene;
		delete GameCamera;
		//
		delete GuiScene;
		delete GuiCamera;
	}
};

#endif //LP2_GAME_H

