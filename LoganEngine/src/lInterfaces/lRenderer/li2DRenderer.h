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

class li2DLayer
{
public:
	virtual void SetScene(li2DScene *scene) = 0;
	virtual void SetCamera(li2DCamera *camera) = 0;

	li2DLayer(){}
	virtual ~li2DLayer(){}
};

class liLayerVisitor
{
public:
	virtual void Visit2DLayer(li2DLayer &layer) = 0;

	liLayerVisitor(){}
	virtual ~liLayerVisitor(){}
};

class liLayer
{
public:
	virtual void Enable() = 0;
	virtual void Disable() = 0;

	virtual void Accept(liLayerVisitor &layer_visitor) = 0;

	liLayer(){}
	virtual ~liLayer(){}
};

class liViewport
{
public:

	virtual liLayer *Create2DLayer() = 0;

	virtual int GetX() = 0;
	virtual int GetY() = 0;

	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;

	virtual void SetWidth(int width) = 0;
	virtual void SetHeight(int height) = 0;

	liViewport(){}
	virtual ~liViewport(){}
};

class liFramebuffer
{
public:
	virtual unsigned int GetWidth() = 0;
	virtual unsigned int GetHeight() = 0;
	//
	virtual liViewport *CreateViewport(int x,int y,int width,int height) = 0;
	//
	liFramebuffer(){}
	virtual ~liFramebuffer(){}
};

class li2DRenderer
{
public:

	virtual liFramebuffer &GetMainFramebuffer() = 0;
	virtual void Render() = 0;

	li2DRenderer(){}
	virtual ~li2DRenderer(){}
};

#endif // LI_VIEW_H

