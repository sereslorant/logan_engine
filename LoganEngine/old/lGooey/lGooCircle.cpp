
#include "lGooCircle.h"

/*lGooCircle::lGooCircle():lGooObject(L_GOO_CIRCLE),Radius(1.0)
{

}*/

lGooCircle::lGooCircle(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon,lmScalar radius):lGooObject(pos,scale,orientation,polygon),Radius(radius)
{

}

lGooCircle::~lGooCircle()
{

}

void lGooCircle::SetRadius(lmScalar radius)
{
    Radius = radius;
}

lmMatrix3x3 lGooCircle::Transform() const
{
    lmMatrix3x3 mTranslate  = lmTranslate3x3(Position);
    lmMatrix3x3 mRotate     = lmRotate3x3(lmDegToRad(Orientation));
    lmMatrix3x3 mScale      = lmScale3x3(Scale*Radius);

    if(Parent != nullptr)
	{
		return Parent->Transform() * mTranslate * mRotate * mScale;
	}
	else
	{
		return mTranslate * mRotate * mScale;
	}
}


