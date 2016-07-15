#ifndef L_GAME_H
#define L_GAME_H

#include "../lInterfaces/lGame/liConsole.h"
#include "../lInterfaces/lGame/liGame.h"

class lGame : public liGame
{
protected:
	liConsole &Console;
	liGameMode *GameMode;
	li2DScene *Scene;

	const float dt = 1.0/60.0;

public:

	virtual void Step() override
	{
		GameMode->Logic(dt);
	}

	lGame(liConsole &console,liGameMode *game_mode,li2DScene *scene)
		:Console(console),GameMode(game_mode),Scene(scene)
	{
		//Üres függvénytörzs
	}

	virtual ~lGame() override
	{
		delete GameMode;
		delete Scene;
	}
};

/*
 * Ezen még elmélkedni kell egy sort!!!
 */
/*
#include "../lInterfaces/lRenderer/li2DRenderer.h"

class lGame2D : public lGame
{
protected:
	li2DRenderer &Renderer;

public:

	lGame2D(liGameMode &game_mode,liConsole &console,li2DRenderer &renderer)
		:lGame(game_mode,console),Renderer(renderer)
	{
		//Üres függvénytörzs
	}

	virtual ~lGame2D() override
	{
		//Üres függvénytörzs
	}
};
*/

#endif // L_GAME_H

