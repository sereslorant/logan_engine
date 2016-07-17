#ifndef LI_RENDERER_H
#define LI_RENDERER_H

#include "li2DScene.h"

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

#endif // LI_RENDERER_H

