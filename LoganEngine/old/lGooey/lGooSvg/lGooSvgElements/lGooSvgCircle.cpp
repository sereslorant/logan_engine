
#include "../../lGooAnimation/lGooAnimate/lGooCircleAnimate.h"

#include "lGooSvgCircle.h"
#include "lGooSvgRoot.h"

#include "../lGooSVG.h"

const std::map<std::string,lGooCircle::lGooCircleAttrib> CircleAttribs = {{"cx",{&lGooCircle::SetX,&lGooCircle::MoveX,&lGooCircle::GetX}},
																		  {"cy",{&lGooCircle::SetY,&lGooCircle::MoveY,&lGooCircle::GetY}},
																		  {"r" ,{&lGooCircle::SetRadius,&lGooCircle::IncreaseRadius,&lGooCircle::GetRadius}},
																		  };

const lGooCircle::lGooCircleAttrib *GetCircleAttrib(const std::string &attrib_name)
{
	auto I = CircleAttribs.find(attrib_name);

	if(I != CircleAttribs.end())
	{
		return &I->second;
	}
	else
	{
		return nullptr;
	}
}
/*
bool lGooSvgCircle::LoadCircleAnim(lGooSvgAnim *anim)
{

	lmScalar Duration	= anim->GetAnimAttributes().Attr_dur;
	lmScalar Repeat		= anim->GetAnimAttributes().Attr_repeatCount * Duration;

	lGooAnimate::lGooAnimFill AnimFill;

	switch(anim->GetAnimAttributes().Attr_fill)
	{
	case lGooSvgAnimAttributes::F_FREEZE:
		AnimFill = lGooAnimate::FREEZE;
		break;
	case lGooSvgAnimAttributes::F_REMOVE:
		AnimFill = lGooAnimate::REMOVE;
		break;
	}

	const lGooCircle::lGooCircleAttrib *CircleAttrib = GetCircleAttrib(anim->GetAnimAttributes().Attr_attributeName);
	if(CircleAttrib == nullptr)
	{
		return false;
	}

	lGooScalarAnimateParam::liGooScalarAnimState *AnimState = new lGooCircleAnimateParam(NewCircle,*CircleAttrib);


	lmScalar BeginValue;
	SVG_LENGTH_DIMENSION Dim_BeginValue;

	lmScalar EndValue;
	SVG_LENGTH_DIMENSION Dim_EndValue;

	switch(anim->GetAnimAttributes().AnimType)
	{
	case lGooSvgAnimAttributes::ANIM_FROM_TO:

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_from,Dim_BeginValue);
		BeginValue	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_from),
												 Dim_BeginValue);

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_to,Dim_EndValue);
		EndValue	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_to),
												 Dim_EndValue);

		break;

	case lGooSvgAnimAttributes::ANIM_FROM_BY:

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_from,Dim_BeginValue);
		BeginValue	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_from),
												 Dim_BeginValue);

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_by,Dim_EndValue);
		EndValue	= BeginValue + lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_by),
															  Dim_EndValue);

		break;

	case lGooSvgAnimAttributes::ANIM_TO:
		BeginValue = AnimState->GetParam();

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_to,Dim_EndValue);
		EndValue = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_to),
											  Dim_EndValue);

		break;

	case lGooSvgAnimAttributes::ANIM_BY:
		BeginValue = AnimState->GetParam();

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_by,Dim_EndValue);
		EndValue = BeginValue + lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_by),
														   Dim_EndValue);

		break;
	}

	lGooScalarAnimateParam *AnimateParam = new lGooScalarAnimateParam(AnimState,BeginValue,EndValue,Duration);

	lGooAnimate *Animate = new lGooAnimate(AnimateParam,Duration,Repeat,AnimFill);
	NewCircle->AddAnimation(Animate);

	SvgRoot->Animations.push_back({anim,Animate});

	std::cout << "New anim: " << std::endl;

	std::cout << "begin: " << std::endl;

	for(auto I : anim->GetAnimAttributes().Attr_begin)
	{
		std::cout << I << ";";
	}

	std::cout << "\nend: " << std::endl;

	for(auto I : anim->GetAnimAttributes().Attr_end)
	{
		std::cout << I << ";";
	}

	if(anim->HasId)
	{
		if(SvgRoot->AnimIds.find(anim->Id) == SvgRoot->AnimIds.end())
		{
			SvgRoot->AnimIds[anim->Id] = Animate;

			std::cout << "Id: " << anim->Id << std::endl;
		}

		NewCircle->AddAnimationId(anim->Id,Animate);
	}

	return true;
}
*/
lGooScalarAnimateParam::liGooScalarAnimState *lGooSvgCircle::LoadShapeAttribAnim(lGooSvgAnim *anim)
{
	const lGooCircle::lGooCircleAttrib *CircleAttrib = GetCircleAttrib(anim->GetAnimAttributes().Attr_attributeName);
	if(CircleAttrib == nullptr)
	{
		return nullptr;
	}

	lGooScalarAnimateParam::liGooScalarAnimState *AnimState = new lGooCircleAnimateParam((lGooCircle *)NewObject,*CircleAttrib);
	return AnimState;
}

void lGooSvgCircle::LoadAttribs(const std::string &attribute,std::string &value)
{
	CircleAttributes.LoadAttribs(attribute,value);
	lGooSvgShape::LoadAttribs(attribute,value);
}

void lGooSvgCircle::ProcessAttribs()
{
	/*NewCircle = Renderer->NewCircle(lmVector2D(CircleAttributes.Attr_cx,CircleAttributes.Attr_cy),
							 lmVector2D(1.0,1.0),0.0,
							 CircleAttributes.Attr_r);*/

	lGooSvgShape::ProcessAttribs();
}
/*
void lGooSvgCircle::ProcessElement()
{
	/*SvgRoot.GetSvg()->AddObject(NewCircle);

	if(HasId)
	{
		SvgRoot.GetSvg()->AddObjectId(Id,NewCircle);
	}* /
}
*/
void lGooSvgCircle::CreateObject(liGooObject *&obj)
{
	std::cout << "Creating Circle" << std::endl;

	NewObject = ObjectFactory->NewCircle();
							/*lmVector2D({CircleAttributes.Attr_cx,CircleAttributes.Attr_cy}),
							 lmVector2D({1.0,1.0}),0.0,
							 CircleAttributes.Attr_r);*/
	NewObject->SetPosition({CircleAttributes.Attr_cx,CircleAttributes.Attr_cy});

	liGooCircle *NewCircle = NewObject->ToCircle();
	NewCircle->SetRadius(CircleAttributes.Attr_r);

	PaintingAttributes.SetPaintingAttributes(NewObject,ObjectFactory);

	for(auto I : Animates)
	{
		LoadAnim(I,NewObject);
	}

	obj = NewObject;
	NewObject = nullptr;
}

lGooSvgCircle::lGooSvgCircle(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root)
	:lGooSvgShape(ref_file,"circle",svg_root,object_factory)//,SvgRoot(svg_root)
{

}

lGooSvgCircle::~lGooSvgCircle()
{

}
