
#include "lGooSvgCircleAttributes.h"
#include "../lGooSVG.h"

void lGooSvgCircleAttributes::Load_cx(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_cx;

	lGooSVG::ParseLengthDimension(value,Dim_cx);
	Attr_cx	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_cx);

	std::cout << "cx=" << Attr_cx << std::endl;
}

void lGooSvgCircleAttributes::Load_cy(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_cy;

	lGooSVG::ParseLengthDimension(value,Dim_cy);
	Attr_cy	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_cy);

	std::cout << "cy=" << Attr_cy << std::endl;
}

void lGooSvgCircleAttributes::Load_r(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_r;

	lGooSVG::ParseLengthDimension(value,Dim_r);
	Attr_r	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_r);

	std::cout << "r=" << Attr_r << std::endl;
}

void lGooSvgCircleAttributes::LoadAttribs(const std::string &attribute,std::string &value)
{
	for(int i=0;i < SVG_CIRCLE_ATTRIBS_SIZE;i++)
	{
		if(attribute == SVG_CircleAttribs[i])
		{
			(this->*AttribLoaders[i])(value);
		}
	}
}

lGooSvgCircleAttributes::lGooSvgCircleAttributes()
{
	Attr_cx = 0.0,
	Attr_cy = 0.0,
	Attr_r	= 0.0;
}
