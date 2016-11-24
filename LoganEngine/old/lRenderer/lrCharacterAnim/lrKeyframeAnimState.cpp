
#include "lrKeyframeAnimState.h"

#include "../../lGameModes/lEvent.h"



#include <iostream>

class TEST_FinishedCallback : public liEventCallback
{
public:
	virtual void Callback() override
	{
		std::cout << "Finished" << std::endl;
	}

	TEST_FinishedCallback(){}
	virtual ~TEST_FinishedCallback() override{}
};



lrKeyframeAnimState::lrKeyframeAnimState(liKeyframeAnimation *keyframe_animation)
	:KeyframeAnimation(keyframe_animation)
{
	FinishedEvent = new lEvent;

	TestFinishedCallback = new TEST_FinishedCallback;
	FinishedEvent->AddCallback(TestFinishedCallback);
}

lrKeyframeAnimState::~lrKeyframeAnimState()
{
	delete FinishedEvent;

	delete TestFinishedCallback;
}
