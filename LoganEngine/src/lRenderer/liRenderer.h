#ifndef LI_RENDERER_H
#define LI_RENDERER_H

#include <lScene/l2DScene/li2DScene.h>
#include <lScene/l3DScene/li3DScene.h>

#include <lResourceManager/liResourceManager.h>

class li2DLayer
{
public:
	virtual void SetScene(li2DScene *scene) = 0;
	virtual void SetCamera(li2DCamera *camera) = 0;
	//
	li2DLayer(){}
	virtual ~li2DLayer(){}
};

class li3DLayer
{
public:
	virtual void SetScene(li3DScene *scene) = 0;
	virtual void SetFrustum(liFrustum *frustum) = 0;
	virtual void SetCamera(li3DCamera *camera) = 0;
	//
	li3DLayer()
	{}
	//
	virtual ~li3DLayer()
	{}
};

class liLayerVisitor
{
public:
	virtual void Visit2DLayer(li2DLayer &layer) = 0;
	virtual void Visit3DLayer(li3DLayer &layer) = 0;
	//
	liLayerVisitor(){}
	virtual ~liLayerVisitor(){}
};

class liLayer
{
public:
	virtual void Enable() = 0;
	virtual void Disable() = 0;
	//
	virtual void Accept(liLayerVisitor &layer_visitor) = 0;
	//
	liLayer(){}
	virtual ~liLayer(){}
};

class liViewport
{
public:
	//
	virtual liLayer *Create2DLayer() = 0;
	virtual liLayer *Create3DLayer() = 0;
	//
	virtual int GetX() = 0;
	virtual int GetY() = 0;
	//
	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;
	//
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	//
	virtual void SetWidth(int width) = 0;
	virtual void SetHeight(int height) = 0;
	//
	liViewport(){}
	virtual ~liViewport(){}
};

class liFramebuffer2D
{
public:
	virtual unsigned int GetWidth() = 0;
	virtual unsigned int GetHeight() = 0;
	//
	virtual liViewport *CreateViewport(int x,int y,int width,int height) = 0;
	//
	liFramebuffer2D(){}
	virtual ~liFramebuffer2D(){}
};

class liRenderer
{
public:
	//
	virtual void SetResourceManager(liResourceManager *resource_manager) = 0;
	//
	virtual liFramebuffer2D &GetMainFramebuffer() = 0;
	virtual void Render() = 0;
	//
	liRenderer(){}
	virtual ~liRenderer(){}
};

#endif // LI_RENDERER_H
