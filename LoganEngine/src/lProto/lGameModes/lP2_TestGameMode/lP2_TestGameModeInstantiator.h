#ifndef LP2_TEST_GAME_MODE_INSTANTIATOR_H
#define LP2_TEST_GAME_MODE_INSTANTIATOR_H

#include "../../liGameInstantiator.h"

class lP2_TestGameInstantiator : public liGameInstantiator
{
public:
	virtual liGame *CreateGame(liApiAdapter &api_adapter,lrmResourceManager &resource_manager,liRenderer &renderer,liConsole &console) override;
	//
	lP2_TestGameInstantiator()
	{}
	//
	virtual ~lP2_TestGameInstantiator() override
	{}
	/*
	 * End of class
	 */
};

#endif // LP2_TEST_GAME_MODE_INSTANTIATOR_H
