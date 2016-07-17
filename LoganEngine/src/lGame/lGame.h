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

	li2DRenderer &Renderer;

	const float dt = 1.0/60.0;

public:

	virtual void Step() override
	{
		GameMode->Logic(dt);
	}

	lGame(liConsole &console,liGameMode *game_mode,li2DRenderer &renderer)
		:Console(console),GameMode(game_mode),Renderer(renderer)
	{
		//Üres függvénytörzs
	}

	virtual ~lGame() override
	{
		delete GameMode;
	}
};

#endif // L_GAME_H

