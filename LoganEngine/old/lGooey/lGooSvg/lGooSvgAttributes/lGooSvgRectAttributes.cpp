
#include "lGooSvgRectAttributes.h"
#include "../lGooSVG.h"

void lGooSvgRectAttributes::Load_x(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_x;

	lGooSVG::ParseLengthDimension(value,Dim_x);
	Attr_x	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_x);

	std::cout << "x=" << Attr_x << std::endl;
}

void lGooSvgRectAttributes::Load_y(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_y;

	lGooSVG::ParseLengthDimension(value,Dim_y);
	Attr_y	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_y);

	std::cout << "y=" << Attr_x << std::endl;
}

void lGooSvgRectAttributes::Load_width(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_width;

	lGooSVG::ParseLengthDimension(value,Dim_width);
	Attr_width	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_width);

	std::cout << "width=" << Attr_x << std::endl;
}

void lGooSvgRectAttributes::Load_height(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_height;

	lGooSVG::ParseLengthDimension(value,Dim_height);
	Attr_height = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_height);

	std::cout << "height=" << Attr_x << std::endl;
}

void lGooSvgRectAttributes::LoadAttribs(const std::string &attribute,std::string &value)
{
	for(int i=0;i < SVG_RECT_ATTRIBS_SIZE;i++)
	{
		if(attribute == SVG_RectAttribs[i])
		{
			(this->*AttribLoaders[i])(value);
		}
	}
}

lGooSvgRectAttributes::lGooSvgRectAttributes()
{
	Attr_x = 0.0;
	Attr_y = 0.0;
	Attr_width	= 0.0;
	Attr_height	= 0.0;
}
