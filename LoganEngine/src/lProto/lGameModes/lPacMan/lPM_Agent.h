#ifndef LPM_AGENT_H
#define LPM_AGENT_H

#include "liPM_Visitor.h"

#include <list>

class lPM_Agent : public liPM_Visitable, public liPM_Visitor, public liPM_Agent
{
protected:
	int X;
	int Y;

	int dX = 0;
	int dY = 0;

	bool PassThrough;

	std::list<liPM_AgentObserver *> AgentObservers;

	/*
	 * IDEIGLENES MEGOLDÁS
	 */
	//static constexpr float dS = 40.0;
	//liView::liDrawable *Drawable;

public:

	int GetX()
	{
		return X;
	}

	int GetY()
	{
		return Y;
	}

	int GetNextX()
	{
		return X + dX;
	}

	int GetNextY()
	{
		return Y + dY;
	}

	bool IsPassThrough()
	{
		return PassThrough;
	}

	virtual void Update()
	{
		//Üres
	}

	virtual void Step()
	{
		X += dX;
		Y += dY;

		//Drawable->SetPosition(X*dS + dS/2.0,Y*dS + dS/2.0);

		for(liPM_AgentObserver *AgentObserver : AgentObservers)
		{
			AgentObserver->RefreshPosition(X,Y);
		}
	}

	virtual void Subscribe(liPM_AgentObserver *agent_observer) override
	{
		AgentObservers.push_back(agent_observer);
	}

	virtual void Visit(liPM_Agent *agent) override
	{}

	virtual void Visit(liPM_Coin *coin) override
	{}

	virtual void Visit(liPM_PacMan *pac_man) override
	{}

	virtual void Accept(liPM_Visitor *visitor) override
	{
		visitor->Visit(this);
	}

	lPM_Agent(int x,int y,bool pass_through/*,liView::liDrawable *drawable*/)
		:X(x),Y(y),PassThrough(pass_through)//,Drawable(drawable)
	{}

	virtual ~lPM_Agent()
	{}
};

#endif // LPM_AGENT_H
