
#include "lGooAnimate.h"

/*
 * TMP!!!!!!!!!!!!!!!!!!!
 * TEST
 * TMP!!!!!!!!!!!!!!!!!!!
 */

#include <iostream>

class lTEST_BeginCallback : public liEventCallback
{
	virtual void Callback() override
	{
		std::cout << "Begin" << std::endl;
	}
};

class lTEST_EndCallback : public liEventCallback
{
	virtual void Callback() override
	{
		std::cout << "End" << std::endl;
	}
};

class lTEST_RepeatCallback : public liEventCallback
{
	virtual void Callback() override
	{
		std::cout << "Repeat" << std::endl;
	}
};
/*
 * TMP!!!!!!!!!!!!!!!!!!!
 * TEST
 * TMP!!!!!!!!!!!!!!!!!!!
 */

class lGooAnimBeginCallback : public liEventCallback
{
private:
	lGooAnimate *Anim;
public:
	virtual void Callback() override
	{
		Anim->Trigger();

		//std::cout << "AnimTriggered" << std::endl;
	}

	lGooAnimBeginCallback(lGooAnimate *anim)
		:Anim(anim)
	{

	}

	virtual ~lGooAnimBeginCallback() override
	{

	}
};

void lGooAnimate::Step(lmScalar dt)
{
	BeginEvent.Update(dt);
    EndEvent.Update(dt);
    RepeatEvent.Update(dt);

	switch(AnimState)
	{
	case IDLE:
		break;

	case TRIGGERED:
		BeginEvent.Fire();

	case STARTING:
		AnimateParam->SetBeginValue();
		TimeElapsed = 0.0;
		AnimState = RUNNING;
		break;

	case RUNNING:
		AnimateParam->IncreaseParam(dt);
		TimeElapsed += dt;

		if(TimeElapsed > Duration)
		{
			AnimState = OVER;
		}

		if(RepeatDuration > 0.0)
		{
			RepeatDurTimer += dt;

			if(RepeatDurTimer > RepeatDuration)
			{
				AnimState = OVER;
			}
		}

		break;

	case OVER:

		if(RepeatDuration == 0.0)
		{
			RepeatEvent.Fire();
			TimeElapsed = 0.0;

			AnimState = STARTING;
		}
		else
		{
			if(AnimFill == FREEZE)
			{
				//lmScalar Szercsy = TimeElapsed / Duration;

				if(TimeElapsed > 0.0 && TimeElapsed < (Duration - 1e-4))
				{
					AnimateParam->SetCurrentValue(TimeElapsed);
				}
				else
				{
					AnimateParam->SetEndValue();
				}
			}
			else if(AnimFill == REMOVE)
			{
				AnimateParam->SetInitialValue();
			}

			if(RepeatDurTimer > RepeatDuration)
			{
				EndEvent.Fire();

				AnimState = IDLE;
				RepeatDurTimer = 0.0;
			}
			else
			{
				RepeatEvent.Fire();

				AnimState = STARTING;
				TimeElapsed = 0.0;
			}
		}
		break;
	}
}

lGooAnimate::lGooAnimate(liGooAnimateParam *animate_param,lmScalar duration,lmScalar repeat_duration,lGooAnimFill anim_fill)
	:AnimateParam(animate_param),Duration(duration),AnimFill(anim_fill),AnimState(IDLE)
{
	if(repeat_duration < 0.0)
	{
		RepeatDuration = 0.0;
	}
	else
	{
		RepeatDuration = repeat_duration;
	}

	TimeElapsed = 0.0;
	RepeatDurTimer = 0.0;
	/*
	TEST_BeginCallback = new lTEST_BeginCallback();
	BeginEvent.AddCallback(TEST_BeginCallback);
	TEST_EndCallback = new lTEST_EndCallback();
	EndEvent.AddCallback(TEST_EndCallback);
	TEST_RepeatCallback = new lTEST_RepeatCallback();
	RepeatEvent.AddCallback(TEST_RepeatCallback);
	*/
	BeginCallback = new lGooAnimBeginCallback(this);
}

lGooAnimate::~lGooAnimate()
{
	delete TEST_BeginCallback;
	delete TEST_EndCallback;
	delete TEST_RepeatCallback;

	delete BeginCallback;
}
