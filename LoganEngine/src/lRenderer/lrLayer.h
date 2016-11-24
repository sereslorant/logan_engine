#ifndef LR_LAYER_H
#define LR_LAYER_H

#include "../lInterfaces/lRenderer/liRenderer.h"

class lrLayer : public liLayer
{
private:
	bool Enabled = false;
	//
	virtual void DrawScene() = 0;
	//
public:
	//
	virtual void Enable() override
	{
		Enabled = true;
	}
	//
	virtual void Disable() override
	{
		Enabled = false;
	}
	//
	void Draw()
	{
		if(Enabled)
		{
			DrawScene();
		}
	}
	//
	lrLayer(){}
	virtual ~lrLayer() override{}
};

class lr2DLayer : public lrLayer, public li2DLayer
{
protected:
	li2DScene *Scene = nullptr;
	li2DCamera *Camera = nullptr;
	//
public:
	//
	virtual void Accept(liLayerVisitor &layer_visitor) override
	{
		layer_visitor.Visit2DLayer(*this);
	}
	//
	virtual void SetScene(li2DScene *scene) override
	{
		Scene = scene;
	}
	//
	virtual void SetCamera(li2DCamera *camera) override
	{
		Camera = camera;
	}
	//
	lr2DLayer()
	{}
	//
	virtual ~lr2DLayer() override
	{}
};

class lr3DLayer : public lrLayer, public li3DLayer
{
protected:
	li3DScene *Scene = nullptr;
	liFrustum *Frustum = nullptr;
	li3DCamera *Camera = nullptr;
	//
public:
	//
	virtual void Accept(liLayerVisitor &layer_visitor) override
	{
		layer_visitor.Visit3DLayer(*this);
	}
	//
	virtual void SetScene(li3DScene *scene) override
	{
		Scene = scene;
	}
	//
	virtual void SetFrustum(liFrustum *frustum) override
	{
		Frustum = frustum;
	}
	//
	virtual void SetCamera(li3DCamera *camera) override
	{
		Camera = camera;
	}
	//
	lr3DLayer(){}
	//
	virtual ~lr3DLayer() override {}
};


#endif // LR_LAYER_H
