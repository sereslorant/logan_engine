#ifndef LR_2D_SCENE_H
#define LR_2D_SCENE_H

#include "../../lInterfaces/lRenderer/li2DScene.h"
#include "../lrColor.h"


class lr2DCamera : public li2DCamera
{
private:
	lmVector2D		Position;

	float Width;
	float Height;

public:
	virtual const lmVector2D &GetPosition() override
	{
		return Position;
	}

	virtual void SetPosition(const lmVector2D &position) override
	{
		Position = position;
	}

	virtual float GetWidth() override
	{
		return Width;
	}

	virtual float GetHeight() override
	{
		return Height;
	}

	/*
	bool IsEnabled()
	{
		return Enabled;
	}

	void Enable()
	{
		Enabled = true;
	}

	void Disable()
	{
		Enabled = false;
	}
	*/

	lr2DCamera(const lmVector2D &position,float width,float height)
		:Position(position),Width(width),Height(height)
	{
		//
	}

	virtual ~lr2DCamera() override
	{
		//
	}
};

class lr2DElement : public li2DElement
{
protected:
	lmVector2D Position;
	lrColor Color;
	//
	bool Hidden = false;
	//
public:

	virtual void SetPosition(const lmVector2D &position) override
	{
		Position = position;
	}

	virtual void SetColor(const liColor &color) override
	{
		Color = color;
	}

	virtual const liColor &GetColor() override
	{
		return Color;
	}

	virtual void Show() override
	{
		Hidden = false;
	}

	virtual void Hide() override
	{
		Hidden = true;
	}

	virtual bool IsHidden() override
	{
		return Hidden;
	}

	lr2DElement(const lmVector2D &position)
		:Position(position),Color(0.0,0.0,0.0,0.0)
	{}

	virtual ~lr2DElement() override
	{}
};

class lr2DRectangle : public lr2DElement, public li2DRectangle
{
protected:
	float Width;
	float Height;
	//
public:
	/*
	float GetX()
	{
		return Position[0];
	}

	float GetY()
	{
		return Position[1];
	}
	*/
	virtual const lmVector2D &GetPosition() override
	{
		return Position;
	}

	virtual float GetWidth() override
	{
		return Width;
	}

	virtual float GetHeight() override
	{
		return Height;
	}

	virtual void Accept(li2DSceneVisitor &visitor) override
	{
		visitor.VisitRectangle(*this);
	}

	lr2DRectangle(const lmVector2D &position,float width,float height)
		:lr2DElement(position),Width(width),Height(height)
	{}

	virtual ~lr2DRectangle() override
	{}
};

#include <list>

class lr2DScene : public li2DScene
{
protected:
	//
	class lr2DElementFactory : public liElementFactory
	{
	private:
		lr2DScene &Scene;
		//
	public:
		//
		virtual li2DElement *CreateRectangle(const lmVector2D &position,float width,float height) override
		{
			lr2DRectangle *Rect = new lr2DRectangle(position,width,height);
			Scene.Elements.push_back(Rect);

			return Rect;
		}
		//
		lr2DElementFactory(lr2DScene &scene)
			:Scene(scene)
		{}
		//
		virtual ~lr2DElementFactory()
		{}
	};
	//
	bool HasBackground;
	lrColor BgColor;
	//
	std::list<lr2DElement *> Elements;
	//
	lr2DElementFactory ElementFactory;
	//
public:
	//
	virtual bool HasBackgroundColor() override
	{
		return HasBackground;
	}
	//
	virtual const liColor &GetBackgroundColor() override
	{
		return BgColor;
	}
	//
	virtual void SetBackgroundColor(const liColor &bg_color) override
	{
		BgColor = bg_color;
		HasBackground = true;
	}
	//
	virtual void ClearBackgroundColor() override
	{
		HasBackground = false;
	}
	//
	virtual liElementFactory &GetElementFactory() override
	{
		return ElementFactory;
	}
	//
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void Draw(li2DSceneDrawer &scene_drawer) override
	{
		for(lr2DElement *Element : Elements)
		{
			scene_drawer.Draw(*Element);
		}
	}
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	lr2DScene()
		:HasBackground(false),BgColor(0.0,0.0,0.0,0.0),ElementFactory(*this)
	{
		//
	}
	//
	virtual ~lr2DScene()
	{
		for(lr2DElement *Element : Elements)
		{
			delete Element;
		}
	}
};

#endif // LR_2D_SCENE_H
