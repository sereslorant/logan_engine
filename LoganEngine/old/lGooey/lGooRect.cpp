
#include "lGooRect.h"

/*lGooRect::lGooRect():lGooObject(L_GOO_RECT),Width(1.0),Height(1.0)
{

}*/

lGooRect::lGooRect(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon,lmScalar width,lmScalar height)
    :lGooObject(pos,scale,orientation,polygon),Width(width),Height(height)
{

}

lGooRect::~lGooRect()
{

}

void lGooRect::SetWidth(lmScalar width)
{
    Width = width;
}

void lGooRect::SetHeight(lmScalar height)
{
    Height = height;
}

lmMatrix3x3 lGooRect::Transform() const
{
    lmMatrix3x3 mTranslate  = lmTranslate3x3(Position);
    lmMatrix3x3 mRotate     = lmRotate3x3(lmDegToRad(Orientation));

    //lmMatrix3x3 mScale      = lmMatrix3x3::lmScale(Scale.X,Scale.Y);
    lmMatrix3x3 mSize      = lmScale3x3(Width,Height);
    //lmMatrix3x3

    if(Parent != nullptr)
	{
		return Parent->Transform() * mTranslate * mRotate /** mScale*/ * mSize;
	}
	else
	{
		return mTranslate * mRotate /** mScale*/ * mSize;
	}
}
