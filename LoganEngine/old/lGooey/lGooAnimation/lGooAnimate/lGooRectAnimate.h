#ifndef L_GOO_RECT_ANIMATE_H
#define L_GOO_RECT_ANIMATE_H

#include "lGooScalarAnimate.h"
#include "../../lGooRect.h"

class lGooRectAnimateParam : public lGooScalarAnimateParam::liGooScalarAnimState
{
protected:
    lGooRect *Rect;
    const lGooRect::lGooRectAttrib RectAttrib;

public:

	virtual lmScalar GetParam() override
	{
		return (Rect->*RectAttrib.GetValue)();
	}

    virtual void SetParam(lmScalar value) override
    {
    	(Rect->*RectAttrib.SetValue)(value);
    }

    virtual void IncreaseParam(lmScalar d_value) override
    {
    	(Rect->*RectAttrib.IncValue)(d_value);
    }

    lGooRectAnimateParam(lGooRect *rect,const lGooRect::lGooRectAttrib &rect_attrib)
        :Rect(rect),RectAttrib(rect_attrib)
    {}

    virtual ~lGooRectAnimateParam() override
    {}

    /*
     * Class vége
     */
};

#endif // L_GOO_RECT_ANIMATE_H
