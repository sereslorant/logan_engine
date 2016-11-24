
#include "../../lGooAnimation/lGooAnimate/lGooLineAnimate.h"

#include "lGooSvgLine.h"
#include "lGooSvgRoot.h"

#include "../lGooSVG.h"

const std::map<std::string,lGooLine::lGooLineAttrib> LineAttribs = {{"x1",{&lGooLine::SetX,&lGooLine::MoveX,&lGooLine::GetX}},
																	{"y1",{&lGooLine::SetY,&lGooLine::MoveY,&lGooLine::GetY}},
																	{"x2",{&lGooLine::SetVX,&lGooLine::IncreaseVX,&lGooLine::GetVX}},
																	{"y2",{&lGooLine::SetVY,&lGooLine::IncreaseVY,&lGooLine::GetVY}},
																	};

const lGooLine::lGooLineAttrib *GetLineAttrib(const std::string &attrib_name)
{
	auto I = LineAttribs.find(attrib_name);

	if(I != LineAttribs.end())
	{
		return &I->second;
	}
	else
	{
		return nullptr;
	}
}

lGooScalarAnimateParam::liGooScalarAnimState *lGooSvgLine::LoadShapeAttribAnim(lGooSvgAnim *anim)
{
	const lGooLine::lGooLineAttrib *LineAttrib = GetLineAttrib(anim->GetAnimAttributes().Attr_attributeName);
	if(LineAttrib == nullptr)
	{
		return nullptr;
	}

	lGooScalarAnimateParam::liGooScalarAnimState *AnimState = new lGooLineAnimateParam((lGooLine *)NewObject,*LineAttrib);
	return AnimState;
}

void lGooSvgLine::LoadAttribs(const std::string &attribute,std::string &value)
{
	LineAttributes.LoadAttribs(attribute,value);
	lGooSvgShape::LoadAttribs(attribute,value);
}

void lGooSvgLine::ProcessAttribs()
{
	lGooSvgShape::ProcessAttribs();
}

void lGooSvgLine::CreateObject(liGooObject *&obj)
{
	std::cout << "Creating Line" << std::endl;

	NewObject = ObjectFactory->NewLine();
								/*lmVector2D({LineAttributes.Attr_x1,LineAttributes.Attr_y1}),
								lmVector2D({1.0,1.0}),0.0,
								lmVector2D({LineAttributes.Attr_x2,LineAttributes.Attr_y2}));*/
	NewObject->SetPosition({LineAttributes.Attr_x1,LineAttributes.Attr_y1});

	PaintingAttributes.SetPaintingAttributes(NewObject,ObjectFactory);

	liGooLine *NewLine = NewObject->ToLine();
	NewLine->SetV2({LineAttributes.Attr_x2,LineAttributes.Attr_y2});

	for(auto I : Animates)
	{
		LoadAnim(I,NewObject);
	}

	obj = NewObject;
	NewObject = nullptr;
}

lGooSvgLine::lGooSvgLine(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root)
	:lGooSvgShape(ref_file,"line",svg_root,object_factory)
{

}

lGooSvgLine::~lGooSvgLine()
{

}
