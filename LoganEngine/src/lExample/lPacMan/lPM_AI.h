#ifndef LPM_AI_H
#define LPM_AI_H

#include "lPM_Controller/liPM_AgentController.h"
#include "lPM_Model/lPM_Model.h"

#include <iostream>
#include <vector>


// just a test
enum lPM_AI_Cell
{
	URES,
	WALL,
	PINZ,
};

struct lPM_State
{
	std::vector<std::vector<lPM_AI_Cell> > MapState;
	unsigned int PacManX;
	unsigned int PacManY;

	void Print()
	{
		for(unsigned int i=0;i < MapState.size();i++)
		{
			for(unsigned int j=0;j < MapState[i].size();j++)
			{
				if((i == PacManX) && (j == PacManY))
				{
					std::cout << "@";
				}
				else
				{
					if(MapState[i][j] == WALL)
					{std::cout << "#";}
					if(MapState[i][j] == URES)
					{std::cout << " ";}
					if(MapState[i][j] == PINZ)
					{std::cout << "$";}
				}
			}
			std::cout << std::endl;
		}
	}

	void SetSize(unsigned int width,unsigned int height)
	{
		MapState.resize(width);
		for(auto &I : MapState)
		{
			I.resize(height);

			for(auto &E : I)
			{
				E = URES;
			}
		}
	}
};

class lPM_AI_Percept : public liPM_ModelObserver
{
private:
	//
	class WallObserver : public liPM_AgentObserver
	{
	protected:
		lPM_State &State;
		unsigned int X = 0;
		unsigned int Y = 0;
		//
	public:
		virtual void WriteState()
		{
			State.MapState[X][Y] = WALL;
		}
		//
		virtual void RefreshPosition(int x,int y) override
		{
			std::cout << x << ";" << y << std::endl;
			X = x;
			Y = y;
		}
		//
		void SetAgent(liPM_Agent *agent)
		{
			X = agent->GetX();
			Y = agent->GetY();
		}
		//
		WallObserver(lPM_State &state)
			:State(state)
		{
			//
		}
		virtual ~WallObserver() override {}
	};
	//
	class PacManObserver : public WallObserver,public liPM_PacManObserver
	{
	public:
		virtual void WriteState() override
		{
			State.PacManX = X;
			State.PacManY = Y;
		}
		//
		virtual void RewardReceived(unsigned int reward) override
		{
			std::cout << reward << std::endl;
		}
		//
		PacManObserver(lPM_State &state)
			:WallObserver(state)
		{}
		//
		virtual ~PacManObserver(){}
	};
	//
	class CoinObserver : public WallObserver,public liPM_CoinObserver
	{
	private:
		bool Eaten = false;
		//
	public:
		virtual void WriteState() override
		{
			if(Eaten)
				{State.MapState[X][Y] = URES;}
			else
				{State.MapState[X][Y] = PINZ;}
		}
		//
		virtual void CoinEaten() override
		{
			std::cout << "CoinEaten" << std::endl;
			Eaten = true;
		}
		//
		CoinObserver(lPM_State &state)
			:WallObserver(state)
		{}
		//
		virtual ~CoinObserver(){}
	};
	//
	std::list<WallObserver *> Observers;
	//
	class PerceptVisitor : public liPM_Visitor
	{
	private:
		std::list<WallObserver *> &Observers;
		lPM_State &State;
		//
		WallObserver *NewObserver = nullptr;
	public:
		//
		virtual void Visit(liPM_Agent *agent) override
		{
			if(NewObserver == nullptr)
			{
				NewObserver = new WallObserver(State);
				Observers.push_back(NewObserver);
			}

			agent->Subscribe(NewObserver);
			NewObserver->SetAgent(agent);
		}
		//
		virtual void Visit(liPM_PacMan *pac_man) override
		{
			PacManObserver *NewPmObserver = new PacManObserver(State);
			Observers.push_back(NewPmObserver);
			//
			pac_man->Subscribe(NewPmObserver);
			NewObserver = NewPmObserver;
		}
		//
		virtual void Visit(liPM_Coin *coin) override
		{
			CoinObserver *NewCoinObserver = new CoinObserver(State);
			Observers.push_back(NewCoinObserver);
			//
			coin->Subscribe(NewCoinObserver);
			NewObserver = NewCoinObserver;
		}
		//
		PerceptVisitor(std::list<WallObserver *> &observers,lPM_State &state)
			:Observers(observers),State(state)
		{}
		//
		virtual ~PerceptVisitor()
		{}
	};
	lPM_State State;
	//
public:
	//
	void RefreshState()
	{
		for(WallObserver *Observer : Observers)
		{
			Observer->WriteState();
		}
		//
		State.Print();
	}
	//
	virtual void Subscribe(lPM_Agent *agent) override
	{
		PerceptVisitor perceptVisitor(Observers,State);
		agent->Accept(&perceptVisitor);
	}
	//
	virtual void SetDimensions(unsigned int width,unsigned int height) override
	{
		std::cout << "Width = " << width << " Height = " << height << std::endl;
		State.SetSize(width,height);
		//
		RefreshState();
	}
	//
	virtual void LoopEnded() override
	{
		std::cout << "Loop" << std::endl;
	}
	//
	lPM_AI_Percept()
	{
		//Üres
	}
	//
	virtual ~lPM_AI_Percept() override
	{
		//Üres
		for(auto I : Observers)
		{
			delete I;
		}
	}
	//
};


class lPM_AI_Controller : public liPM_AgentController
{
private:
	enum AI_DIRECTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

	AI_DIRECTION Direction = RIGHT;
	lPM_AI_Percept *Percept = nullptr;

public:

	void SetPercept(lPM_AI_Percept *percept)
	{Percept = percept;}

	virtual void Think() override
	{
		if(Percept != nullptr)
		{Percept->RefreshState();}

		switch(Direction)
		{
		case UP:
			Direction = LEFT;
			break;
		case LEFT:
			Direction = DOWN;
			break;
		case DOWN:
			Direction = RIGHT;
			break;
		case RIGHT:
			Direction = UP;
			break;
		}
	}

	virtual bool GetUp() override
	{
		return Direction == UP;
	}

	virtual bool GetDown() override
	{
		return Direction == DOWN;
	}

	virtual bool GetLeft() override
	{
		return Direction == LEFT;
	}

	virtual bool GetRight() override
	{
		return Direction == RIGHT;
	}

	lPM_AI_Controller(){}
	virtual ~lPM_AI_Controller() override {}
};

#endif // LPM_AI_H

