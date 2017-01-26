#ifndef LI_GAME_INSTANTIATOR_H
#define LI_GAME_INSTANTIATOR_H

#include "../lInterfaces/lGame/liGame.h"

#include "../lInterfaces/lApiAdapter/liApiAdapter.h"
#include "../lResourceManager/lResourceManager.h"
#include "../lInterfaces/lRenderer/liRenderer.h"
#include "../lInterfaces/lGame/liConsole.h"

class liGameInstantiator
{
public:
	virtual liGame *CreateGame(liApiAdapter &api_adapter,lrmResourceManager &resource_manager,liRenderer &renderer,liConsole &console) = 0;

	liGameInstantiator(){}
	virtual ~liGameInstantiator(){}
};

#endif // LI_GAME_INSTANTIATOR_H