#ifndef LI_GAME_H
#define LI_GAME_H

class liGameMode
{
public:
	virtual void Logic(double dt) = 0;

	liGameMode(){}
	virtual ~liGameMode(){}
};

class liGame
{
public:
	//virtual void Stop() = 0;
	virtual void Step() = 0;

	liGame(){}
	virtual ~liGame(){}
};

#endif // LI_GAME_H
