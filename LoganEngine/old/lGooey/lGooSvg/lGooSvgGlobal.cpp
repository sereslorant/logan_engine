
#include "lGooSvgGlobal.h"


const std::string SVG_Shapes[] = {"rect","circle","line","ellipse","svg"};
const unsigned int SVG_SHAPES_SIZE = sizeof(SVG_Shapes)/sizeof(std::string);

const std::string SVG_PaintingAttribs[] = {"stroke","stroke-width","stroke-opacity","fill","fill-opacity"};
const unsigned int SVG_PAINTING_ATTRIBS_SIZE = sizeof(SVG_PaintingAttribs)/sizeof(std::string);

const std::string SVG_RectAttribs[] = {"x","y","width","height"};
const unsigned int SVG_RECT_ATTRIBS_SIZE = sizeof(SVG_RectAttribs)/sizeof(std::string);

const std::string SVG_CircleAttribs[] = {"cx","cy","r"};
const unsigned int SVG_CIRCLE_ATTRIBS_SIZE = sizeof(SVG_CircleAttribs)/sizeof(std::string);

const std::string SVG_LineAttribs[] = {"x1","y1","x2","y2"};
const unsigned int SVG_LINE_ATTRIBS_SIZE = sizeof(SVG_LineAttribs)/sizeof(std::string);

const std::string SVG_EllipseAttribs[] = {"cx","cy","rx","ry"};
const unsigned int SVG_ELLIPSE_ATTRIBS_SIZE = sizeof(SVG_EllipseAttribs)/sizeof(std::string);


const std::string SVG_TimingAttribs[] = {"begin","dur","end","restart","repeatCount","repeatDur","fill"};
const unsigned int SVG_TIMING_ATTRIBS_SIZE = sizeof(SVG_TimingAttribs)/sizeof(std::string);

const std::string SVG_AnimAttribs[] = {"attributeName","attributeType","additive","accumulate"};
const unsigned int SVG_ANIM_ATTRIBS_SIZE = sizeof(SVG_AnimAttribs)/sizeof(std::string);

/*
TODO!!!
Az alábbit át kéne olvasni!

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

const std::string SVG_AnimateAttribs[] = {"calcMode","keyTimes","keySplines","values","from","to","by"};
const unsigned int SVG_ANIMATE_ATTRIBS_SIZE = sizeof(SVG_AnimateAttribs)/sizeof(std::string);


const float CentiMeter = 96.0 / 2.54;

float u_cm(float param)
{
	return param * CentiMeter;
}

float u_mm(float param)
{
	return param * CentiMeter / 10.0;
}

float u_q(float param)
{
	return param * CentiMeter / 40.0;
}

float u_in(float param)
{
	return param * CentiMeter * 2.54;
}

float u_pc(float param)
{
	return param * CentiMeter * 2.54 / 6.0;
}

float u_pt(float param)
{
	return param * CentiMeter * 2.54 / 72.0;
}

float u_px(float param)
{
	return param;
}

