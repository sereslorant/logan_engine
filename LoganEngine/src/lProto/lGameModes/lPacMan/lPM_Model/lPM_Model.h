#ifndef LPM_MODEL_H
#define LPM_MODEL_H

#include "lPM_Agents/lPM_Agent.h"
#include "lPM_Agents/lPM_PacMan.h"
#include "lPM_Agents/lPM_Coin.h"

class liPM_AgentFactory
{
public:
	virtual void AddAgent(unsigned int x,unsigned int y,bool pass_through) = 0;
	virtual void AddPacMan(unsigned int x,unsigned int y,liPM_AgentController *agent_control) = 0;
	virtual void AddCoin(unsigned int x,unsigned int y) = 0;

	liPM_AgentFactory()
	{
		//Üres
	}

	virtual ~liPM_AgentFactory()
	{
		//Üres
	}
};

class liPM_ModelObserver
{
public:
	virtual void Subscribe(lPM_Agent *agent) = 0;
	virtual void SetDimensions(unsigned int width,unsigned int height) = 0;

	virtual void LoopEnded() = 0;

	liPM_ModelObserver()
	{
		//Üres
	}

	virtual ~liPM_ModelObserver()
	{
		//Üres
	}
};

#include <list>

class lPM_Model
{
private:

	/*
	 * Belső classok
	 */

	class lPM_AgentFactory : public liPM_AgentFactory
	{
	private:
		lPM_Model &Model;

	public:
		virtual void AddAgent(unsigned int x,unsigned int y,bool pass_through) override
		{
			lPM_Agent *Agent = new lPM_Agent(x,y,pass_through);
			Model.Agents.push_back(Agent);
			//
			for(liPM_ModelObserver *Observer : Model.Observers)
			{
				Observer->Subscribe(Agent);
			}
		}

		virtual void AddPacMan(unsigned int x,unsigned int y,liPM_AgentController *agent_control) override
		{
			lPM_PacMan *PacMan = new lPM_PacMan(x,y,agent_control);
			Model.Agents.push_back(PacMan);
			//
			for(liPM_ModelObserver *Observer : Model.Observers)
			{
				Observer->Subscribe(PacMan);
			}
		}

		virtual void AddCoin(unsigned int x,unsigned int y) override
		{
			lPM_Coin *Coin = new lPM_Coin(x,y);
			Model.Agents.push_back(Coin);
			//
			for(liPM_ModelObserver *Observer : Model.Observers)
			{
				Observer->Subscribe(Coin);
			}
		}

		lPM_AgentFactory(lPM_Model &model)
			:Model(model)
		{
			//Üres
		}

		~lPM_AgentFactory()
		{
			//Üres
		}
	};

	/*
	 * Attribútumok
	 */
	unsigned int Width = 0;
	unsigned int Height = 0;

	std::list<lPM_Agent *> Agents;
	lPM_AgentFactory AgentFactory;

	std::list<liPM_ModelObserver *> Observers;

	/*
	 * Metódusok
	 */

	void AddAgent(lPM_Agent *agent)
	{
		Agents.push_back(agent);
	}

public:

	void SetDimensions(unsigned int width,unsigned int height)
	{
		Width = width;
		Height = height;
		//
		for(liPM_ModelObserver *Observer : Observers)
		{
			Observer->SetDimensions(Width,Height);
		}
	}

	liPM_AgentFactory *GetAgentFactory()
	{
		return &AgentFactory;
	}

	void Subscribe(liPM_ModelObserver *observer)
	{
		Observers.push_back(observer);
		observer->SetDimensions(Width,Height);

		for(lPM_Agent *Agent : Agents)
		{
			observer->Subscribe(Agent);
		}
	}

	void Step()
	{
		for(lPM_Agent *I : Agents)
		{
			I->Update();

			bool WillCollide = false;
			for(lPM_Agent *J : Agents)
			{
				if(I != J)
				{
					if(I->GetNextX() == J->GetNextX() && I->GetNextY() == J->GetNextY())
					{
						if(!I->IsPassThrough() && !J->IsPassThrough())
							{WillCollide = true;}
					}

					if(I->GetX() == J->GetX() && I->GetY() == J->GetY())
					{
						I->Accept(J);
					}
				}
			}

			if(!WillCollide)
				{I->Step();}
		}

		for(liPM_ModelObserver *Observer : Observers)
		{
			Observer->LoopEnded();
		}
	}

	lPM_Model()
		:AgentFactory(*this)
	{
		//Üres
	}

	~lPM_Model()
	{
		for(lPM_Agent *Agent : Agents)
		{
			delete Agent;
		}
	}
};

#endif // LPM_MODEL_H
