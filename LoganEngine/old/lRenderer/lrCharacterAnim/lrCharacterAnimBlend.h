#ifndef LR_CHARACTER_ANIM_BLEND_H
#define LR_CHARACTER_ANIM_BLEND_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liCharacterAnim.h"
#include "../../lInterfaces/lRenderer/lrCharacterAnim/liBlendFunc.h"

class lrCharacterAnimBlend : public liCharacterAnimState
{
private:
	double TransitionTime;
	double TimeElapsed = 0.0;

	liCharacterAnimState *CurrentState;
	liCharacterAnimState *NextState;

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
		NextState->Reset();
		BlendFunc->Reset();
	}

	virtual void Update(double dt) override
	{
		if(TimeElapsed < TransitionTime)
		{
			CurrentState->Update(dt);
			NextState->Update(dt);

			BlendFunc->Update(dt);

			TimeElapsed += dt;
		}
		else
		{
			EndEvent->Fire();
		}
	}

	virtual void GetSkeleton(liSkeleton *destination) override;

	lrCharacterAnimBlend(liCharacterAnimState *current_state,liCharacterAnimState *next_state,liBlendFunc *blend_func,double transition_time);

	virtual ~lrCharacterAnimBlend() override;
};

#endif // LR_CHARACTER_ANIM_BLEND_H
