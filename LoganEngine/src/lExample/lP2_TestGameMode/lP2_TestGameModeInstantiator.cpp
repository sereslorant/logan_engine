
#include "lP2_TestGameModeInstantiator.h"

#include "lP2_TestGameMode.h"
#include "lGame/lP2World2D.h"

#include <lGame/lGame.h>

liGame *lP2_TestGameInstantiator::CreateGame(liApiAdapter &api_adapter,liResourceManager &resource_manager,liRenderer &renderer,liConsole &console)
{
	liWorld2D *World = new lP2World2D;
	liGameMode *GameMode = new lP2_TestGameMode(api_adapter.GetInput(),*World,renderer);
	liGame *Game = new lSimulationGame<liWorld2D>(console,GameMode,renderer,World);
	//
	return Game;
}

liGameInstantiator *CreateGameInstantiator()
{
    return new lP2_TestGameInstantiator;
}
