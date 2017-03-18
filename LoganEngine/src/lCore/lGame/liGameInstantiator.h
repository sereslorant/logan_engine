#ifndef LI_GAME_INSTANTIATOR_H
#define LI_GAME_INSTANTIATOR_H

#include <lApiAdapter/liApiAdapter.h>
#include <lApiAdapter/liConsole.h>
#include <lResourceManager/liResourceManager.h>
#include <lRenderer/liRenderer.h>

#include "liGame.h"

class liGameInstantiator
{
public:
	virtual liGame *CreateGame(liApiAdapter &api_adapter,liResourceManager &resource_manager,liRenderer &renderer,liConsole &console) = 0;

	liGameInstantiator(){}
	virtual ~liGameInstantiator(){}
};

#endif // LI_GAME_INSTANTIATOR_H