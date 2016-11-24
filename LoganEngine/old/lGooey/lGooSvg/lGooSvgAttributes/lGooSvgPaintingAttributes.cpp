
#include "lGooSvgPaintingAttributes.h"
#include "../lGooSVG.h"

void lGooSvgPaintingAttributes::Load_stroke(std::string &value)
{
	StrokeFound = true;
	lGooSVG::ParseColor(value,Attr_stroke);

	std::cout << "stroke=("<<Attr_stroke.GetRed() << ";" << Attr_stroke.GetGreen() << ";" << Attr_stroke.GetBlue() << ")" <<std::endl;
}

void lGooSvgPaintingAttributes::Load_stroke_width(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_stroke_width;

	lGooSVG::ParseLengthDimension(value,Dim_stroke_width);
	Attr_stroke_width	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_stroke_width);

	std::cout << "stroke-width="<<Attr_stroke_width<<std::endl;
}

void lGooSvgPaintingAttributes::Load_stroke_opacity(std::string &value)
{
	float Alpha = lGooSVG::ParseNumber(value);
	Alpha = lmClamp((float)0.0,(float)1.0,Alpha);

	Attr_stroke.SetAlpha(Alpha);

	std::cout << "stroke-opacity="<<Attr_stroke.GetAlpha() <<std::endl;
}

void lGooSvgPaintingAttributes::Load_fill(std::string &value)
{
	lGooSVG::ParseColor(value,Attr_fill);

	std::cout << "fill=("<<Attr_fill.GetRed() << ";" << Attr_fill.GetGreen() << ";" << Attr_fill.GetBlue() << ")" <<std::endl;
}

void lGooSvgPaintingAttributes::Load_fill_opacity(std::string &value)
{
	float Alpha = lGooSVG::ParseNumber(value);
	Alpha = lmClamp((float)0.0,(float)1.0,Alpha);

	Attr_fill.SetAlpha(Alpha);
}

void lGooSvgPaintingAttributes::LoadAttribs(const std::string &attribute,std::string &value/*,SVG_LENGTH_DIMENSION &value_dimension*/)
{
	///TODO: FILL RULE-T HOZZÁ KELL ADNI!
	//http://www.w3.org/TR/SVG2/painting.html#Introduction

	for(int i=0;i < SVG_PAINTING_ATTRIBS_SIZE;i++)
	{
		if(attribute == SVG_PaintingAttribs[i])
		{
			(this->*AttribLoaders[i])(value);
		}
	}
}

void lGooSvgPaintingAttributes::SetPaintingAttributes(liGooObject *obj,liGooObjectFactory *object_factory)
{
	obj->SetColor(Attr_fill);
	liGuiBrush *FillBrush = object_factory->GetRenderer()->CreateSolidBrush(&obj->GetSolidColor());
	//FillBrush->SetColor(obj->GetColor());

	obj->SetFillBrush(FillBrush);

	if(StrokeFound)
	{
		obj->SetOutline(true);
		obj->SetOutlineColor(Attr_stroke);

		liGuiBrush *StrokeBrush = object_factory->GetRenderer()->CreateSolidBrush(&obj->GetSolidOutline());
		//StrokeBrush->SetColor(obj->GetOutlineColor());

		obj->SetStrokeBrush(StrokeBrush);

		obj->SetOutlineWidth(Attr_stroke_width);
	}
}

lGooSvgPaintingAttributes::lGooSvgPaintingAttributes()
	:Attr_stroke_width(0.0),StrokeFound(false)
{
	Attr_fill.SetAlpha(1.0);
	Attr_stroke.SetAlpha(1.0);
}
