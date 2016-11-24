
#include "../../lGooAnimation/lGooAnimate/lGooRectAnimate.h"

#include "lGooSvgRect.h"
#include "lGooSvgRoot.h"

#include "../lGooSVG.h"

const std::map<std::string,lGooRect::lGooRectAttrib> RectAttribs = {{"x",{&lGooRect::SetX,&lGooRect::MoveX,&lGooRect::GetX}},
																	{"y",{&lGooRect::SetY,&lGooRect::MoveY,&lGooRect::GetY}},
																	{"width",{&lGooRect::SetWidth,&lGooRect::IncreaseWidth,&lGooRect::GetWidth}},
																	{"height",{&lGooRect::SetHeight,&lGooRect::IncreaseHeight,&lGooRect::GetHeight}},
																	};

const lGooRect::lGooRectAttrib *GetRectAttrib(const std::string &attrib_name)
{
	auto I = RectAttribs.find(attrib_name);

	if(I != RectAttribs.end())
	{
		return &I->second;
	}
	else
	{
		return nullptr;
	}
}

lGooScalarAnimateParam::liGooScalarAnimState *lGooSvgRect::LoadShapeAttribAnim(lGooSvgAnim *anim)
{
	const lGooRect::lGooRectAttrib *RectAttrib = GetRectAttrib(anim->GetAnimAttributes().Attr_attributeName);
	if(RectAttrib == nullptr)
	{
		return nullptr;
	}

	lGooScalarAnimateParam::liGooScalarAnimState *AnimState = new lGooRectAnimateParam((lGooRect *)NewObject,*RectAttrib);
	return AnimState;
}

void lGooSvgRect::LoadAttribs(const std::string &attribute,std::string &value)
{
	RectAttributes.LoadAttribs(attribute,value);
	lGooSvgShape::LoadAttribs(attribute,value);
}

void lGooSvgRect::ProcessAttribs()
{
	lGooSvgShape::ProcessAttribs();
}

void lGooSvgRect::CreateObject(liGooObject *&obj)
{
	std::cout << "Creating Rect" << std::endl;

	NewObject = ObjectFactory->NewRect();/*lmVector2D({RectAttributes.Attr_x,RectAttributes.Attr_y}),
										lmVector2D({1.0,1.0}),0.0,
										RectAttributes.Attr_width,RectAttributes.Attr_height);*/
	NewObject->SetPosition({RectAttributes.Attr_x,RectAttributes.Attr_y});

	PaintingAttributes.SetPaintingAttributes(NewObject,ObjectFactory);

	liGooRect *NewRect = NewObject->ToRect();
	NewRect->SetWidth(RectAttributes.Attr_width);
	NewRect->SetHeight(RectAttributes.Attr_height);

	for(auto I : Animates)
	{
		LoadAnim(I,NewObject);
	}

	obj = NewObject;
	NewObject = nullptr;
}

lGooSvgRect::lGooSvgRect(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root)
	:lGooSvgShape(ref_file,"rect",svg_root,object_factory)
{

}

lGooSvgRect::~lGooSvgRect()
{

}
