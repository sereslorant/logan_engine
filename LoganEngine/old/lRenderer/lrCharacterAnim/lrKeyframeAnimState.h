#ifndef LR_KEYFRAME_ANIM_STATE_H
#define LR_KEYFRAME_ANIM_STATE_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liCharacterAnim.h"
#include "../../lInterfaces/lRenderer/lrCharacterAnim/liBlendFunc.h"
#include "../../lInterfaces/lRenderer/lrCharacterAnim/liKeyframeAnimation.h"

class lrKeyframeAnimState : public liCharacterAnimState
{
private:
	double TimeElapsed;
	liKeyframeAnimation *KeyframeAnimation;

	liEvent *FinishedEvent;

	liEventCallback *TestFinishedCallback;

public:

	liEvent *GetFinishedEvent()
	{
		return FinishedEvent;
	}

	virtual void Reset() override
	{
		TimeElapsed = 0.0;
	}

	virtual void Update(double dt) override
	{
		TimeElapsed += dt;
		while(TimeElapsed > KeyframeAnimation->GetAnimDuration())
		{
			TimeElapsed -= KeyframeAnimation->GetAnimDuration();
			FinishedEvent->Fire();
		}
	}

	virtual void GetSkeleton(liSkeleton *destination) override
	{
		KeyframeAnimation->GetAnimation(TimeElapsed,destination);
	}

	lrKeyframeAnimState(liKeyframeAnimation *keyframe_animation);

	virtual ~lrKeyframeAnimState() override;
};

#endif // LR_KEYFRAME_ANIM_STATE_H
