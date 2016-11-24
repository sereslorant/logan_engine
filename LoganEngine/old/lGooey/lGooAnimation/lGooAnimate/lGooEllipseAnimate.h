#ifndef L_GOO_ELLIPSE_ANIMATE_H
#define L_GOO_ELLIPSE_ANIMATE_H

#include "lGooScalarAnimate.h"
#include "../../lGooEllipse.h"

class lGooEllipseAnimateParam : public lGooScalarAnimateParam::liGooScalarAnimState
{
protected:
    lGooEllipse *Ellipse;
    const lGooEllipse::lGooEllipseAttrib EllipseAttrib;

public:

	virtual lmScalar GetParam() override
	{
		return (Ellipse->*EllipseAttrib.GetValue)();
	}

    virtual void SetParam(lmScalar value) override
    {
    	(Ellipse->*EllipseAttrib.SetValue)(value);
    }

    virtual void IncreaseParam(lmScalar d_value) override
    {
    	(Ellipse->*EllipseAttrib.IncValue)(d_value);
    }

    lGooEllipseAnimateParam(lGooEllipse *ellipse,const lGooEllipse::lGooEllipseAttrib &ellipse_attrib)
        :Ellipse(ellipse),EllipseAttrib(ellipse_attrib)
    {}

    virtual ~lGooEllipseAnimateParam() override
    {}

    /*
     * Class vége
     */
};

#endif // L_GOO_ELLIPSE_ANIMATE_H
