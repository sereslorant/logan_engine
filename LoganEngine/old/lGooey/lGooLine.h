#ifndef L_GOO_LINE_H
#define L_GOO_LINE_H

#include "lGooObject.h"

class lGooLine : public lGooObject, public liGooLine
{
public:

    typedef void (lGooLine::* AttribSetter) (lmScalar value);
    typedef lmScalar (lGooLine::* AttribGetter) () const;

    struct lGooLineAttrib
    {
    	AttribSetter SetValue;
    	AttribSetter IncValue;
    	AttribGetter GetValue;

    	lGooLineAttrib(AttribSetter set_value,AttribSetter inc_value,AttribGetter get_value)
			:SetValue(set_value),IncValue(inc_value),GetValue(get_value)
    	{}
    };

protected:
	lmVector2D V2;

public:

    lGooLine(liGuiElement *polygon)
		:lGooObject(polygon)
    {

    }

	lGooLine(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon,const lmVector2D &v2)
		:lGooObject(pos,scale,orientation,polygon),V2(v2)
	{

	}

	~lGooLine()
	{

	}

	void SetVX(lmScalar x)
	{
		V2[0] = x;
	}

	void SetVY(lmScalar y)
	{
		V2[1] = y;
	}

	void IncreaseVX(lmScalar dx)
	{
		V2[0] += dx;
	}

	void IncreaseVY(lmScalar dy)
	{
		V2[1] += dy;
	}

	lmScalar GetVX() const
	{
		return V2[0];
	}

	lmScalar GetVY() const
	{
		return V2[1];
	}

	virtual const lmVector2D &GetV2() const override
	{
		return V2;
	}

	virtual void SetV2(const lmVector2D &v2) override
	{
		V2 = v2;
	}

	virtual lmMatrix3x3 Transform() const override
	{
		lmMatrix3x3 mTranslate  = lmTranslate3x3(Position);
		lmMatrix3x3 mRotate     = lmRotate3x3(lmDegToRad(Orientation));
		lmMatrix3x3 mScale      = lmScale3x3(Scale);

		lmMatrix3x3 mV2;
		mV2[0][0] = 1;mV2[1][0] = V2[0] - Position[0];mV2[2][0] = 0;
		mV2[0][1] = 0;mV2[1][1] = V2[1] - Position[1];mV2[2][1] = 0;
		mV2[0][2] = 0;mV2[1][2] = 0;			 mV2[2][2] = 1;

		if(Parent != nullptr)
		{
			return Parent->Transform() * mTranslate * mRotate * mScale * mV2;
		}
		else
		{
			return mTranslate * mRotate * mScale * mV2;
		}
	}

    virtual liGooLine *ToLine() override {return this;}
};

#endif
