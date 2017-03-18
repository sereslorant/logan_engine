#ifndef LPM_GAME_INSTANTIATOR_H
#define LPM_GAME_INSTANTIATOR_H

#include <lGame/liGameInstantiator.h>

class lPM_GameInstantiator : public liGameInstantiator
{
public:
	virtual liGame *CreateGame(liApiAdapter &api_adapter,liResourceManager &resource_manager,liRenderer &renderer,liConsole &console) override;
	//
	lPM_GameInstantiator()
	{}
	//
	virtual ~lPM_GameInstantiator() override
	{}
	/*
	 * End of class
	 */
};

#endif // LPM_GAME_INSTANTIATOR_H