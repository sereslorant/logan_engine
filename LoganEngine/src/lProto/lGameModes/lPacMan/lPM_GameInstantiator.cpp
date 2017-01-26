
#include "lPM_GameInstantiator.h"

#include "lPacMan.h"
#include "../../../lGame/lGame.h"

liGame *lPM_GameInstantiator::CreateGame(liApiAdapter &api_adapter,lrmResourceManager &resource_manager,liRenderer &renderer,liConsole &console)
{
	liGameMode *GameMode = new lPM_Game(api_adapter.GetInput(),renderer,0.125);
	liGame *Game = new lGame(console,GameMode,renderer);
	//
	return Game;
}

liGameInstantiator *CreateGameInstantiator()
{
    return new lPM_GameInstantiator;
}
