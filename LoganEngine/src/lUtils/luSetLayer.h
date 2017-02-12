#ifndef LU_SET_LAYER_H
#define LU_SET_LAYER_H

#include <lRenderer/liRenderer.h>

#include <lScene/l2DScene/li2DScene.h>

class luSet2DLayer : public liLayerVisitor
{
private:
	li2DScene *Scene;
	li2DCamera *Camera;
	//
public:
	//
	virtual void Visit2DLayer(li2DLayer &layer) override
	{
		layer.SetScene(Scene);
		layer.SetCamera(Camera);
	}
	//
	virtual void Visit3DLayer(li3DLayer &layer) override
	{
		//Üres
	}
	//
	luSet2DLayer(li2DScene *scene,li2DCamera *camera)
		:Scene(scene),Camera(camera)
	{}
	//
	virtual ~luSet2DLayer() override
	{}
};

#include <lScene/l3DScene/li3DScene.h>
#include <lScene/l3DScene/li3DCamera.h>

class luSet3DLayer : public liLayerVisitor
{
private:
	li3DScene *Scene;
	liFrustum *Frustum;
	li3DCamera *Camera;

public:
	//
	virtual void Visit2DLayer(li2DLayer &layer) override
	{
		//Üres
	}
	//
	virtual void Visit3DLayer(li3DLayer &layer) override
	{
		layer.SetScene(Scene);
		layer.SetFrustum(Frustum);
		layer.SetCamera(Camera);
	}
	//
	luSet3DLayer(li3DScene *scene,liFrustum *frustum,li3DCamera *camera)
		:Scene(scene),Frustum(frustum),Camera(camera)
	{}
	//
	virtual ~luSet3DLayer() override
	{}
};


#endif // LU_SET_LAYER_H
