#ifndef L_GOO_EVENT_H
#define L_GOO_EVENT_H

#include <list>
#include <map>

#include "liGooEvent.h"

class lGooEvent : public liGooEvent
{
private:
	std::list<liEventCallback *> Callbacks;
	std::map<lmScalar,liTimer *>OffsetEvents;

	bool Fired = false;
	double TimeElapsed = 0.0;
	double MaxOffsetEvent = 0.0;

public:

	virtual void AddOffsetEvent(lmScalar offset,liTimer *timer) override;
    virtual liTimer *GetOffsetEvent(lmScalar offset) override;

	virtual void AddCallback(liEventCallback *callback) override;

	virtual void Fire() override;

	virtual void Update(double dt)
	{
		if(Fired)
		{
			TimeElapsed += dt;

			for(auto I : OffsetEvents)
			{
				if(I.first < TimeElapsed)
				{
					I.second->Enable();
				}

				I.second->Update(dt);
			}

			if(TimeElapsed > MaxOffsetEvent)
			{
				Fired = false;
			}
		}
	}

	lGooEvent();

	virtual ~lGooEvent();
};

#endif
