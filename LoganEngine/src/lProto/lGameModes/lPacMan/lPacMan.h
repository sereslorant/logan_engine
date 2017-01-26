#ifndef L_PACMAN_H
#define L_PACMAN_H

/*
class lPM_CommandLineView : public liPM_ModelObserver
{
private:

	class lPM_AgentView : public liPM_AgentObserver
	{
	private:
		unsigned int X;
		unsigned int Y;

	public:

		virtual void RefreshPosition(int x,int y)
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
*/

#include "lPM_Model/lPM_Model.h"
#include "lPM_JSONLoader.h"

#include "lPM_Controller/lPM_PacManJoystickController.h"
#include "lPM_View/lPM_GraphicalView.h"

#include "lPM_AI.h"

#include <sstream>

#include "../../../lInterfaces/lGame/liGame.h"
#include "../../../lGame/lTimer.h"
#include "../../../lInterfaces/lApiAdapter/liApiAdapter.h"
#include "../../../lInterfaces/lRenderer/liRenderer.h"

class lPM_Game : public liGameMode
{
protected:

	class lPacManStepCallback : public liEventCallback
	{
	private:
		lPM_Model &Model;

	public:
		virtual void Callback() override
		{
			Model.Step();
		}

		lPacManStepCallback(lPM_Model &model)
			:Model(model)
		{}

		virtual ~lPacManStepCallback() override
		{}
	};
	//
	float dS = 40.0;
	//
	liTimer *Timer;
	//
	lPacManStepCallback *PacManStepCallback;
	liPM_AgentController *AgentController = nullptr;
	lPM_AI_Percept *AI_Percept = nullptr;
	//
	lPM_Model Model;
	//
	//lPM_CommandLineView CommandLineView;
	lPM_GraphicalView GraphicalView;

public:

	virtual void Logic(double dt) override
	{
		Timer->Update(dt);
	}

	lPM_Game(liInput &input,liRenderer &renderer,double time_step)
		:GraphicalView(renderer)
	{
		//
		Timer = new lTimer(time_step,true);
		Timer->Enable();
		//
		PacManStepCallback = new lPacManStepCallback(Model);
		Timer->GetLimitReachedEvent()->AddCallback(PacManStepCallback);
		//
		if(input.GetNumControllers() > 0)
		{
			liController *Controller = input.GetController(0);
			AgentController = new lPM_PacManJoystickController(Controller);
		}
		else
		{
			AI_Percept = new lPM_AI_Percept;
			Model.Subscribe(AI_Percept);
			//
			lPM_AI_Controller *Ctrl = new lPM_AI_Controller;
			Ctrl->SetPercept(AI_Percept);
			AgentController = Ctrl;
		}
		//
		const char *Map0 =
		R"(
		[["e",    "sc_ul","sh","sc_ur","e"    ],
		 ["sc_ul","sc_lr","p", "sc_ll","sc_ur"],
		 ["sv",   "f",    "f", "f",    "sv"   ],
		 ["sv",   "f",    "dv","f",    "sv"   ],
		 ["sv",   "f",    "f", "f",    "sv"   ],
		 ["sc_ll","sc_ur","f", "sc_ul","sc_lr"],
		 ["e",    "sc_ll","sh","sc_lr","e"    ]]
		)";
		//
		const char *Map1 = R"(
		[["sc_ul","sh","sh",   "sh","sh","hc_ur","sh","sh","sh",   "sh","sc_ur"],
		 ["sv",   "f", "f",    "f", "f", "dv",   "f", "f", "f",    "f", "sv"   ],
		 ["sv",   "f", "dc_ul","dh","f", "dv",   "f", "dh","dc_ur","f", "sv"   ],
		 ["sv",   "f", "dv",   "f", "f", "f",    "f", "f", "dv",   "f", "sv"   ],
		 ["sv",   "f", "dv",   "f", "dh","dh",   "dh","f", "dv",   "f", "sv"   ],
		 ["sv",   "f", "f",    "f", "f", "f",    "f", "f", "f",    "f", "sv"   ],
		 ["sv",   "f", "dv",   "f", "dh","dh",   "dh","f", "dv",   "f", "sv"   ],
		 ["sv",   "f", "dv",   "f", "f", "f",    "f", "f", "dv",   "f", "sv"   ],
		 ["sv",   "f", "dc_ll","dh","f", "p",    "f", "dh","dc_lr","f", "sv"   ],
		 ["sv",   "f", "f",    "f", "f", "dv",   "f", "f", "f",    "f", "sv"   ],
		 ["sc_ll","sh","sh",   "sh","sh","hc_ll","sh","sh","sh",   "sh","sc_lr"]]
		 )";
		//
		std::stringstream MapStream(Map1);
		//
		liJSON_Value *Map;
		lJSON_Util::Parse(MapStream,Map);
		//
		lPM_JSONLoader MapLoader(Model.GetAgentFactory(),AgentController);
		Map->Accept(MapLoader);
		//
		Model.SetDimensions(MapLoader.GetWidth(),MapLoader.GetHeight());
		//
		//Model.Subscribe(&CommandLineView);
		Model.Subscribe(&GraphicalView);
		//
		delete Map;
	}

	virtual ~lPM_Game() override
	{
		delete Timer;
		delete PacManStepCallback;

		delete AgentController;
		delete AI_Percept;
	}
};

#endif // L_PACMAN_H
