#ifndef LI_GUI_RENDERER_H
#define LI_GUI_RENDERER_H

#include "liGuiElement.h"
#include "../lRenderer/lrColor.h"
#include "../lInterfaces/lRenderer/liImg.h"

class liGuiBrush
{
public:

	virtual void UseBrush() = 0;
	virtual void Draw(const liGuiElement *gui_element,bool outline) = 0;

	liGuiBrush(){}
	virtual ~liGuiBrush(){}
};

class liGuiSolidColor
{
public:
	virtual void SetColor(const liColor &color) = 0;
	virtual const liColor &GetColor() const = 0;

	liGuiSolidColor(){}
	virtual ~liGuiSolidColor(){}
};

#include "../lMath/lmMatrix3x3.h"

class liGuiDisplayListNode
{
public:

	virtual bool IsEnabled() const = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;

	virtual bool IsOutline() const = 0;
	virtual void SetOutline(bool outline) = 0;

	virtual float GetOutlineWidth() const = 0;
	virtual void SetOutlineWidth(float outline_width) = 0;

	virtual liGuiBrush *GetBrush() = 0;
	virtual void SetBrush(liGuiBrush *brush) = 0;

	virtual const lmMatrix3x3 &GetTransformation() const = 0;
	virtual void SetTransformation(const lmMatrix3x3 &transformation) = 0;

	virtual const liGuiElement *GetGuiElement() const = 0;
	virtual void SetGuiElement(liGuiElement *gui_element) = 0;

	liGuiDisplayListNode(){}
	virtual ~liGuiDisplayListNode(){}
};

class liGuiDisplayList
{
public:

	virtual void SetWidth(float width) = 0;
	virtual void SetHeight(float height) = 0;

	virtual float GetWidth() = 0;
	virtual float GetHeight() = 0;

	virtual bool BgColorEnabled() = 0;
	virtual const liColor &GetBgColor() = 0;

	virtual void SetBgColorEnabled(bool has_bg_color) = 0;
	virtual void SetBgColor(const liColor &bg_color) = 0;

	virtual liGuiDisplayListNode *CreateNonTexturedListNode() = 0;
	virtual liGuiDisplayListNode *CreateTexturedListNode(const std::string &texture) = 0;

	liGuiDisplayList(){}
	virtual ~liGuiDisplayList(){}
};

#include "../lInterfaces/lRenderer/liFramebuffer.h"

class liGuiRenderer
{
public:

    virtual liGuiElement *AddPolygon(const lmVector2D Vrt[],unsigned int Length) = 0;

    virtual liGuiElement *GetCircle() = 0;
    virtual liGuiElement *GetRect() = 0;
    virtual liGuiElement *GetLine() = 0;
    virtual liGuiElement *GetEllipse() = 0;

    virtual liGuiBrush *CreateSolidBrush(liGuiSolidColor *solid_color) = 0;

    virtual liGuiDisplayList *CreateDisplayList() = 0;

    virtual void SetRenderer(liGuiDisplayList *display_list) = 0;
    virtual void Draw(liFramebuffer *framebuffer) = 0;
    //virtual void ClearDL() = 0;

    liGuiRenderer(){}
    virtual ~liGuiRenderer(){}
};

class lrGuiDisplayListNode : public liGuiDisplayListNode
{
private:
	bool Enabled = true;

	bool Outline = false;
	float OutlineWidth = 0.0;

	liGuiBrush *Brush = nullptr;

	liGuiElement *GuiElement = nullptr;
	lmMatrix3x3 Transformation;

public:

	virtual bool IsEnabled() const override {return Enabled;}
	virtual void Enable() override {Enabled = true;}
	virtual void Disable() override {Enabled = false;}

	virtual bool IsOutline() const override {return Outline;}
	virtual void SetOutline(bool outline) override {Outline = outline;}

	virtual float GetOutlineWidth() const override {return OutlineWidth;}
	virtual void SetOutlineWidth(float outline_width) override {OutlineWidth = outline_width;}

	virtual liGuiBrush *GetBrush() override {return Brush;}
	virtual void SetBrush(liGuiBrush *brush) override {Brush = brush;}

	virtual const liGuiElement *GetGuiElement() const override {return GuiElement;}
	virtual void SetGuiElement(liGuiElement *gui_element) override {GuiElement = gui_element;}

	virtual const lmMatrix3x3 &GetTransformation() const override {return Transformation;}
	virtual void SetTransformation(const lmMatrix3x3 &transformation) override {Transformation = transformation;}

	lrGuiDisplayListNode(){}
	virtual ~lrGuiDisplayListNode() override {}
};

#include <string>

#include <list>
#include <map>

class lrGuiDisplayList : public liGuiDisplayList
{
private:
	float Width = 0.0;
	float Height = 0.0;

	bool HasBgColor = false;
	lrColor BgColor = {0.0,0.0,0.0,0.0};

	std::list<liGuiDisplayListNode *> NonTexturedNodes;
	std::map<std::string,std::list<liGuiDisplayListNode *> > TexturedNodes;

public:

	virtual void SetWidth(float width) override {Width = width;}
	virtual void SetHeight(float height) override {Height = height;}

	virtual float GetWidth() override {return Width;}
	virtual float GetHeight() override {return Height;}

	virtual bool BgColorEnabled() override
	{
		return HasBgColor;
	}

	virtual const liColor &GetBgColor() override
	{
		return BgColor;
	}

	virtual void SetBgColorEnabled(bool has_bg_color) override
	{
		HasBgColor = has_bg_color;
	}

	virtual void SetBgColor(const liColor &bg_color) override
	{
		BgColor.Red		= bg_color.GetRed();
		BgColor.Green	= bg_color.GetGreen();
		BgColor.Blue	= bg_color.GetBlue();
		BgColor.Alpha	= bg_color.GetAlpha();
	}

	virtual liGuiDisplayListNode *CreateNonTexturedListNode() override
	{
		liGuiDisplayListNode *NewNode = new lrGuiDisplayListNode;
		NonTexturedNodes.push_back(NewNode);

		return NewNode;
	}

	virtual liGuiDisplayListNode *CreateTexturedListNode(const std::string &texture) override
	{
		liGuiDisplayListNode *NewNode = new lrGuiDisplayListNode;
		TexturedNodes[texture].push_back(NewNode);

		return NewNode;
	}


	std::list<liGuiDisplayListNode *> &GetNonTexturedNodes() {return NonTexturedNodes;}
	std::map<std::string,std::list<liGuiDisplayListNode *> > &GetTexturedNodes() {return TexturedNodes;}

	lrGuiDisplayList()
	{}

	virtual ~lrGuiDisplayList() override
	{
		for(liGuiDisplayListNode *I : NonTexturedNodes)
		{
			delete I;
		}

		for(auto I : TexturedNodes)
		{
			for(liGuiDisplayListNode *J : I.second)
			{
				delete J;
			}
		}
	}
};


#endif // LI_GUI_RENDERER_H
