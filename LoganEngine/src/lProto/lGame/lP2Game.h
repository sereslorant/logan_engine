#ifndef LP2_GAME_H
#define LP2_GAME_H

#include "../../lRenderer/lr2DScene/lr2DScene.h"
#include "../../lRenderer/lr3DScene/lr3DScene.h"
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

		virtual void Visit3DLayer(li3DLayer &layer) override
		{
			//Üres
		}

		Set2DLayer(li2DScene *scene,li2DCamera *camera)
			:Scene(scene),Camera(camera)
		{}

		virtual ~Set2DLayer() override {}
	};

	class Set3DLayer : public liLayerVisitor
	{
	private:
		//li3DScene *Scene;
		liFrustum *Frustum;
		li3DCamera *Camera;

	public:

		virtual void Visit2DLayer(li2DLayer &layer) override
		{
			//Üres
		}

		virtual void Visit3DLayer(li3DLayer &layer) override
		{
			//layer.SetScene(Scene);
			layer.SetFrustum(Frustum);
			layer.SetCamera(Camera);
		}

		Set3DLayer(/*li3DScene *scene,*/liFrustum *frustum,li3DCamera *camera)
			:/*Scene(scene),*/Frustum(frustum),Camera(camera)
		{}

		virtual ~Set3DLayer() override {}
	};

	liInput &Input;
	//
	liWorld2D &World;
	liRenderer &Renderer;
	//
	liViewport *Viewport;
	liViewport *Viewport3D;
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
	lrFrustum *Frustum3D;
	lr3DCamera *Camera3D;
	liController *Controller;
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
			Camera->SetPosition(Body->GetPosition() - lmVector2D({Camera->GetWidth()/2.0f,Camera->GetHeight()/2.0f}));
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
	bool Mode = true;
	//
public:
	//
	virtual void Logic(double dt) override
	{
		if(Mode)
		{
			if(Controller != nullptr)
			{
				lmVector3D Displacement;
				lmScalar Dist = 0.25;
				if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) > 0.01)
				{
					Displacement += Dist*Camera3D->GetDirection();
				}
				//
				if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) < -0.01)
				{
					Displacement -= Dist*Camera3D->GetDirection();
				}
				Camera3D->SetPosition(Camera3D->GetPosition() + Displacement);
				//
				float Angle = 0.0;
				if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) > 0.01)
				{
					Angle += PI/24.0;
				}
				//
				if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) < -0.01)
				{
					Angle -= PI/24.0;
				}
				Camera3D->SetYaw(Camera3D->GetYaw() + Angle);
			}
		}
		else
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
	}
	//
	lP2ProtoGameMode(liInput &input,liWorld2D &world,liRenderer &renderer)
		:Input(input),World(world),Renderer(renderer)
	{
		unsigned int Width = Renderer.GetMainFramebuffer().GetWidth();
		unsigned int Height = Renderer.GetMainFramebuffer().GetHeight();
		//
		Viewport = Renderer.GetMainFramebuffer().CreateViewport(0,0,Width,Height/2);
		Viewport3D = Renderer.GetMainFramebuffer().CreateViewport(0,Height/2,Width,Height/2);
		//
		liLayer *TestLayer = Viewport3D->Create3DLayer();
		//
		Frustum3D = new lrFrustum(0.5,-0.5,-0.5,0.5,100.0,1.0);
		Camera3D = new lr3DCamera(lmVector3D({0.0,0.0,0.0}),0.0,0.0);
		Controller = input.GetController(0);
		//
		Set3DLayer SetTestLayer(Frustum3D,Camera3D);
		TestLayer->Accept(SetTestLayer);
		//
		TestLayer->Enable();
		//
		GameScene = new lr2DScene;
		GameScene->SetBackgroundColor(lrColor(1.0,1.0,1.0,1.0));
		GameCamera = new lr2DCamera({0.0,0.0},Width,Height/2.0);
		//
		GuiScene = new lr2DScene;
		GuiCamera = new lr2DCamera({0.0,0.0},Width,Height/2.0);
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
			lmVector2D Position = {0.0f,0.0f};
			lmVector2D Velocity = {0.0f,0.0f};
			Agents.push_back(new lPlayerAgent(World.CreateBody(Position,Velocity),GameScene->GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0),Input.GetController(0),GameCamera));
		}
		//
		for(unsigned int i=0;i < 5;i++)
		{
			lmVector2D Position = {100.0f + i*80.0f,100.0f};
			lmVector2D Velocity = {0.0f,0.0f};
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

