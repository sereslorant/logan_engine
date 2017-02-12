#ifndef LI_GAME_INSTANTIATOR_H
#define LI_GAME_INSTANTIATOR_H

#include <lGame/liGame.h>
#include <lGame/liConsole.h>

#include <lApiAdapter/liApiAdapter.h>
#include <lResourceManager/lResourceManager.h>
#include <lRenderer/liRenderer.h>

class liGameInstantiator
{
public:
	virtual liGame *CreateGame(liApiAdapter &api_adapter,lrmResourceManager &resource_manager,liRenderer &renderer,liConsole &console) = 0;

	liGameInstantiator(){}
	virtual ~liGameInstantiator(){}
};

#endif // LI_GAME_INSTANTIATOR_H