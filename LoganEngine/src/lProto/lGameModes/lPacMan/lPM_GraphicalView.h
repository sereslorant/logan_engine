#ifndef LPM_GRAPHICAL_VIEW_H
#define LPM_GRAPHICAL_VIEW_H

#include "lPM_Model.h"
#include "../../../lRenderer/lrColor.h"

class lPM_GraphicalView : public liPM_ModelObserver
{
private:

	static constexpr float dS = 40.0;

	class lPM_AgentView : public liPM_AgentObserver
	{
	protected:
		unsigned int X;
		unsigned int Y;

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

			Drawable->SetPosition({X*dS + dS/2.0,Y*dS + dS/2.0});
		}


		lPM_AgentView(li2DElement *drawable):Drawable(drawable)
		{}

		virtual ~lPM_AgentView() override {}
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
			//std::cout << "Reward: " << reward << std::endl;
		}

		lPM_PacManView(li2DElement *drawable):lPM_AgentView(drawable)
		{
			Drawable->SetColor(lrColor(1.0,1.0,0.0,1.0));
		}

		virtual ~lPM_PacManView() override {}
	};

	class lPM_AgentSubscriber : public liPM_Visitor
	{
	private:
		lPM_GraphicalView &GraphicalView;

		lPM_AgentView *NewObserver = nullptr;

	public:

		virtual void Visit(liPM_Agent *agent) override
		{
			if(NewObserver == nullptr)
			{
				NewObserver = new lPM_AgentView(GraphicalView.ElementFactory.CreateRectangle({0*dS + dS/2.0,0*dS + dS/2.0},dS,dS));
			}
			//
			agent->Subscribe(NewObserver);
			GraphicalView.AgentViews.push_back(NewObserver);
		}

		virtual void Visit(liPM_PacMan *pac_man) override
		{
			lPM_PacManView *PacManView = new lPM_PacManView(GraphicalView.ElementFactory.CreateRectangle({0*dS + dS/2.0,0*dS + dS/2.0},dS,dS));
			//
			pac_man->Subscribe(PacManView);
			NewObserver = PacManView;
		}

		virtual void Visit(liPM_Coin *coin) override
		{
			lPM_CoinView *CoinView = new lPM_CoinView(GraphicalView.ElementFactory.CreateRectangle({0*dS + dS/2.0,0*dS + dS/2.0},dS,dS));
			//
			coin->Subscribe(CoinView);
			NewObserver = CoinView;
		}

		lPM_AgentSubscriber(lPM_GraphicalView &graphical_view)
			:GraphicalView(graphical_view)
		{}

		virtual ~lPM_AgentSubscriber() override
		{}
	};

	std::list<lPM_AgentView *> AgentViews;
	li2DScene::liElementFactory &ElementFactory;
	//li2DRenderer::liDrawableFactory &DrawableFactory;

public:

	virtual void Subscribe(lPM_Agent *agent) override
	{
		lPM_AgentSubscriber AgentSubscriber(*this);
		//
		agent->Accept(&AgentSubscriber);
	}

	virtual void LoopEnded() override
	{
		//std::cout << "Loop ended" << std::endl;
	}

	lPM_GraphicalView(li2DScene::liElementFactory &element_factory)
		:ElementFactory(element_factory)
	{}

	virtual ~lPM_GraphicalView()
	{
		for(lPM_AgentView *AgentView : AgentViews)
		{
			delete AgentView;
		}
	}
};


#endif // LPM_GRAPHICAL_VIEW_H
