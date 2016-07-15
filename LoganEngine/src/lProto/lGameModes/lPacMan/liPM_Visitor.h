#ifndef LI_PM_VISITOR_H
#define LI_PM_VISITOR_H

class liPM_AgentObserver
{
public:
	virtual void RefreshPosition(int x,int y) = 0;

	liPM_AgentObserver()
	{
		//Üres
	}

	virtual ~liPM_AgentObserver()
	{
		//Üres
	}
};

class liPM_Agent
{
public:
	virtual void Subscribe(liPM_AgentObserver *agent_observer) = 0;

	liPM_Agent()
	{
		//Üres
	}

	virtual ~liPM_Agent()
	{
		//Üres
	}
};

class liPM_PacManObserver
{
public:
	virtual void RewardReceived(unsigned int reward) = 0;

	liPM_PacManObserver()
	{
		//Üres
	}

	virtual ~liPM_PacManObserver()
	{
		//Üres
	}
};

class liPM_PacMan
{
public:
	virtual void Subscribe(liPM_PacManObserver *pac_man_observer) = 0;

	liPM_PacMan()
	{
		//Üres
	}

	virtual ~liPM_PacMan()
	{
		//Üres
	}
};

class liPM_CoinObserver
{
public:
	virtual void CoinEaten() = 0;

	liPM_CoinObserver()
	{
		//Üres
	}

	virtual ~liPM_CoinObserver()
	{
		//Üres
	}
};

class liPM_Coin
{
public:

	virtual void Subscribe(liPM_CoinObserver *coin_observer) = 0;

	virtual bool IsEaten() = 0;
	virtual void Eat() = 0;

	liPM_Coin()
	{
		//Üres
	}

	virtual ~liPM_Coin()
	{
		//Üres
	}
};

class liPM_Visitor
{
public:

	virtual void Visit(liPM_Agent *agent) = 0;
	virtual void Visit(liPM_PacMan *pac_man) = 0;
	virtual void Visit(liPM_Coin *coin) = 0;

	liPM_Visitor()
	{
		//Üres
	}

	virtual ~liPM_Visitor()
	{
		//Üres
	}
};

class liPM_Visitable
{
public:

	virtual void Accept(liPM_Visitor *Visitor) = 0;

	liPM_Visitable()
	{
		//Üres
	}

	virtual ~liPM_Visitable()
	{
		//Üres
	}
};

#endif // LI_PM_VISITOR_H
