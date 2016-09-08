
#include <iostream>

#include "RendererTest.h"

#include "../../lScene/lr3DScene/lr3DScene.h"

#include "../../lUtils/luSetLayer.h"
#include "../../lUtils/luGetModMesh.h"

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
	Angle += PI/64.0;
	//Camera3D->SetYaw(Angle);
	lmQuaternion NewOrientation(lmVector3D({-1.0f,1.0f,0.0f}),Angle);
	//
	TestElement->SetOrientation(NewOrientation);
}

void RendererTest::Draw()
{
	if(Renderer != nullptr)
	{
		Renderer->Render();
	}
	else
	{
		std::cerr << "Error: Renderer is not set" << std::endl;
	}
}
