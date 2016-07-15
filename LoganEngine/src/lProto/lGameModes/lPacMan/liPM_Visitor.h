#ifndef LI_PM_VISITOR_H
#define LI_PM_VISITOR_H

class liPM_AgentObserver
{
public:
	virtual void RefreshPosition(int x,int y) = 0;

	liPM_AgentObserver() {}
	virtual ~liPM_AgentObserver() {}
};

class liPM_Agent
{
public:
	virtual void Subscribe(liPM_AgentObserver *agent_observer) = 0;

	liPM_Agent()
	{}

	virtual ~liPM_Agent()
	{}
};

class liPM_PacManObserver
{
public:
	virtual void RewardReceived(unsigned int reward) = 0;

	liPM_PacManObserver(){}
	virtual ~liPM_PacManObserver(){}
};

class liPM_PacMan
{
public:
	virtual void Subscribe(liPM_PacManObserver *pac_man_observer) = 0;

	liPM_PacMan()
	{}

	virtual ~liPM_PacMan()
	{}
};

class liPM_CoinObserver
{
public:
	virtual void CoinEaten() = 0;

	liPM_CoinObserver(){}
	virtual ~liPM_CoinObserver(){}
};

class liPM_Coin
{
public:

	virtual void Subscribe(liPM_CoinObserver *coin_observer) = 0;

	virtual bool IsEaten() = 0;
	virtual void Eat() = 0;

	liPM_Coin()
	{}

	virtual ~liPM_Coin()
	{}
};

class liPM_Visitor
{
public:

	virtual void Visit(liPM_Agent *agent) = 0;
	virtual void Visit(liPM_PacMan *pac_man) = 0;
	virtual void Visit(liPM_Coin *coin) = 0;

	liPM_Visitor()
	{}

	virtual ~liPM_Visitor()
	{}
};

class liPM_Visitable
{
public:

	virtual void Accept(liPM_Visitor *Visitor) = 0;

	liPM_Visitable()
	{}

	virtual ~liPM_Visitable()
	{}
};

#endif // LI_PM_VISITOR_H
