#ifndef LPM_AI_H
#define LPM_AI_H

#include "liPM_AgentController.h"

class lPM_AI_Controller : public liPM_AgentController
{
private:
	enum AI_DIRECTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

	AI_DIRECTION Direction = RIGHT;

public:

	virtual void Think() override
	{
		switch(Direction)
		{
		case UP:
			Direction = LEFT;
			break;
		case LEFT:
			Direction = DOWN;
			break;
		case DOWN:
			Direction = RIGHT;
			break;
		case RIGHT:
			Direction = UP;
			break;
		}
	}

	virtual bool GetUp() override
	{
		return Direction == UP;
	}

	virtual bool GetDown() override
	{
		return Direction == DOWN;
	}

	virtual bool GetLeft() override
	{
		return Direction == LEFT;
	}

	virtual bool GetRight() override
	{
		return Direction == RIGHT;
	}

	lPM_AI_Controller(){}
	virtual ~lPM_AI_Controller() override {}
};

#endif // LPM_AI_H

