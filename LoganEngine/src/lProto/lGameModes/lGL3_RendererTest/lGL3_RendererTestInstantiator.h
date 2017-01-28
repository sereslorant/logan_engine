#ifndef LGL3_RENDERERTESTINSTANTIATOR_H
#define LGL3_RENDERERTESTINSTANTIATOR_H

#include "../../liGameInstantiator.h"
#include "../../../lTest/TestRenderer/DummyRM.h"

class lGL3_RendererTestInstantiator : public liGameInstantiator
{
private:
	DummyRM *Dummy = new DummyRM;
	//
public:
	virtual liGame* CreateGame(liApiAdapter& api_adapter, lrmResourceManager& resource_manager, liRenderer& renderer, liConsole& console);
	//
	lGL3_RendererTestInstantiator(){}
	virtual ~lGL3_RendererTestInstantiator() override {delete Dummy;}
};

#endif // LGL3_RENDERERTESTINSTANTIATOR_H
