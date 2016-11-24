#ifndef L_GOO_SVG_LINE_ATTRIBUTES
#define L_GOO_SVG_LINE_ATTRIBUTES

#include "../lGooSvgGlobal.h"

#include <string>

struct lGooSvgLineAttributes
{
	float Attr_x1;
	float Attr_y1;
	float Attr_x2;
	float Attr_y2;


	typedef void (lGooSvgLineAttributes::* LineAttribLoader)(std::string &value);

	void Load_x1(std::string &value);
	void Load_y1(std::string &value);
	void Load_x2(std::string &value);
	void Load_y2(std::string &value);

	LineAttribLoader AttribLoaders[4] = {&lGooSvgLineAttributes::Load_x1,
										 &lGooSvgLineAttributes::Load_y1,
										 &lGooSvgLineAttributes::Load_x2,
										 &lGooSvgLineAttributes::Load_y2
										 };

	void LoadAttribs(const std::string &attribute,std::string &value);

	lGooSvgLineAttributes();
};

#endif // L_GOO_SVG_LINE_ATTRIBUTES
