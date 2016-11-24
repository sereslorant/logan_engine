#ifndef L_GOO_SVG_ELLIPSE_ATTRIBUTES
#define L_GOO_SVG_ELLIPSE_ATTRIBUTES

#include "../lGooSvgGlobal.h"

#include <string>

struct lGooSvgEllipseAttributes
{
	float Attr_cx;
	float Attr_cy;
	float Attr_rx;
	float Attr_ry;


	typedef void (lGooSvgEllipseAttributes::* EllipseAttribLoader)(std::string &value);

	void Load_cx(std::string &value);
	void Load_cy(std::string &value);
	void Load_rx(std::string &value);
	void Load_ry(std::string &value);


	EllipseAttribLoader AttribLoaders[4] = {&lGooSvgEllipseAttributes::Load_cx,
											&lGooSvgEllipseAttributes::Load_cy,
											&lGooSvgEllipseAttributes::Load_rx,
											&lGooSvgEllipseAttributes::Load_ry
											};


	void LoadAttribs(const std::string &attribute,std::string &value);
	lGooSvgEllipseAttributes();
};

#endif
