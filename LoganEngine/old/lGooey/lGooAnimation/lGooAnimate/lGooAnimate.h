#ifndef L_GOO_ANIMATE_H
#define L_GOO_ANIMATE_H

#include "../lGooAnimation.h"

/** \brief
 * \todo ÁT KELL TERVEZNI!!!
 *
 */
class lGooAnimate : public lGooAnimation
{
public:

	enum lGooAnimFill
	{
		FREEZE,
		REMOVE,
	};

    enum lGooAnimState
    {
        IDLE,
        TRIGGERED,
        STARTING,
        RUNNING,
        OVER,
    };

	class liGooAnimateParam
	{
	public:
		virtual void SetBeginValue() = 0;
		virtual void SetEndValue() = 0;
		virtual void SetInitialValue() = 0;

		virtual void SetCurrentValue(lmScalar time_elapsed) = 0;
		virtual void IncreaseParam(lmScalar dt) = 0;

		liGooAnimateParam()
		{
			//Üres függvénytörzs
		}

		virtual ~liGooAnimateParam()
		{
			//Üres függvénytörzs
		}

		/*
		 * Class vége
		 */
	};

protected:
    liGooAnimateParam *AnimateParam;

    lmScalar TimeElapsed;
    lmScalar RepeatDurTimer;

    lmScalar Duration;
    lmScalar RepeatDuration; /**< \todo KI KELL CSERÉLNI AIM_DURATIONRE!!! */

    lGooAnimFill AnimFill;
    lGooAnimState AnimState;

    lGooEvent BeginEvent;
    lGooEvent EndEvent;
    lGooEvent RepeatEvent;

    /*TMP!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    liEventCallback *TEST_BeginCallback		= nullptr;
    liEventCallback *TEST_EndCallback		= nullptr;
    liEventCallback *TEST_RepeatCallback	= nullptr;
    /*TMP!!!!!!!!!!!!!!!!!!!!!!!!!!*/

    liEventCallback *BeginCallback;

public:

	lGooEvent *GetBeginEvent()
	{
		return &BeginEvent;
	}

	lGooEvent *GetEndEvent()
	{
		return &EndEvent;
	}

	lGooEvent *GetRepeatEvent()
	{
		return &RepeatEvent;
	}

	liEventCallback *GetBeginCallback()
	{
		return BeginCallback;
	}

    void Trigger()
    {
        AnimState = TRIGGERED;
    }

    virtual void Step(lmScalar dt) override;

    lGooAnimate(liGooAnimateParam *animate_param,lmScalar duration,lmScalar repeat_duration,lGooAnimFill anim_fill);

    virtual ~lGooAnimate() override;

    /*
     * Class vége
     */
};

#endif // L_GOO_ANIMATE_H
