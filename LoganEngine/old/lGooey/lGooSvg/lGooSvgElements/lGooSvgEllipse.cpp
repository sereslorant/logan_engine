
#include "../../lGooAnimation/lGooAnimate/lGooEllipseAnimate.h"

#include "lGooSvgEllipse.h"
#include "lGooSvgRoot.h"

#include "../lGooSVG.h"

const std::map<std::string,lGooEllipse::lGooEllipseAttrib> EllipseAttribs = {{"cx",{&lGooEllipse::SetX,&lGooEllipse::MoveX,&lGooEllipse::GetX}},
																			 {"cy",{&lGooEllipse::SetY,&lGooEllipse::MoveY,&lGooEllipse::GetY}},
																			 {"rx",{&lGooEllipse::SetRadiusX,&lGooEllipse::IncreaseRadiusX,&lGooEllipse::GetRadiusX}},
																			 {"ry",{&lGooEllipse::SetRadiusY,&lGooEllipse::IncreaseRadiusY,&lGooEllipse::GetRadiusY}},
																			 };

const lGooEllipse::lGooEllipseAttrib *GetEllipseAttrib(const std::string &attrib_name)
{
	auto I = EllipseAttribs.find(attrib_name);

	if(I != EllipseAttribs.end())
	{
		return &I->second;
	}
	else
	{
		return nullptr;
	}
}

lGooScalarAnimateParam::liGooScalarAnimState *lGooSvgEllipse::LoadShapeAttribAnim(lGooSvgAnim *anim)
{
	const lGooEllipse::lGooEllipseAttrib *EllipseAttrib = GetEllipseAttrib(anim->GetAnimAttributes().Attr_attributeName);
	if(EllipseAttrib == nullptr)
	{
		return nullptr;
	}

	lGooScalarAnimateParam::liGooScalarAnimState *AnimState = new lGooEllipseAnimateParam((lGooEllipse *)NewObject,*EllipseAttrib);
	return AnimState;
}

void lGooSvgEllipse::LoadAttribs(const std::string &attribute,std::string &value)
{
	EllipseAttributes.LoadAttribs(attribute,value);
	lGooSvgShape::LoadAttribs(attribute,value);
}

void lGooSvgEllipse::ProcessAttribs()
{
	lGooSvgShape::ProcessAttribs();
}

void lGooSvgEllipse::CreateObject(liGooObject *&obj)
{
	std::cout << "Creating Ellipse" << std::endl;

	NewObject = ObjectFactory->NewEllipse();	/*lmVector2D({EllipseAttributes.Attr_cx,EllipseAttributes.Attr_cy}),
										lmVector2D({1.0,1.0}),0.0,
										EllipseAttributes.Attr_rx,EllipseAttributes.Attr_ry);
										*/
	NewObject->SetPosition({EllipseAttributes.Attr_cx,EllipseAttributes.Attr_cy});

	PaintingAttributes.SetPaintingAttributes(NewObject,ObjectFactory);

	liGooEllipse *NewEllipse = NewObject->ToEllipse();
	NewEllipse->SetRadiusX(EllipseAttributes.Attr_rx);
	NewEllipse->SetRadiusY(EllipseAttributes.Attr_ry);

	for(auto I : Animates)
	{
		LoadAnim(I,NewObject);
	}

	obj = NewObject;
	NewObject = nullptr;
}

lGooSvgEllipse::lGooSvgEllipse(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root)
	:lGooSvgShape(ref_file,"ellipse",svg_root,object_factory)
{

}

lGooSvgEllipse::~lGooSvgEllipse()
{

}
