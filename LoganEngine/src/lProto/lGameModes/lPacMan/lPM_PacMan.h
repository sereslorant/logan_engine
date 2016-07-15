#ifndef LPM_PAC_MAN_H
#define LPM_PAC_MAN_H

#include "lPM_Agent.h"

#include "liPM_AgentController.h"

class lPM_PacMan : public lPM_Agent, public liPM_PacMan
{
protected:

	unsigned int Reward = 0;
	liPM_AgentController *AgentControl;

	std::list<liPM_PacManObserver *> PacManObservers;

public:

	virtual void Visit(liPM_Coin *coin) override
	{
		if(!coin->IsEaten())
		{
			coin->Eat();
			Reward++;

			for(liPM_PacManObserver *Observer : PacManObservers)
			{
				Observer->RewardReceived(Reward);
			}
		}
	}

	virtual void Accept(liPM_Visitor *visitor) override
	{
		visitor->Visit((liPM_PacMan *)this);
		lPM_Agent::Accept(visitor);
	}

	virtual void Subscribe(liPM_PacManObserver *pac_man_observer) override
	{
		PacManObservers.push_back(pac_man_observer);
		//
		pac_man_observer->RewardReceived(Reward);
	}

	virtual void Update() override
	{
		dX = 0;
		dY = 0;

		if(AgentControl != nullptr)
		{
			AgentControl->Think();

			if(AgentControl->GetUp())
			{dY -= 1;}
			if(AgentControl->GetDown())
			{dY += 1;}
			if(AgentControl->GetRight())
			{dX += 1;}
			if(AgentControl->GetLeft())
			{dX -= 1;}
		}
	}

	lPM_PacMan(int x,int y,liPM_AgentController *agent_control = nullptr)
		:lPM_Agent(x,y,false),AgentControl(agent_control)
	{
		//Üres
	}

	virtual ~lPM_PacMan() override
	{
		//Üres
	}
};

#endif // LPM_PAC_MAN_H
