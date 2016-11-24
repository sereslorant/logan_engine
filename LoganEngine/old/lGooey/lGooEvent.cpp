
#include "lGooEvent.h"

void lGooEvent::AddOffsetEvent(lmScalar offset,liTimer *timer)
{
	auto I = OffsetEvents.find(offset);

	if(I == OffsetEvents.end())
	{
		OffsetEvents[offset] = timer;

		if(offset > MaxOffsetEvent)
		{
			MaxOffsetEvent = offset;
		}
	}
}

liTimer *lGooEvent::GetOffsetEvent(lmScalar offset)
{
	auto I = OffsetEvents.find(offset);

	if(I == OffsetEvents.end())
	{
		return nullptr;
	}

	return I->second;
}

void lGooEvent::AddCallback(liEventCallback *callback)
{
	Callbacks.push_back(callback);
}

void lGooEvent::Fire()
{
	for(auto I : Callbacks)
	{
		I->Callback();
	}

	TimeElapsed = 0.0;
	Fired = true;
}

lGooEvent::lGooEvent()
{

}

lGooEvent::~lGooEvent()
{
	for(auto I : OffsetEvents)
	{
		delete I.second;
	}
}
