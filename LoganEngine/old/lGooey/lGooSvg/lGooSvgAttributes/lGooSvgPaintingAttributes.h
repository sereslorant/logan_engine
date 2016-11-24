#ifndef L_GOO_SVG_PAINTING_ATTRIBUTES_H
#define L_GOO_SVG_PAINTING_ATTRIBUTES_H

#include "../lGooSvgGlobal.h"

#include <string>

#include "../../liGooObjectFactory.h"
#include "../../lGooObject.h"

struct lGooSvgPaintingAttributes
{
	lGooColor Attr_fill;

	bool StrokeFound; //Nyilván kell tartanunk, hogy van-e körvonal, vagy nincs.
	lGooColor Attr_stroke;
	float Attr_stroke_width;

	typedef void (lGooSvgPaintingAttributes::* PaintingAttribLoader)(std::string &value);

	//Stroke propertyk betöltése
	void Load_stroke(std::string &value);
	void Load_stroke_width(std::string &value);
	void Load_stroke_opacity(std::string &value);
	//void Load_stroke_linecap(std::string &value);		//Három értéket vehet majd fel:  butt || round || square
	//void Load_stroke_linejoin(std::string &value);	//Három értéket vehet majd fel: miter || miter-clip || round || bevel || arcs
	//void Load_stroke_miterlimit(std::string &value);	// Number
	//void Load_stroke_dasharray(std::string &value);	// none || <dasharray>
	//void Load_stroke_dashoffset(std::string &value);	// <length> | <percentage>

	//Fill propertyk betöltése
	void Load_fill(std::string &value);
	//void Load_fill_rule(std::string &value); //Két értéket vehet majd fel: nonzero || evenodd
	void Load_fill_opacity(std::string &value);

	//Paint order propertyk betöltése
	//void Load_paint_order(std::string &value); //normal | [ fill || stroke || markers ] Default: normal, ami fill,stroke,markers

	//Color space-k interpolationjének betöltése
	//void Load_color_interpolation(std::string &value); //Az alábbi értékeket veheti fel: auto | sRGB | linearRGB Default: sRGB

	PaintingAttribLoader AttribLoaders[5] = {&lGooSvgPaintingAttributes::Load_stroke,
											 &lGooSvgPaintingAttributes::Load_stroke_width,
											 &lGooSvgPaintingAttributes::Load_stroke_opacity,
											 &lGooSvgPaintingAttributes::Load_fill,
											 &lGooSvgPaintingAttributes::Load_fill_opacity
											 };

	void LoadAttribs(const std::string &attribute,std::string &value);

	void SetPaintingAttributes(liGooObject *obj,liGooObjectFactory *object_factory);

	lGooSvgPaintingAttributes();
};

#endif // L_GOO_SVG_PAINTING_ATTRIBUTES_H
