#ifndef LI_AGENT_CONTROL_H
#define LI_AGENT_CONTROL_H

class liPM_AgentController
{
public:
	virtual void Think() = 0;

	virtual bool GetUp() = 0;
	virtual bool GetDown() = 0;
	virtual bool GetLeft() = 0;
	virtual bool GetRight() = 0;

	liPM_AgentController(){}
	virtual ~liPM_AgentController(){}
};

#endif // LI_AGENT_CONTROL_H
