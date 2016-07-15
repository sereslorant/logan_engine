#ifndef LI_AGENT_H
#define LI_AGENT_H

class liAgent
{
public:
	virtual void MarkRemovable() = 0;
	virtual bool IsRemovable() = 0;

	virtual void Update(double dt) = 0;

	liAgent(){}

	virtual ~liAgent() {}
};

#endif // LI_AGENT_H
