#ifndef LPM_COIN_H
#define LPM_COIN_H

#include "lPM_Agent.h"

class lPM_Coin : public lPM_Agent, public liPM_Coin
{
protected:
	bool Eaten = false;

	std::list<liPM_CoinObserver *> CoinObservers;

public:

	virtual bool IsEaten() override
	{
		return Eaten;
	}

	virtual void Eat() override
	{
		Eaten = true;

		for(liPM_CoinObserver *Observer : CoinObservers)
		{
			Observer->CoinEaten();
		}

		//Drawable->Hide();
	}

	virtual void Subscribe(liPM_CoinObserver *coin_observer) override
	{
		CoinObservers.push_back(coin_observer);
	}

	virtual void Accept(liPM_Visitor *visitor) override
	{
		visitor->Visit((liPM_Coin *)this);
		lPM_Agent::Accept(visitor);
	}

	lPM_Coin(int x,int y/*,liView::liDrawable *drawable*/)
		:lPM_Agent(x,y,true/*,drawable*/)
	{
		//Drawable->SetColor(1.0,1.0,0.5,1.0);
	}

	virtual ~lPM_Coin() override
	{}
};

#endif // LPM_COIN_H
