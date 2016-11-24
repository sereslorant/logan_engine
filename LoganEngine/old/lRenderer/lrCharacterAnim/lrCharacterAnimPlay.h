#ifndef LR_CHARACTER_ANIM_PLAY_H
#define LR_CHARACTER_ANIM_PLAY_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liCharacterAnim.h"

class lrCharacterAnimPlay : public liCharacterAnimState
{
private:
	double Duration;
	double TimeElapsed = 0.0;

	liCharacterAnimState *AnimState;

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
	}

	virtual void Update(double dt) override
	{
		if(TimeElapsed < Duration)
		{
			AnimState->Update(dt);

			TimeElapsed += dt;
		}
		else
		{
			EndEvent->Fire();
		}
	}

	virtual void GetSkeleton(liSkeleton *destination) override
	{
		AnimState->GetSkeleton(destination);
	}

	lrCharacterAnimPlay(liCharacterAnimState *anim_state,double duration);

	virtual ~lrCharacterAnimPlay() override;
};

#endif // LR_CHARACTER_ANIM_PLAY_H
