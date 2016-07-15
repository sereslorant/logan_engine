#ifndef LI_VIEW_H
#define LI_VIEW_H

#include "../../lMath/lmVectorND.hpp"
#include "liColor.h"

class li2DCamera
{
public:
	virtual const lmVector2D &GetPosition() = 0;
	virtual void SetPosition(const lmVector2D &position) = 0;

	virtual float GetWidth() = 0;
	virtual float GetHeight() = 0;

	li2DCamera(){}
	virtual ~li2DCamera(){}
};

class li2DRectangle
{
public:
	virtual const lmVector2D &GetPosition() = 0;
	virtual float GetWidth() = 0;
	virtual float GetHeight() = 0;

	li2DRectangle(){}
	virtual ~li2DRectangle(){}
};

class li2DSceneVisitor
{
public:
	//
	virtual void VisitRectangle(li2DRectangle &rectangle) = 0;
	//
	li2DSceneVisitor(){}
	virtual ~li2DSceneVisitor(){}
};

class li2DElement
{
public:
	virtual void SetPosition(const lmVector2D &position) = 0;
	virtual void SetColor(const liColor &color) = 0;
	virtual const liColor &GetColor() = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual bool IsHidden() = 0;
	//
	virtual void Accept(li2DSceneVisitor &visitor) = 0;
	//
	li2DElement(){}
	virtual ~li2DElement(){}
};

class li2DSceneDrawer
{
public:
	virtual void Draw(li2DElement &element) = 0;

	li2DSceneDrawer(){}
	virtual ~li2DSceneDrawer(){}
};

class li2DScene
{
public:
	//
	class liElementFactory
	{
	public:
		//
		virtual li2DElement *CreateRectangle(const lmVector2D &position,float width,float height) = 0;
		//
		liElementFactory(){}
		virtual ~liElementFactory(){}
	};
	//
	virtual bool HasBackgroundColor() = 0;
	virtual const liColor &GetBackgroundColor() = 0;
	virtual void SetBackgroundColor(const liColor &bg_color) = 0;
	virtual void ClearBackgroundColor() = 0;
	//
	virtual liElementFactory &GetElementFactory() = 0;
	//
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void Draw(li2DSceneDrawer &scene_visitor) = 0;
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	li2DScene(){}
	virtual ~li2DScene(){}
};

class li2DRenderer
{
public:
	/*
	class liDrawableFactory
	{
	public:
		//
		virtual li2DElement *CreateRect(float x,float y,float width,float height) = 0;
		//
		liDrawableFactory(){}
		virtual ~liDrawableFactory(){}
	};
	*/

	//virtual liDrawableFactory &GetDrawableFactory() = 0;
	virtual void SetScene(li2DScene *scene) = 0;
	virtual void SetCamera(li2DCamera *camera) = 0;
	virtual void Render() = 0;

	li2DRenderer(){}
	virtual ~li2DRenderer(){}
};

#endif // LI_VIEW_H

