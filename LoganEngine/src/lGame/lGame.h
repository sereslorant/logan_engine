#ifndef L_GAME_H
#define L_GAME_H

#include "../lInterfaces/lGame/liConsole.h"
#include "../lInterfaces/lGame/liGame.h"

class lGame : public liGame
{
protected:
	liConsole &Console;
	liGameMode *GameMode;

	/*
	 * TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 */
	li2DScene *Scene;
	li2DCamera *Camera;
	/*
	 * TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	 */

	const float dt = 1.0/60.0;

public:

	virtual void Step() override
	{
		GameMode->Logic(dt);
	}

	lGame(liConsole &console,liGameMode *game_mode,li2DScene *scene,li2DCamera *camera)
		:Console(console),GameMode(game_mode),Scene(scene),Camera(camera)
	{
		//Üres függvénytörzs
	}

	virtual ~lGame() override
	{
		delete GameMode;

		delete Scene;
		delete Camera;
	}
};

#endif // L_GAME_H

