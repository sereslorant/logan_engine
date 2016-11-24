#ifndef L_GOO_CIRCLE_ANIMATE_H
#define L_GOO_CIRCLE_ANIMATE_H

#include "lGooScalarAnimate.h"
#include "../../lGooCircle.h"

class lGooCircleAnimateParam : public lGooScalarAnimateParam::liGooScalarAnimState
{
protected:
    lGooCircle *Circle;
	const lGooCircle::lGooCircleAttrib CircleAttrib;

public:

	virtual lmScalar GetParam() override
	{
		return (Circle->*CircleAttrib.GetValue)();
	}

    virtual void SetParam(lmScalar value) override
    {
    	(Circle->*CircleAttrib.SetValue)(value);
    }

    virtual void IncreaseParam(lmScalar d_value) override
    {
    	(Circle->*CircleAttrib.IncValue)(d_value);
    }

    lGooCircleAnimateParam(lGooCircle *circle,const lGooCircle::lGooCircleAttrib &circle_attrib)
        :Circle(circle),CircleAttrib(circle_attrib)
    {}

    virtual ~lGooCircleAnimateParam() override
    {}

    /*
     * Class vége
     */
};

#endif // L_GOO_CIRCLE_ANIMATE_H
