#ifndef L_GOO_SVG_ANIM_ATTRIBUTES
#define L_GOO_SVG_ANIM_ATTRIBUTES

#include "../lGooSvgGlobal.h"

#include <string>
#include <list>

struct lGooSvgAnimAttributes
{
	bool Error;

	bool HasId;
	std::string Id;

	//Timing attributes
	std::list<std::string> Attr_begin;
	double Attr_dur;
	std::list<std::string> Attr_end;

	enum SVG_ANIM_RESTART
	{
		RST_ALWAYS,
		RST_NEVER,
		RST_WHEN_NOT_ACTIVE,
	};
	SVG_ANIM_RESTART Attr_restart; /*(always | never |  whenNotActive)*/
	double Attr_repeatCount; /*Indefinite esetén nullára kell állítani*/
	double Attr_repeatDur; /*Mi a fasz ez? o.O*/

	enum SVG_ANIM_FILL
	{
		F_REMOVE,
		F_FREEZE,
	};
	SVG_ANIM_FILL Attr_fill; /*(remove | freeze)*/

	//Anim attributes
	bool AttributeFound = false;
	std::string Attr_attributeName;

	enum SVG_ANIM_ATTRIB_TYPE
	{
		T_CSS,
		T_XML,
		T_AUTO,
	};
	SVG_ANIM_ATTRIB_TYPE Attr_attributeType; /*(CSS,XML,auto)*/

	enum SVG_ANIM_ADDITIVE
	{
		ADD_REPLACE,
		ADD_SUM,
	};
	SVG_ANIM_ADDITIVE Attr_additive; /*(replace | sum)*/

	enum SVG_ANIM_ACCUMULATE
	{
		ACC_NONE,
		ACC_SUM,
	};
	SVG_ANIM_ACCUMULATE Attr_accumulate; /*(none | sum)*/

	//Animate attributes
	enum SVG_ANIM_CALC_MODE
	{
		CM_DISCRETE,
		CM_LINEAR,
		CM_PACED,
		CM_SPLINE,
	};
	SVG_ANIM_CALC_MODE Attr_calcMode; /*(discrete | linear | paced | spline )*/

	std::list<double> Attr_keyTimes;
	std::list<double> Attr_keySplines;

	bool ValuesFound;
	std::list<std::string> Attr_values;


	/*
	The simpler from/to/by syntax provides for several variants. To use one of these variants,
	one of by or to must be specified; a from value is optional. It is not legal to specify
	both by and to attributes - if both are specified, only the to attribute will be used
	(the by will be ignored). The combinations of attributes yield the following classes
	of animation:

	from-to animation: Specifying a from value and a to value defines a simple animation,
	equivalent to a values list with 2 values. The animation function is defined to start
	with the from value, and to finish with the to value.

	from-by animation: Specifying a from value and a by value defines a simple animation
	in which the animation function is defined to start with the from value, and to change
	this over the course of the simple duration by a delta specified with the by attribute.
	This may only be used with attributes that support addition (e.g. most numeric attributes).

	by animation: Specifying only a by value defines a simple animation in which the animation
	function is defined to offset the underlying value for the attribute, using a delta that
	varies over the course of the simple duration, starting from a delta of 0 and ending with
	the delta specified with the by attribute. This may only be used with attributes that support
	addition.

	to animation: This describes an animation in which the animation function is defined to start
	with the underlying value for the attribute, and finish with the value specified with the to
	attribute. Using this form, an author can describe an animation that will start with any current
	value for the attribute, and will end up at the desired to value.
	*/

	bool FromFound;
	std::string Attr_from;

	bool ToFound;
	std::string Attr_to; //Ha to és by attribute is van, akkor csak a to-t használjuk!

	bool ByFound;
	std::string Attr_by;

	enum SVG_ANIM_TYPE
	{
		ANIM_FROM_TO,
		ANIM_FROM_BY,
		ANIM_BY,
		ANIM_TO,
		ANIM_VALUE,
	};
	SVG_ANIM_TYPE AnimType;

	void LoadAttribs(const std::string &attribute,std::string &value);
	void Process();

	lGooSvgAnimAttributes();
};

#endif
