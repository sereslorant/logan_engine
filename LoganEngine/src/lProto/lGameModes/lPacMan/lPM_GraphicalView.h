#ifndef LPM_GRAPHICAL_VIEW_H
#define LPM_GRAPHICAL_VIEW_H

#include "lPM_Model.h"

#include "../../../lRenderer/lr2DScene/lr2DScene.h"
#include "../../../lInterfaces/lRenderer/li2DRenderer.h"

#include "../../../lMath/lMath.h"
#include "../../../lRenderer/lrColor.h"

class lPM_GraphicalView : public liPM_ModelObserver
{
private:

	static constexpr float dS = 40.0;
	static constexpr float CoinSize = 20.0;


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

	class lPM_AgentView : public liPM_AgentObserver
	{
	protected:
		int X = 0;
		int Y = 0;

		li2DElement *Drawable;
	public:

		virtual void RefreshPosition(int x,int y)
		{
			if((X == x) && (Y == y))
			{
				return;
			}

			X = x;
			Y = y;

			lmVector2D NewPosition = {X*dS + dS/2.0,Y*dS + dS/2.0};
			Drawable->SetPosition(NewPosition);
		}


		lPM_AgentView(li2DElement *drawable):Drawable(drawable)
		{
			//Üres
		}

		virtual ~lPM_AgentView() override
		{
			//Üres
		}
	};

	class lPM_CoinView : public lPM_AgentView, public liPM_CoinObserver
	{
	public:

		virtual void CoinEaten() override
		{
			Drawable->Hide();
		}

		lPM_CoinView(li2DElement *drawable):lPM_AgentView(drawable)
		{
			Drawable->SetColor(lrColor(1.0,1.0,0.5,1.0));
		}

		virtual ~lPM_CoinView() override {}
	};

	class lPM_PacManView : public lPM_AgentView, public liPM_PacManObserver
	{
	public:

		virtual void RewardReceived(unsigned int reward) override
		{
			//Itt tudnánk értesülni arról, hogy a PacMan-ünk megzabált egy pínzt
		}

		lPM_PacManView(li2DElement *drawable):lPM_AgentView(drawable)
		{
			Drawable->SetColor(lrColor(1.0,1.0,0.0,1.0));
		}

		virtual ~lPM_PacManView() override
		{
			//Üres
		}
	};

	class lPM_AgentSubscriber : public liPM_Visitor
	{
	private:
		lPM_GraphicalView &GraphicalView;
		li2DScene::liElementFactory &ElementFactory;

		lPM_AgentView *NewObserver = nullptr;

	public:

		virtual void Visit(liPM_Agent *agent) override
		{
			if(NewObserver == nullptr)
			{
				lmVector2D NewPosition = {0*dS + dS/2.0,0*dS + dS/2.0};
				NewObserver = new lPM_AgentView(ElementFactory.CreateRectangle(NewPosition,dS,dS));
			}
			//
			agent->Subscribe(NewObserver);
			GraphicalView.AgentViews.push_back(NewObserver);
		}

		virtual void Visit(liPM_PacMan *pac_man) override
		{
			lmVector2D NewPosition = {0*dS + dS/2.0,0*dS + dS/2.0};
			lPM_PacManView *PacManView = new lPM_PacManView(ElementFactory.CreateRectangle(NewPosition,dS,dS));
			//
			pac_man->Subscribe(PacManView);
			NewObserver = PacManView;
		}

		virtual void Visit(liPM_Coin *coin) override
		{
			lmVector2D NewPosition = {0*dS + dS/2.0,0*dS + dS/2.0};
			lPM_CoinView *CoinView = new lPM_CoinView(ElementFactory.CreateRectangle(NewPosition,CoinSize,CoinSize));
			//
			coin->Subscribe(CoinView);
			NewObserver = CoinView;
		}

		lPM_AgentSubscriber(lPM_GraphicalView &graphical_view,li2DScene::liElementFactory &element_factory)
			:GraphicalView(graphical_view),ElementFactory(element_factory)
		{
			//Üres
		}

		virtual ~lPM_AgentSubscriber() override
		{
			//Üres
		}
	};

	std::list<lPM_AgentView *> AgentViews;
	//
	liRenderer &Renderer;
	//
	lr2DScene *Scene;
	lr2DCamera *Camera;
	//
	liViewport *Viewport;
	liLayer *Layer;
public:

	virtual void Subscribe(lPM_Agent *agent) override
	{
		lPM_AgentSubscriber AgentSubscriber(*this,Scene->GetElementFactory());
		//
		agent->Accept(&AgentSubscriber);
	}

	virtual void LoopEnded() override
	{
		//Itt tudnánk értesülni arról, hogy a ciklus véget ért.
	}

	lPM_GraphicalView(liRenderer &renderer)
		:Renderer(renderer)
	{
		unsigned int Width = Renderer.GetMainFramebuffer().GetWidth();
		unsigned int Height = Renderer.GetMainFramebuffer().GetHeight();
		//
		Viewport = Renderer.GetMainFramebuffer().CreateViewport(0,0,Width,Height);
		//
		Scene = new lr2DScene;
		Scene->SetBackgroundColor(lrColor(1.0,1.0,1.0,1.0));
		Camera = new lr2DCamera({0.0,0.0},Width,Height);
		//
		Set2DLayer SetLayer(Scene,Camera);
		//
		Layer = Viewport->Create2DLayer();
		//
		Layer->Accept(SetLayer);
		//
		Layer->Enable();
	}

	virtual ~lPM_GraphicalView()
	{
		for(lPM_AgentView *AgentView : AgentViews)
		{
			delete AgentView;
		}
	}
};


#endif // LPM_GRAPHICAL_VIEW_H
