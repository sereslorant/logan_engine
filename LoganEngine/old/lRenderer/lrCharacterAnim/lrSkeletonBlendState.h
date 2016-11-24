#ifndef LR_SKELETON_BLEND_STATE_H
#define LR_SKELETON_BLEND_STATE_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liCharacterAnim.h"
#include "../../lInterfaces/lRenderer/lrCharacterAnim/liBlendFunc.h"

#include "lrSkeleton.h"

class lrSkeletonBlendState : public liCharacterAnimState
{
private:
	double TransitionTime;
	double TimeElapsed = 0.0;

	lrSkeleton Begin;
	lrSkeleton End;

	liBlendFunc *BlendFunc;

	liEvent *EndEvent;

	liEventCallback *TestEndCallback;

public:

	liEvent *GetEndEvent()
	{
		return EndEvent;
	}

	virtual void Reset() override
	{
		TimeElapsed = 0.0;
		BlendFunc->Reset();
	}

	virtual void Update(double dt) override
	{
		BlendFunc->Update(dt);
		TimeElapsed += dt;

		while(TimeElapsed > TransitionTime)
		{
			TimeElapsed -= TransitionTime;
			BlendFunc->Reset();

			EndEvent->Fire();
		}
	}

	virtual void GetSkeleton(liSkeleton *destination) override
	{
		double Weight = BlendFunc->GetWeight();

		lrLinearInterpolation(&Begin,&End,Weight,destination);
	}

	lrSkeletonBlendState(liSkeleton *begin,liSkeleton *end,liBlendFunc *blend_func,double transition_time);

	virtual ~lrSkeletonBlendState() override;
};

#endif // LR_SKELETON_BLEND_STATE_H
