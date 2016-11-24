#ifndef L_GOO_LINE_ANIMATE_H
#define L_GOO_LINE_ANIMATE_H

#include "lGooScalarAnimate.h"
#include "../../lGooLine.h"

class lGooLineAnimateParam : public lGooScalarAnimateParam::liGooScalarAnimState
{
protected:
    lGooLine *Line;
    const lGooLine::lGooLineAttrib LineAttrib;

public:

	virtual lmScalar GetParam() override
	{
		return (Line->*LineAttrib.GetValue)();
	}

    virtual void SetParam(lmScalar value) override
    {
    	(Line->*LineAttrib.SetValue)(value);
    }

    virtual void IncreaseParam(lmScalar d_value) override
    {
    	(Line->*LineAttrib.IncValue)(d_value);
    }

    lGooLineAnimateParam(lGooLine *line,const lGooLine::lGooLineAttrib &line_attrib)
        :Line(line),LineAttrib(line_attrib)
    {}

    virtual ~lGooLineAnimateParam() override
    {}

    /*
     * Class vége
     */
};

#endif // L_GOO_LINE_ANIMATE_H
