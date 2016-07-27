#ifndef L_GAME_H
#define L_GAME_H

#include "../lInterfaces/lGame/liConsole.h"
#include "../lInterfaces/lGame/liGame.h"
#include "../lInterfaces/lRenderer/li2DRenderer.h"

class lGame : public liGame
{
protected:
	liConsole &Console;
	liGameMode *GameMode;

	liRenderer &Renderer;

	const float dt = 1.0f/60.0f;

public:

	virtual void Step() override
	{
		GameMode->Logic(dt);
	}

	lGame(liConsole &console,liGameMode *game_mode,liRenderer &renderer)
		:Console(console),GameMode(game_mode),Renderer(renderer)
	{
		//Üres függvénytörzs
	}

	virtual ~lGame() override
	{
		delete GameMode;
	}
};

template<class liWorld_T>
class lSimulationGame : public lGame
{
protected:
	//
	liWorld_T *World;
	//
public:
	//
	virtual void Step() override
	{
		World->Simulate(dt);
		//
		GameMode->Logic(dt);
	}
	//
	lSimulationGame(liConsole &console,liGameMode *game_mode,liRenderer &renderer,liWorld_T *world)
		:lGame(console,game_mode,renderer),World(world)
	{
		//
	}
	//
	virtual ~lSimulationGame() override
	{
		delete World;
	}
};


#endif // L_GAME_H

