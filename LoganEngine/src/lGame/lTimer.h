#ifndef L_TIMER_H
#define L_TIMER_H

#include "../lInterfaces/lGame/liTimer.h"

#include "lEvent.h"

class lTimer : public liTimer
{
private:
	bool Removable = false;

	bool Enabled;
	bool Repeat;
	double TimeElapsed;
	double TimeLimit;

	class lEnableTimerCallback : public liEventCallback
	{
	private:
		lTimer &Timer;

	public:

		virtual void Callback() override;

		lEnableTimerCallback(lTimer &timer);
		virtual ~lEnableTimerCallback() override;
	};

	class lDisableTimerCallback : public liEventCallback
	{
	private:
		lTimer &Timer;

	public:

		virtual void Callback() override;

		lDisableTimerCallback(lTimer &timer);
		virtual ~lDisableTimerCallback() override;
	};

	liEventCallback *EnableTimerCallback;
	liEventCallback *DisableTimerCallback;

	lEvent LimitReachedEvent;

public:
	virtual void MarkRemovable() override
	{Removable = true;}

	virtual bool IsRemovable() override
	{return Removable;}

	virtual void Enable() override;
	virtual void Disable() override;

	virtual bool IsEnabled() override;
	virtual bool GetRepeat() override;

	virtual double	GetTimeLimit() override;
	virtual void	SetTimeLimit(double time_limit) override;
	virtual double	GetTimeElapsed() override;

	virtual liEvent			*GetLimitReachedEvent() override;
	virtual liEventCallback *GetEnableTimerCallback() override;
	virtual liEventCallback *GetDisableTimerCallback() override;

	virtual void Update(double dt)override;

	lTimer(double time_limit,bool repeat = false);
	virtual ~lTimer() override;
};

#endif
