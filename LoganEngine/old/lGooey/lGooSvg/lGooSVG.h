#ifndef L_GOO_SVG
#define L_GOO_SVG

#include <list>
#include <string>

#include <fstream>
#include <iostream>

#include <algorithm>

#include "../lGooRenderer.h"

#include "XmlLoader.h"

#include "lGooSvgElements/lGooSvgRoot.h"

#include "lGooSvgGlobal.h"


/** \brief Csomagoló osztály egy ifstream köré, ami egy svg fájlból betölt egy GUI-t.
 *
 * Ami kész van:
 * -Be tudunk tölteni kört. (cx,cx,r)
 * -Be tudunk tölteni ellipszist. (cx,cy,rx,ry)
 * -Be tudunk tölteni line-t (x1,y1,x2,y2)
 * -Félig be tudunk tölteni rectet (x,y,width,height)
 *
 * Ami NINCS kész:
 * -A recteknek be kéne üzemelni a cx és cy paraméterét, ami lekerekíti a rect sarkait.
 * -Be kéne üzemelni a Polygont!
 * -Be kéne üzemelni a 'g' elementet!
 * -Egy csomó painting attribute, lásd: lGooSvgPaintingAttributes.h
 * -Markerek
 * -Display és Visibility propertyk
 * -Nincs styling, ehhez kéne CSS
 * -Nincsenek fontok
 *
 * Amit esélyes, hogy nem fog támogatni:
 * -Relatív mértékegységek
 *
 * \todo opcionálissá kéne tenni az XML fejléc jelenlétét a fájlban. Egyelőre hibaüzenetet dob, ha nincs. Mindezek mellé kommentelni kéne, hogy a parser hol túl toleráns fájlbetöltésnél, és javítani kéne.
 *
 */
class lGooSVG : public XmlLoader
{
public:
	enum SVG_ERROR
	{
		SVG_ERROR_OK			= 0,
		SVG_COULDNT_OPEN_FILE	= 1,
		SVG_XML_SYNTAX_ERROR	= 2,
		SVG_XML_FILE_CORRUPTED	= 4,
		SVG_ROOT_NOT_FOUND		= 8,
		SVG_SVG_SYNTAX_ERROR	= 16,
	};

	static void LoadColorTable()
	{
		ColorTable["white"]		= lGooColor(1.0,1.0,1.0,1.0);
		ColorTable["black"]		= lGooColor(0.0,0.0,0.0,1.0);
		ColorTable["blue"]		= lGooColor(0.0,0.0,1.0,1.0);
		ColorTable["orange"]	= lGooColor(1.0,165.0/255.0,0.0,1.0);
	}

	static int ParseInt(std::string &value)
	{
		return std::stoi(value);
	}

	static double ParseNumber(std::string &value)
	{
		return std::stod(value);
	}

	//"em" | "ex" | "px" | "in" | "cm" | "mm" | "pt" | "pc" | "%")

	static void ParseLengthDimension(std::string &value,SVG_LENGTH_DIMENSION &value_type);

	static double SetValueDimension(double value,SVG_LENGTH_DIMENSION value_type)
	{
		switch(value_type)
		{
		//case EM:
		//	break;

		//case EX:
		//	break;

		case PX:
			return u_px(value);

		case IN:
			return u_in(value);

		case CM:
			return u_cm(value);

		case MM:
			return u_mm(value);

		case PT:
			return u_pt(value);

		case PC:
			return u_pc(value);

		case PERCENTAGE:
			return value;
		}
	}

    /** \brief
     *
     * \todo Okosítani kéne.
     *
     * \param value std::string&
     * \param color lGooColor&
     * \return void
     *
     */
	static void ParseColor(std::string &value,lGooColor &color);
	static double ParseClockValue(std::string value);
	static void ParseSemiColonSeparatedList(std::list<std::string> &dest,const std::string &src);

private:
	liGooObjectFactory *ObjectFactory;
	lGooSvgRoot *SvgRoot;

	static std::map<std::string,lGooColor> ColorTable;

public:

	lGooSvgRoot *GetSvgRoot()
	{
		return SvgRoot;
	}

	lGooSVG(const std::string &filename,liGooObjectFactory *object_factory)
		:XmlLoader(filename),ObjectFactory(object_factory)
	{
		SvgRoot = new lGooSvgRoot(file,ObjectFactory);
		RootElement = SvgRoot;
	}

	~lGooSVG()
	{

	}
};

#endif // L_GOO_SVG
