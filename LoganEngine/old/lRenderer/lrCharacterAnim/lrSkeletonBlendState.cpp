
#include "lrSkeletonBlendState.h"

#include "../../lGameModes/lEvent.h"



#include <iostream>

class TEST_EndCallback : public liEventCallback
{
public:
	virtual void Callback() override
	{
		std::cout << "Finished" << std::endl;
	}

	TEST_EndCallback(){}
	virtual ~TEST_EndCallback() override{}
};



lrSkeletonBlendState::lrSkeletonBlendState(liSkeleton *begin,liSkeleton *end,liBlendFunc *blend_func,double transition_time)
	:TransitionTime(transition_time),/*Begin(begin),End(end),*/BlendFunc(blend_func)
{
	Begin.Copy(*begin);
	End.Copy(*end);

	EndEvent = new lEvent;

	TestEndCallback = new TEST_EndCallback;
	EndEvent->AddCallback(TestEndCallback);
}

lrSkeletonBlendState::~lrSkeletonBlendState()
{
	delete BlendFunc;
	delete EndEvent;

	delete TestEndCallback;
}
