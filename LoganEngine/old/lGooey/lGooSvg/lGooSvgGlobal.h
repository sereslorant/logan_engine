#ifndef L_GOO_SVG_GLOBAL_H
#define L_GOO_SVG_GLOBAL_H

#include <string>

enum SVG_LENGTH_DIMENSION
{
	//EM,
	//EX,
	PX,
	IN,
	CM,
	MM,
	PT,
	PC,
	PERCENTAGE,
};

enum SVG_SHAPE_INDEX
{
	SVG_CIRCLE,
	SVG_RECT,
	SVG_LINE,
	SVG_ELLIPSE,
};

extern const std::string SVG_Shapes[];
extern const unsigned int SVG_SHAPES_SIZE;

enum SVG_PAINTING_ATTRIBS_INDEX
{
	SVG_ANIM_FILL,
	SVG_ANIM_STROKE,
	SVG_ANIM_STROKE_WIDTH,
};

extern const std::string SVG_PaintingAttribs[];
extern const unsigned int SVG_PAINTING_ATTRIBS_SIZE;

enum SVG_RECT_ATTRIBS_INDEX
{
	SVG_RECT_X,
	SVG_RECT_Y,
	SVG_RECT_WIDTH,
	SVG_RECT_HEIGHT,
};

extern const std::string SVG_RectAttribs[];
extern const unsigned int SVG_RECT_ATTRIBS_SIZE;

enum SVG_CIRCLE_ATTRIBS_INDEX
{
	SVG_CIRCLE_CX,
	SVG_CIRCLE_CY,
	SVG_CIRCLE_R,
};

extern const std::string SVG_CircleAttribs[];
extern const unsigned int SVG_CIRCLE_ATTRIBS_SIZE;

enum SVG_LINE_ATTRIBS_INDEX
{
	SVG_LINE_X1,
	SVG_LINE_Y1,
	SVG_LINE_X2,
	SVG_LINE_Y2,
};

extern const std::string SVG_LineAttribs[];
extern const unsigned int SVG_LINE_ATTRIBS_SIZE;

enum SVG_ELLIPSE_ATTRIBS_INDEX
{
	SVG_ELLIPSE_CX,
	SVG_ELLIPSE_CY,
	SVG_ELLIPSE_RX,
	SVG_ELLIPSE_RY,
};

extern const std::string SVG_EllipseAttribs[];
extern const unsigned int SVG_ELLIPSE_ATTRIBS_SIZE;

enum SVG_TIMING_ATTRIBS_INDEX
{
	SVG_TIMING_BEGIN,
	SVG_TIMING_DUR,
	SVG_TIMING_END,
	SVG_TIMING_RESTART,
	SVG_TIMING_REPEAT_COUNT,
	SVG_TIMING_REPEAT_DUR,
	SVG_TIMING_FILL,
};

extern const std::string SVG_TimingAttribs[];
extern const unsigned int SVG_TIMING_ATTRIBS_SIZE;

enum SVG_ANIM_ATTRIBS_INDEX
{
	SVG_ANIM_ATTRIBUTE_NAME,
	SVG_ANIM_ATTRIBUTE_TYPE,
	SVG_ANIM_ADDITIVE,
	SVG_ANIM_ACCUMULATE,
};

extern const std::string SVG_AnimAttribs[];
extern const unsigned int SVG_ANIM_ATTRIBS_SIZE;

enum SVG_ANIMATE_ATTRIBS_INDEX
{
	SVG_ANIMATE_CALC_MODE,
	SVG_ANIMATE_KEY_TIMES,
	SVG_ANIMATE_KEY_SPLINES,
	SVG_ANIMATE_VALUES,
	SVG_ANIMATE_FROM,
	SVG_ANIMATE_TO,
	SVG_ANIMATE_BY,
};

extern const std::string SVG_AnimateAttribs[];
extern const unsigned int SVG_ANIMATE_ATTRIBS_SIZE;


//Length dimensions
float u_cm(float param);
float u_mm(float param);
float u_q (float param);
float u_in(float param);
float u_pc(float param);
float u_pt(float param);
float u_px(float param);

#endif // L_GOO_SVG_GLOBAL_H
