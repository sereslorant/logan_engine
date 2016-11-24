#ifndef LR_LINEAR_BLEND_FUNC_H
#define LR_LINEAR_BLEND_FUNC_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liBlendFunc.h"

class lrLinearBlendFunc : public liBlendFunc
{
private:
	double TransitionTime;
	double TimeElapsed = 0.0;

	double Constant;
	double Slope;

public:

	virtual void Reset() override
	{
		TimeElapsed = 0.0;
	}

	virtual void Update(double dt) override
	{
		TimeElapsed += dt;
	}

	virtual double GetWeight() override
	{
		double t = TimeElapsed/TransitionTime;

		return lmClamp(0.0,1.0,Constant + t*Slope);
	}

	lrLinearBlendFunc(double transition_time,double constant,double slope)
		:TransitionTime(transition_time),Constant(constant),Slope(slope)
	{}

	virtual ~lrLinearBlendFunc() override
	{}
};

#endif // LR_LINEAR_BLEND_FUNC_H
