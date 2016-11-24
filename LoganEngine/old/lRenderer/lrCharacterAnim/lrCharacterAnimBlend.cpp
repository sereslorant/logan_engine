
#include "lrCharacterAnimBlend.h"
#include "lrSkeleton.h"

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



void lrCharacterAnimBlend::GetSkeleton(liSkeleton *destination)
{
	lrSkeleton Skeleton[2];

	CurrentState->GetSkeleton(&Skeleton[0]);
	NextState->GetSkeleton(&Skeleton[1]);

	double Weight = BlendFunc->GetWeight();

	lrLinearInterpolation(&Skeleton[0],&Skeleton[1],Weight,destination);
}

lrCharacterAnimBlend::lrCharacterAnimBlend(liCharacterAnimState *current_state,liCharacterAnimState *next_state,liBlendFunc *blend_func,double transition_time)
	:TransitionTime(transition_time),CurrentState(current_state),NextState(next_state),BlendFunc(blend_func)
{
	EndEvent = new lEvent;

	TestEndCallback = new TEST_EndCallback;
	EndEvent->AddCallback(TestEndCallback);
}

lrCharacterAnimBlend::~lrCharacterAnimBlend()
{
	delete BlendFunc;

	delete EndEvent;

	delete TestEndCallback;
}
