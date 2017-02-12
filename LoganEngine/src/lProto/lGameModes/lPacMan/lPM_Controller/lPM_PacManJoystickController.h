#ifndef LPM_PAC_MAN_JOYSTICK_CONTROLLER_H
#define LPM_PAC_MAN_JOYSTICK_CONTROLLER_H

#include "liPM_AgentController.h"

#include <lApiAdapter/liInput.h>

struct lPM_PacManJoystickController : public liPM_AgentController
{
private:
	liController *Controller;

	bool Up = false;
	bool Down = false;
	bool Left = false;
	bool Right = false;

public:

	virtual void Think() override
	{
		if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) > 0.01)
		{
			Up = false;
			Down = true;
		}
		else if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) < -0.01)
		{
			Up = true;
			Down = false;
		}
		else
		{
			Up = false;
			Down = false;
		}

		if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) > 0.01)
		{
			Left = false;
			Right = true;
		}
		else if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) < -0.01)
		{
			Left = true;
			Right = false;
		}
		else
		{
			Left = false;
			Right = false;
		}
	}

	virtual bool GetUp() override
	{
		return Up;
	}

	virtual bool GetDown() override
	{
		return Down;
	}

	virtual bool GetLeft() override
	{
		return Left;
	}

	virtual bool GetRight() override
	{
		return Right;
	}

	lPM_PacManJoystickController(liController *controller)
		:Controller(controller)
	{
		//Üres
	}

	virtual ~lPM_PacManJoystickController() override
	{
		//Üres
	}
};


#endif // LPM_PAC_MAN_JOYSTICK_CONTROLLER_H
