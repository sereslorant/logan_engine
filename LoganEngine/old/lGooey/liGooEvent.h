#ifndef LI_GOO_EVENT_H
#define LI_GOO_EVENT_H

#include "../lMath/lMath.h"
#include "../lInterfaces/lGameMode/liEvent.h"
#include "../lInterfaces/lGameMode/liTimer.h"

class liGooEvent : public liEvent
{
public:
	virtual void AddOffsetEvent(lmScalar offset,liTimer *timer) = 0;
    virtual liTimer *GetOffsetEvent(lmScalar offset) = 0;

    virtual void Update(double dt) = 0;

	liGooEvent(){}
	virtual ~liGooEvent(){}
};

#endif // L_GOO_EVENT_H
