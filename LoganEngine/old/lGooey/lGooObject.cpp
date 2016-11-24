
#include "lGooObject.h"
#include "lGooRenderer.h"

using namespace std;
/*
lGooObject::lGooObject(lGooObjectType type):Pos(0.0,0.0),Scale(1.0,1.0),Orientation(0.0),Outline(false),Image("Default"),Type(type),Color(0.0,0.0,0.0,1.0),OutlineColor(0.0,0.0,0.0,1.0)
{

}
*/
lGooObject::lGooObject(const lmVector2D &pos,const lmVector2D &scale,lmScalar orientation,liGuiElement *polygon)
    :Position(pos),Scale(scale),Orientation(orientation)/*,Type(type)*/,Color(0.0,0.0,0.0,1.0),Textured(false),Outline(false),OutlineColor(0.0,0.0,0.0,1.0),OutlineWidth(1.0),Polygon(polygon),Parent(nullptr)
{

}

lGooObject::~lGooObject()
{
	for(auto I : Animations)
	{
		delete I;
	}
}
/*
void lGooObject::MoveV(const lmVector2D &d)
{
    Pos += d;
}

void lGooObject::ScaleV(const lmVector2D &scale)
{
    Scale += scale;
}
*/
void lGooObject::MoveX(lmScalar dX)
{
    Position[0] += dX;
}

void lGooObject::MoveY(lmScalar dY)
{
    Position[1] += dY;
}

void lGooObject::IncreaseScaleX(lmScalar X)
{
    Scale[0] += X;
}

void lGooObject::IncreaseScaleY(lmScalar Y)
{
    Scale[1] += Y;
}
/*
void lGooObject::Rotate(lmScalar Alpha)
{
    Orientation += Alpha;
}
*/
void lGooObject::SetX(lmScalar X)
{
    Position[0] = X;
}
void lGooObject::SetY(lmScalar Y)
{
    Position[1] = Y;
}

void lGooObject::SetScaleX(lmScalar X)
{
    Scale[0] = X;
}

void lGooObject::SetScaleY(lmScalar Y)
{
    Scale[1] = Y;
}
/*
void lGooObject::SetOrientation(lmScalar Alpha)
{
    Orientation = Alpha;
}
*/
/*
lGooObjectType lGooObject::GetType() const
{
    return Type;
}*/
/*
bool lGooObject::GetOutline() const
{
    return Outline;
}

const string &lGooObject::GetImage() const
{
    return Image;
}
*//*
void lGooObject::AddToRenderer(lGooRenderer *Renderer)
{
    Renderer->Add(this);
}
*/
