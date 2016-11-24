#ifndef L_GOO_SVG_CIRCLE_ATTRIBUTES
#define L_GOO_SVG_CIRCLE_ATTRIBUTES

#include "../lGooSvgGlobal.h"

#include <string>

struct lGooSvgCircleAttributes
{
	float Attr_cx;
	float Attr_cy;
	float Attr_r;


	typedef void (lGooSvgCircleAttributes::* CircleAttribLoader)(std::string &value);

	void Load_cx(std::string &value);
	void Load_cy(std::string &value);
	void Load_r(std::string &value);

	CircleAttribLoader AttribLoaders[3] = {&lGooSvgCircleAttributes::Load_cx,
										   &lGooSvgCircleAttributes::Load_cy,
										   &lGooSvgCircleAttributes::Load_r
										   };

	void LoadAttribs(const std::string &attribute,std::string &value);

	lGooSvgCircleAttributes();
};

#endif
