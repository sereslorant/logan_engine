
#include <iostream>

#include "RendererTest.h"

#include "../../lScene/lr3DScene/lr3DScene.h"

#include "../../lUtils/luSetLayer.h"
#include "../../lUtils/luGetModMesh.h"

#include <chrono>

lrMaterial CreateMaterial(const std::string &texture_name)
{
	lrMaterial Material;
	//
	Material.SetDiffuse(L_RED_INDEX,1.0);
	Material.SetDiffuse(L_GREEN_INDEX,1.0);
	Material.SetDiffuse(L_BLUE_INDEX,1.0);
	//
	Material.SetDiffuseMap(texture_name);
	//
	return Material;
}

void RendererTest::Initialize()
{
	if(Renderer != nullptr)
	{
		Viewport3D = Renderer->GetMainFramebuffer().CreateViewport(0,0,600,600);
		//
		liLayer *TestLayer = Viewport3D->Create3DLayer();
		//
		Scene3D = new lr3DScene();
		Frustum3D = new lrFrustum(0.5,-0.5,-0.5,0.5,100.0,1.0);
		Camera3D = new lr3DCamera(lmVector3D({0.0,0.0,0.0}),0.0,0.0);
		//
		lrMaterialLibrary MaterialLibrary;
		MaterialLibrary.AddMaterial("Default",CreateMaterial("Puszcsy"));
		//
		li3DModElement *ModElement = Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({0.0f,0.0f,-4.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary,"Sphere");
		Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({0.0f,3.0f,-4.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary,"Sphere");
		Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({3.0f,0.0f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary,"Cone");
		//
		lrMaterialLibrary MaterialLibrary2;
		MaterialLibrary2.AddMaterial("Default",CreateMaterial("Szercsy"));
		/*TestElement = */Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({-1.5f,1.5f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary2,"Box");
		Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({ 0.0f,0.0f,0.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({10.0f,20.0f,20.0f}),MaterialLibrary2,"Box_InsideOut");
		Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({-3.0f,0.0f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary2,"Anything");
		//
		luGetModMesh Getter;
		ModElement->Accept(Getter);
		TestElement = Getter.GetModMesh();
		//
		ModElement = Scene3D->GetElementFactory().CreatePointLight({0.0f,0.0f,0.0f});
		luGetModLight GetLight;
		ModElement->Accept(GetLight);
		li3DModLight &ModLight = *GetLight.GetModLight();
		//
		ModLight.SetDiffuse(L_RED_INDEX,0.5);
		ModLight.SetDiffuse(L_GREEN_INDEX,0.5);
		ModLight.SetDiffuse(L_BLUE_INDEX,0.5);
		//
		ModElement = Scene3D->GetElementFactory().CreatePointLight({0.0f,0.0f,0.0f});
		ModElement->Accept(GetLight);
		TestLight = GetLight.GetModLight();
		//
		TestLight->SetDiffuse(L_RED_INDEX,0.5);
		TestLight->SetDiffuse(L_GREEN_INDEX,0.5);
		//
		luSet3DLayer SetTestLayer(Scene3D,Frustum3D,Camera3D);
		TestLayer->Accept(SetTestLayer);
		//
		TestLayer->Enable();
	}
	else
	{
		std::cerr << "Error: Renderer is not set" << std::endl;
	}
}

void RendererTest::Loop()
{
	Angle += PI/128.0;
	OtherAngle += PI/128.0;
	//Camera3D->SetYaw(Angle);
	lmVector3D Axle = lmVector3D({-1.0f,1.0f,0.0f});
	Axle.Normalize();
	lmQuaternion NewOrientation(Axle,Angle);
	//
	TestElement->SetOrientation(NewOrientation);

	lmVector3D LightPos;
	LightPos[0] = 10.0f*cos(OtherAngle);
	LightPos[1] = 0.0;
	LightPos[2] = 10.0f*sin(OtherAngle);
	TestLight->SetPosition(LightPos);
}

void RendererTest::Draw()
{
	if(Renderer != nullptr)
	{
		auto Before = std::chrono::steady_clock::now();
		Renderer->Render();
		auto After = std::chrono::steady_clock::now();
		//
		std::chrono::nanoseconds Difference = After - Before;
		//std::chrono::seconds Difference_sec = After - Before;
		//
		std::cout << "Frame time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		//std::cout << "FPS = " << 1.0/Difference_sec.count() << std::endl;
	}
	else
	{
		std::cerr << "Error: Renderer is not set" << std::endl;
	}
}
