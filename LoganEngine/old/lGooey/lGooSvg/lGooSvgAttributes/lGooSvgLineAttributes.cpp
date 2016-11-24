
#include "lGooSvgLineAttributes.h"
#include "../lGooSVG.h"

void lGooSvgLineAttributes::Load_x1(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_x1;

	lGooSVG::ParseLengthDimension(value ,Dim_x1);
	Attr_x1	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_x1);

	std::cout << "x1="<<Attr_x1<<std::endl;
}

void lGooSvgLineAttributes::Load_y1(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_y1;

	lGooSVG::ParseLengthDimension(value ,Dim_y1);
	Attr_y1	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_y1);

	std::cout << "y1="<<Attr_y1<<std::endl;
}

void lGooSvgLineAttributes::Load_x2(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_x2;

	lGooSVG::ParseLengthDimension(value ,Dim_x2);
	Attr_x2	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_x2);

	std::cout << "x2="<<Attr_x2<<std::endl;
}

void lGooSvgLineAttributes::Load_y2(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_y2;

	lGooSVG::ParseLengthDimension(value ,Dim_y2);
	Attr_y2	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_y2);

	std::cout << "y2="<<Attr_y2<<std::endl;
}

void lGooSvgLineAttributes::LoadAttribs(const std::string &attribute,std::string &value)
{
	for(int i=0;i < SVG_LINE_ATTRIBS_SIZE;i++)
	{
		if(attribute == SVG_LineAttribs[i])
		{
			(this->*AttribLoaders[i])(value);
		}
	}
}

lGooSvgLineAttributes::lGooSvgLineAttributes()
{
	Attr_x1 = 0.0,
	Attr_y1 = 0.0,
	Attr_x2 = 0.0,
	Attr_y2 = 0.0;
}

