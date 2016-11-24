
#include "lGooSvgShape.h"

#include "lGooSvgRoot.h"
#include "../lGooSVG.h"

lGooAnimate::liGooAnimateParam *lGooSvgShape::CreateScalarAnimate(lGooSvgAnim *anim,lmScalar duration)
{
	lGooScalarAnimateParam::liGooScalarAnimState *AnimState;
	AnimState = LoadShapeAttribAnim(anim);
	if(AnimState == nullptr)
	{
		return nullptr;
	}

	lmScalar BeginValue;
	SVG_LENGTH_DIMENSION Dim_BeginValue;

	lmScalar EndValue;
	SVG_LENGTH_DIMENSION Dim_EndValue;

	switch(anim->GetAnimAttributes().AnimType)
	{
	case lGooSvgAnimAttributes::ANIM_FROM_TO:

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_from,Dim_BeginValue);
		BeginValue = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_from),
												Dim_BeginValue);

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_to,Dim_EndValue);
		EndValue   = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_to),
												Dim_EndValue);

		break;

	case lGooSvgAnimAttributes::ANIM_FROM_BY:

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_from,Dim_BeginValue);
		BeginValue = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_from),
												Dim_BeginValue);

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_by,Dim_EndValue);
		EndValue  = BeginValue + lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_by),
															Dim_EndValue);
		break;

	case lGooSvgAnimAttributes::ANIM_TO:
		BeginValue = AnimState->GetParam();

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_to,Dim_EndValue);
		EndValue   = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_to),
												Dim_EndValue);

		break;

	case lGooSvgAnimAttributes::ANIM_BY:
		BeginValue = AnimState->GetParam();

		lGooSVG::ParseLengthDimension(anim->GetAnimAttributes().Attr_by,Dim_EndValue);
		EndValue = BeginValue + lGooSVG::SetValueDimension(lGooSVG::ParseNumber(anim->GetAnimAttributes().Attr_by),
															Dim_EndValue);

		break;
	}

	lGooScalarAnimateParam *AnimateParam = new lGooScalarAnimateParam(AnimState,BeginValue,EndValue,duration);
	return AnimateParam;
}

bool lGooSvgShape::LoadAnim(lGooSvgAnim *anim,liGooObject *object)
{
	lmScalar Duration		= anim->GetAnimAttributes().Attr_dur;
	lmScalar RepeatCount	= anim->GetAnimAttributes().Attr_repeatCount;
	lmScalar RepeatDuration	= anim->GetAnimAttributes().Attr_repeatDur;

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

	lGooAnimate::liGooAnimateParam *AnimateParam;
	AnimateParam = CreateScalarAnimate(anim,Duration);
	if(AnimateParam == nullptr)
	{
		return false;
	}

	lGooAnimate *Animate = new lGooAnimate(AnimateParam,Duration,Repeat,AnimFill);
	object->AddAnimation(Animate);

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

	std::cout << std::endl;

	if(anim->HasId)
	{
		if(SvgRoot->AnimIds.find(anim->Id) == SvgRoot->AnimIds.end())
		{
			SvgRoot->AnimIds[anim->Id] = Animate;

			std::cout << "Id: " << anim->Id << std::endl;
		}

		object->AddAnimationId(anim->Id,Animate);
	}

	return true;
}

void lGooSvgShape::LoadAttribs(const std::string &attribute,std::string &value)
{
	PaintingAttributes.LoadAttribs(attribute,value);
}

void lGooSvgShape::ProcessAttribs()
{
	//PaintingAttributes.SetPaintingAttributes(GetObject());
}

void lGooSvgShape::LoadChildElement(std::ifstream &file,const std::string &Tag)
{
	if(Tag == "animate")
	{
		lGooSvgAnim *SvgAnim = new lGooSvgAnim(file);
		SvgAnim->LoadElement();

		Animates.push_back(SvgAnim);
	}
}

lGooSvgShape::lGooSvgShape(std::ifstream &ref_file,const std::string &xml_tag,lGooSvgRoot *svg_root,liGooObjectFactory *object_factory)
	:XmlElement(ref_file,xml_tag),SvgRoot(svg_root),ObjectFactory(object_factory)
{

}

lGooSvgShape::~lGooSvgShape()
{
	for(auto I : Animates)
	{
		delete I;
	}
}
