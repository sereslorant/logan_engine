#ifndef L_GOO_ANIMATION_H
#define L_GOO_ANIMATION_H

#include "../../lMath/lMath.h"

#include <list>

#include "../lGooEvent.h"
#include "../../lGameModes/lTimer.h"

class lGooAnimation
{
public:

	virtual lGooEvent *GetBeginEvent() = 0;
	virtual lGooEvent *GetEndEvent() = 0;
	virtual lGooEvent *GetRepeatEvent() = 0;

	virtual liEventCallback *GetBeginCallback() = 0;

    lGooAnimation()
    {

    }

    virtual ~lGooAnimation()
    {

    }

    virtual void Trigger() = 0;

    virtual void Step(lmScalar dt) = 0;
};

#endif // L_GOO_ANIMATION_H
