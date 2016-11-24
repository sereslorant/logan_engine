
#include "lrCharacterAnimPlay.h"

#include "../../lGameModes/lEvent.h"

#include <iostream>

class TEST_EndCallback : public liEventCallback
{
public:
	virtual void Callback() override
	{
		std::cout << "End" << std::endl;
	}

	TEST_EndCallback(){}
	virtual ~TEST_EndCallback() override{}
};



lrCharacterAnimPlay::lrCharacterAnimPlay(liCharacterAnimState *anim_state,double duration)
	:Duration(duration),AnimState(anim_state)
{
	EndEvent = new lEvent;

	TestEndCallback = new TEST_EndCallback;
	EndEvent->AddCallback(TestEndCallback);
}

lrCharacterAnimPlay::~lrCharacterAnimPlay()
{
	delete EndEvent;

	delete TestEndCallback;
}
