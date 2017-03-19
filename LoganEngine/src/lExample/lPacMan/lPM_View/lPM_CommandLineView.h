#ifndef LPM_COMMAND_LINE_VIEW_H
#define LPM_COMMAND_LINE_VIEW_H

#include "../lPM_Model/lPM_Model.h"

#include <iostream>

class lPM_CommandLineView : public liPM_ModelObserver
{
private:

	class lPM_AgentView : public liPM_AgentObserver
	{
	private:
		unsigned int X;
		unsigned int Y;

	public:

		virtual void RefreshPosition(int x,int y) override
		{
			if((X == x) && (Y == y))
			{
				return;
			}

			X = x;
			Y = y;

			std::cout << "Position changed: " << X << " " << Y << std::endl;
		}

		lPM_AgentView(){}
		virtual ~lPM_AgentView() override {}
	};

	class lPM_CoinView : public lPM_AgentView, public liPM_CoinObserver
	{
	public:

		virtual void CoinEaten() override
		{
			std::cout << "Coin eaten" << std::endl;
		}

		lPM_CoinView(){}
		virtual ~lPM_CoinView() override {}
	};

	class lPM_PacManView : public lPM_AgentView, public liPM_PacManObserver
	{
	public:

		virtual void RewardReceived(unsigned int reward) override
		{
			std::cout << "Reward: " << reward << std::endl;
		}

		lPM_PacManView(){}
		virtual ~lPM_PacManView() override {}
	};

	class lPM_AgentSubscriber : public liPM_Visitor
	{
	private:
		lPM_CommandLineView &CommandLineView;

		lPM_AgentView *NewObserver = nullptr;

	public:

		virtual void Visit(liPM_Agent *agent) override
		{
			if(NewObserver == nullptr)
			{
				NewObserver = new lPM_AgentView;
			}
			//
			agent->Subscribe(NewObserver);
			CommandLineView.AgentViews.push_back(NewObserver);
		}

		virtual void Visit(liPM_PacMan *pac_man) override
		{
			lPM_PacManView *PacManView = new lPM_PacManView;
			//
			pac_man->Subscribe(PacManView);
			NewObserver = PacManView;
		}

		virtual void Visit(liPM_Coin *coin) override
		{
			lPM_CoinView *CoinView = new lPM_CoinView;
			//
			coin->Subscribe(CoinView);
			NewObserver = CoinView;
		}

		lPM_AgentSubscriber(lPM_CommandLineView &command_line_view)
			:CommandLineView(command_line_view)
		{}

		virtual ~lPM_AgentSubscriber() override
		{}
	};

	std::list<lPM_AgentView *> AgentViews;

public:

	virtual void Subscribe(lPM_Agent *agent) override
	{
		lPM_AgentSubscriber AgentSubscriber(*this);
		//
		agent->Accept(&AgentSubscriber);
	}
	
	virtual void SetDimensions(unsigned int width,unsigned int height) override
	{
		//
	}

	virtual void LoopEnded() override
	{
		std::cout << "Loop ended" << std::endl;
	}

	lPM_CommandLineView()
	{}

	virtual ~lPM_CommandLineView()
	{
		for(lPM_AgentView *AgentView : AgentViews)
		{
			delete AgentView;
		}
	}
};

#endif // LPM_COMMAND_LINE_VIEW_H
