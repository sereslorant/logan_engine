
#include "lGooSvgEllipseAttributes.h"
#include "../lGooSVG.h"

void lGooSvgEllipseAttributes::Load_cx(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_cx;

	lGooSVG::ParseLengthDimension(value,Dim_cx);
	Attr_cx	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_cx);

	std::cout << "cx=" << Attr_cx << std::endl;
}

void lGooSvgEllipseAttributes::Load_cy(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_cy;

	lGooSVG::ParseLengthDimension(value,Dim_cy);
	Attr_cy	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_cy);

	std::cout << "cy=" << Attr_cy << std::endl;
}

void lGooSvgEllipseAttributes::Load_rx(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_rx;

	lGooSVG::ParseLengthDimension(value,Dim_rx);
	Attr_rx	= lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_rx);

	std::cout << "rx=" << Attr_rx << std::endl;
}

void lGooSvgEllipseAttributes::Load_ry(std::string &value)
{
	SVG_LENGTH_DIMENSION Dim_ry;

	lGooSVG::ParseLengthDimension(value,Dim_ry);
	Attr_ry = lGooSVG::SetValueDimension(lGooSVG::ParseNumber(value),Dim_ry);

	std::cout << "ry=" << Attr_ry << std::endl;
}

void lGooSvgEllipseAttributes::LoadAttribs(const std::string &attribute,std::string &value)
{
	for(int i=0;i < SVG_ELLIPSE_ATTRIBS_SIZE;i++)
	{
		if(attribute == SVG_EllipseAttribs[i])
		{
			(this->*AttribLoaders[i])(value);
		}
	}
}

lGooSvgEllipseAttributes::lGooSvgEllipseAttributes()
{
	Attr_cx = 0.0,
	Attr_cy = 0.0,
	Attr_rx	= 0.0,
	Attr_ry	= 0.0;
}
