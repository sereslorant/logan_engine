#ifndef LP2_TEST_GAME_MODE_H
#define LP2_TEST_GAME_MODE_H

#include "../../../lInterfaces/lApiAdapter/liInput.h"

#include "lGame/lP2Game.h"

#include "../../../lUtils/luSetLayer.h"

class lP2_TestGameMode : public lP2ProtoGameMode
{
protected:
	//
	struct lBodyState : public liState2D
	{
		lmVector2D Position;
		lmVector2D Velocity;
		//
		virtual const lmVector2D &GetPosition() const override
		{return Position;}
		//
		virtual const lmVector2D &GetVelocity() const override
		{return Velocity;}
		//
		lBodyState(){}
		virtual ~lBodyState(){}
		/*
		 * End of class
		 */
	};
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
	//
	constexpr static lmScalar RADIUS = 25.0;
	//
	class lAgent : public liAgent
	{
	protected:
		//
		bool Removable = false;
		//
		liBody2D &Body;
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
			Element->SetPosition(Body.GetPosition() - lmVector2D({RADIUS,RADIUS}));
		}
		//
		lAgent(liBody2D &body,li2DElement *element)
			:Body(body),Element(element)
		{
			//
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
			lBodyState State;
			State.Position = Body.GetPosition();
			State.Velocity = NewVelocity;
			//
			Body.SetState(State);

			lAgent::Update(dt);
			Camera->SetPosition(Body.GetPosition() - lmVector2D({Camera->GetWidth()/2.0f,Camera->GetHeight()/2.0f}));
		}
		//
		lPlayerAgent(liBody2D &body,li2DElement *element,liController *controller,li2DCamera *camera)
			:lAgent(body,element),Controller(controller),Camera(camera)
		{}
		//
		virtual ~lPlayerAgent() override
		{}
	};
	//
	liController *Controller = nullptr;
	bool Mode = true;
	//
public:
	//
	virtual void Logic(double dt) override
	{
		if(Controller != nullptr && Controller->GetNumButtons() >= 2)
		{
			if(Controller->GetButton(0)->IsPressed())
			{
				Mode = true;
			}
			if(Controller->GetButton(1)->IsPressed())
			{
				Mode = false;
			}
		}

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
			lP2ProtoGameMode::Logic(dt);
		}
	}
	//
	lP2_TestGameMode(liInput &input,liWorld2D &world,liRenderer &renderer)
		:lP2ProtoGameMode(input,world,renderer)
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
		Controller = Input.GetController(0);
		//
		luSet3DLayer SetTestLayer(nullptr,Frustum3D,Camera3D);
		TestLayer->Accept(SetTestLayer);
		//
		TestLayer->Enable();
		//
		GameScene = new lr2DScene;
		GameScene->SetBackgroundColor(lrColor(1.0,1.0,1.0,1.0));
		GameCamera = new lr2DCamera({0.0f,0.0f},Width,Height/2.0f);
		//
		GuiScene = new lr2DScene;
		GuiCamera = new lr2DCamera({0.0f,0.0f},Width,Height/2.0f);
		//
		luSet2DLayer SetGameLayer(GameScene,GameCamera);
		luSet2DLayer SetGuiLayer(GuiScene,GuiCamera);
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
			Controller = Input.GetController(0);
			//
			lBodyState State;
			State.Position = {0.0f,0.0f};
			State.Velocity = {0.0f,0.0f};
			//
			auto &Body2DBuilder = World.GetBodyFactory().CreateBody();
			Body2DBuilder.SetState(State);
			auto &Circle2DBuilder = Body2DBuilder.CreateCircle();
			Circle2DBuilder.SetRadius(RADIUS);
			Circle2DBuilder.Construct();
			//
			liBody2D &Body = Body2DBuilder.Construct();
			Agents.push_back(new lPlayerAgent(Body,GameScene->GetElementFactory().CreateRectangle(State.Position,RADIUS*2.0,RADIUS*2.0),Controller,GameCamera));
		}
		//
		for(unsigned int i=0;i < 5;i++)
		{
			lBodyState State;
			State.Position = {100.0f + i*80.0f,100.0f};
			State.Velocity = {0.0f,0.0f};
			//
			auto &Body2DBuilder = World.GetBodyFactory().CreateBody();
			Body2DBuilder.SetState(State);
			auto &Circle2DBuilder = Body2DBuilder.CreateCircle();
			Circle2DBuilder.SetRadius(RADIUS);
			Circle2DBuilder.Construct();
			//
			liBody2D &Body = Body2DBuilder.Construct();
			Agents.push_back(new lAgent(Body,GameScene->GetElementFactory().CreateRectangle(State.Position,RADIUS*2.0,RADIUS*2.0)));
		}
	}
	//
	virtual ~lP2_TestGameMode() override
	{
		delete GameScene;
		delete GameCamera;
		//
		delete GuiScene;
		delete GuiCamera;
		//
		delete Frustum3D;
		delete Camera3D;
	}
};

#endif // LP2_TEST_GAME_MODE_H

