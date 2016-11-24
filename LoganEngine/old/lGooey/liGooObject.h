#ifndef LI_GOO_OBJECT_H
#define LI_GOO_OBJECT_H

#include <string>

#include "liGooColor.h"
#include "liGooEvent.h"
#include "../lRenderer/liGuiElement.h"
#include "../lRenderer/liGuiRenderer.h"

#include "lGooAnimation/lGooAnimation.h"

class liGooObject;

class liGooGUI
{
public:
	virtual liGooEvent &GetDocBeginEvent() = 0;
	virtual const liGooEvent &GetDocBeginEvent() const = 0;

	virtual liEventCallback *GetDocBeginCallback() = 0;
	//virtual const liEventCallback *GetDocBeginCallback() const = 0;

	virtual unsigned int GetResX() const = 0;
	virtual unsigned int GetResY() const = 0;

	virtual void SetResX(unsigned int res_x) = 0;
	virtual void SetResY(unsigned int res_y) = 0;

	virtual bool GetBgColorEnabled() const = 0;
	virtual const liGooColor &GetBgColor() const = 0;

	virtual void SetBgColorEnabled(bool bg_color_enabled) = 0;
	virtual void SetBgColor(const liGooColor &bg_color) = 0;

	virtual void AddChild(liGooObject *obj) = 0;

	virtual void AddObject(liGooObject *Obj) = 0;
	virtual void AddObjectId(const std::string &id,liGooObject *obj) = 0;

	virtual liGuiDisplayList *GetDisplayList() = 0;

    virtual void Step(lmScalar dt) = 0;

	liGooGUI()
	{}
	virtual ~liGooGUI()
	{}
};

class liGooContainer
{
public:

	virtual void AddObject(liGooObject *Obj) = 0;
	virtual void AddObjectId(const std::string &id,liGooObject *obj) = 0;

    virtual liGooObject *GetObject(const std::string &id) = 0;

	virtual void AddChildrenToGUI(liGooGUI *gui) = 0;

	liGooContainer(){}
	virtual ~liGooContainer(){}
};

class liGooRect
{
public:

    virtual void SetWidth(lmScalar width) = 0;
    virtual void SetHeight(lmScalar height) = 0;

    virtual lmScalar GetWidth() const = 0;
    virtual lmScalar GetHeight() const = 0;

    liGooRect(){};
    virtual ~liGooRect(){};
};

class liGooCircle
{
public:

    virtual lmScalar GetRadius() const = 0;
    virtual void SetRadius(lmScalar radius) = 0;

    liGooCircle(){};
    virtual ~liGooCircle(){};
};

class liGooLine
{
public:

	virtual const lmVector2D &GetV2() const = 0;
	virtual void SetV2(const lmVector2D &v2) = 0;

	liGooLine(){}
	virtual ~liGooLine(){}
};

class liGooEllipse
{
public:

    virtual lmScalar GetRadiusX() const = 0;
    virtual lmScalar GetRadiusY() const = 0;

    virtual void SetRadiusX(lmScalar radius_x) = 0;
    virtual void SetRadiusY(lmScalar radius_y) = 0;

    liGooEllipse(){}
    virtual ~liGooEllipse(){}
};

class liGooObject
{
public:

	virtual const lmVector2D &GetPosition() const = 0;
	virtual const lmVector2D &GetScale() const = 0;
	virtual float GetOrientation() const = 0;

    virtual void SetPosition(const lmVector2D &position) = 0;
    virtual void SetScale(const lmVector2D &scale) = 0;
    virtual void SetOrientation(float orientation) = 0;

	virtual const liGooColor &GetColor() const = 0;
    virtual void SetColor(const liGooColor &color) = 0;

    virtual bool IsTextured() const = 0;
    virtual const std::string &GetTexImage() const = 0;

	virtual void SetTextured(bool textured) = 0;
	virtual void SetTexImage(const std::string &tex_image) = 0;

    virtual bool HasOutline()const = 0;
    virtual const liGooColor &GetOutlineColor() const = 0;
    virtual float GetOutlineWidth() const = 0;

	virtual void SetOutline(bool outline) = 0;
    virtual void SetOutlineColor(const liGooColor &outline_color) = 0;
    virtual void SetOutlineWidth(float outline_width) = 0;

    virtual liGuiSolidColor &GetSolidColor() = 0;
    virtual liGuiSolidColor &GetSolidOutline() = 0;

    virtual liGuiElement *GetPolygon() = 0;
    virtual liGuiBrush *GetFillBrush() = 0;
    virtual void SetFillBrush(liGuiBrush *fill_brush) = 0;
    virtual liGuiBrush *GetStrokeBrush() = 0;
    virtual void SetStrokeBrush(liGuiBrush *stroke_brush) = 0;


    virtual void AddToGUI(liGooGUI *gui) = 0;

    /** \brief Getter függvény - Megadja az objektumot a helyére transzformáló mátrixot - konstans tagfüggvény
     * Tisztán virtuális tagfüggvény
     *
     * \return lmMatrix3x3 Az objektumot a helyére transzformáló mátrixa.
     *
     */
    virtual lmMatrix3x3 Transform() const = 0;

    virtual void SetParent(liGooObject *new_parent) = 0;

    virtual void AddAnimation(lGooAnimation *animation) = 0;
    virtual void AddAnimationId(const std::string &id,lGooAnimation *animation) = 0;

    virtual lGooAnimation *GetAnimation(const std::string &id) = 0;
    virtual const lGooAnimation *GetAnimation(const std::string &id) const = 0;

    virtual void Step(lmScalar dt) = 0;

    virtual liGooContainer *ToContainer() = 0;
    virtual liGooGUI *ToGUI() = 0;

    virtual liGooRect *ToRect() = 0;
    virtual liGooCircle *ToCircle() = 0;
    virtual liGooLine *ToLine() = 0;
    virtual liGooEllipse *ToEllipse() = 0;

	liGooObject(){}
	virtual ~liGooObject(){}
};

#endif // LI_GOO_OBJECT_H
