#ifndef L_GOO_SCALAR_ANIMATE_H
#define L_GOO_SCALAR_ANIMATE_H

#include "lGooAnimate.h"

class lGooScalarAnimateParam : public lGooAnimate::liGooAnimateParam
{
public:
	class liGooScalarAnimState
	{
	public:
		virtual lmScalar GetParam() = 0;
		virtual void SetParam(lmScalar value) = 0;
		virtual void IncreaseParam(lmScalar d_value) = 0;

		liGooScalarAnimState(){}
		virtual ~liGooScalarAnimState(){}

		/*
		 * Class vége
		 */
	};

private:
	liGooScalarAnimState *AnimState;

	lmScalar InitialValue;

    lmScalar BeginValue;
    lmScalar EndValue;
    lmScalar dValue;

public:

	virtual void SetBeginValue() override;
	virtual void SetEndValue() override;
	virtual void SetInitialValue() override;

	virtual void SetCurrentValue(lmScalar time_elapsed) override;
    virtual void IncreaseParam(lmScalar dt) override;

    lGooScalarAnimateParam(liGooScalarAnimState *anim_state,lmScalar begin_value,lmScalar end_value,lmScalar duration);
    virtual ~lGooScalarAnimateParam() override;

    /*
     * Class vége
     */
};

#endif
