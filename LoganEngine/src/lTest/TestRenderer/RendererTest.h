#ifndef RENDERER_TEST_H
#define RENDERER_TEST_H

#include "../../lInterfaces/lRenderer/liRenderer.h"

#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModCamera.h"
#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModScene.h"

class RendererTest
{
private:
	liRenderer *Renderer;
	//
	li3DModScene *Scene3D = nullptr;
	liModFrustum *Frustum3D = nullptr;
	li3DModCamera *Camera3D = nullptr;
	//
	li3DModMesh *TestElement = nullptr;
	li3DModLight *TestLight = nullptr;
	//
	liViewport *Viewport3D = nullptr;
	//
	float Angle = 0.0;
	float OtherAngle = 0.0;
	//
public:
	//
	void Initialize();
	//
	void Loop();
	//
	void Draw();
	//
	RendererTest(liRenderer *renderer)
		:Renderer(renderer)
	{}
	//
	~RendererTest()
	{
		delete Scene3D;
		delete Frustum3D;
		delete Camera3D;
	}
};

#endif // RENDERER_TEST_H
