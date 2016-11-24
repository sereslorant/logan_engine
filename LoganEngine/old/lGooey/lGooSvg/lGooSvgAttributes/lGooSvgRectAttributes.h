#ifndef L_GOO_SVG_RECT_ATTRIBUTES
#define L_GOO_SVG_RECT_ATTRIBUTES

#include "../lGooSvgGlobal.h"

struct lGooSvgRectAttributes
{
	float Attr_x;
	float Attr_y;
	float Attr_width;
	float Attr_height;

	typedef void (lGooSvgRectAttributes::* RectAttribLoader)(std::string &value);

	void Load_x(std::string &value);
	void Load_y(std::string &value);
	void Load_width(std::string &value);
	void Load_height(std::string &value);

	RectAttribLoader AttribLoaders[4] = {&lGooSvgRectAttributes::Load_x,
										 &lGooSvgRectAttributes::Load_y,
										 &lGooSvgRectAttributes::Load_width,
										 &lGooSvgRectAttributes::Load_height
										 };

	void LoadAttribs(const std::string &attribute,std::string &value);
	lGooSvgRectAttributes();
};

#endif
