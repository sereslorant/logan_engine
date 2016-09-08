#ifndef L_GL_3D_LAYER_H
#define L_GL_3D_LAYER_H

#include "../../lRenderer/lrLayer.h"

class lGL3DLayer : public lrLayer, public li3DLayer
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
	lGL3DLayer(){}
	//
	virtual ~lGL3DLayer(){}
};

#endif // LFF_GL_3D_LAYER_H
