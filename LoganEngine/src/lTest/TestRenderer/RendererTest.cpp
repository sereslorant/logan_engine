

#include <iostream>
#include <chrono>

#include "RendererTest.h"

#include <lScene/lrScene/lr3DScene/lr3DScene.h>

#include "../../lUtils/luSetLayer.h"
#include "../../lUtils/luGetModMesh.h"

/*
 * Segédfüggvények material adatok kiszámításához
 */
//
float CalculateFresnel(float refractive_index,float extinction_coefficient)
{
	return (std::pow(refractive_index - 1.0,2) + std::pow(extinction_coefficient,2))/
				(std::pow(refractive_index + 1.0,2) + std::pow(extinction_coefficient,2));
}
//
lrColor CalculateFresnel(const lrColor &refractive_index,const lrColor &extinction_coefficient)
{
	lrColor FresnelReflectance(0.0,0.0,0.0,1.0);
	//
	FresnelReflectance.SetRed(CalculateFresnel(refractive_index.GetRed(),extinction_coefficient.GetRed()));
	FresnelReflectance.SetGreen(CalculateFresnel(refractive_index.GetGreen(),extinction_coefficient.GetGreen()));
	FresnelReflectance.SetBlue(CalculateFresnel(refractive_index.GetBlue(),extinction_coefficient.GetBlue()));
	//
	return FresnelReflectance;
}

lrColor GoldFresnel()
{
	return CalculateFresnel(lrColor(0.17,0.35,1.5,1.0),lrColor(3.1,2.7,1.9,1.0));
}

lrColor SilverFresnel()
{
	return CalculateFresnel(lrColor(0.15865,0.14215,0.13533,1.0),lrColor(3.8929,3.0051,2.3276,1.0));
}

lrMaterial CreateMaterial(const lrColor &color,float roughness,float metallic,const std::string &texture_name)
{
	lrMaterial Material;
	//
	Material.SetAlbedo(color);
	//
	Material.SetFresnel(0.6);
	Material.SetRoughness(roughness);
	Material.SetReflectiveness(0.5);
	Material.SetMetallic(metallic);
	//
	Material.SetAlbedoMap(texture_name);
	//
	return Material;
}

lrMaterial CreateSkinMaterial()
{
	lrMaterial Material;
	//
	lrColor Tan(253.0/255.0,217.0/255.0,181.0/255.0,1.0);
	float SkinFresnel = CalculateFresnel(0.028,0.0);
	//
	Material.SetAlbedo(Tan);
	//
	Material.SetFresnel(SkinFresnel);
	Material.SetRoughness(0.25);
	Material.SetReflectiveness(0.25);
	Material.SetMetallic(0.0);
	//
	Material.SetAlbedoMap("Empty");
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
		luSet3DLayer SetTestLayer(Scene3D,Frustum3D,Camera3D);
		TestLayer->Accept(SetTestLayer);
		//
		TestLayer->Enable();
		//
		SetScene();
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
	LightPos[0] = 7.0f*cos(OtherAngle);
	LightPos[1] = 0.0;
	LightPos[2] = 7.0f*sin(OtherAngle);
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
		//std::cout << "FPS = " << 1.0/(Difference.count() / 1000000000.0) << std::endl;
	}
	else
	{
		std::cerr << "Error: Renderer is not set" << std::endl;
	}
}

void RendererFunctionalityTest::SetScene()
{
	lrMaterialLibrary MaterialLibrary;
	MaterialLibrary.AddMaterial("Default",CreateMaterial(GoldFresnel(),0.35,1.0,"Empty"));
	//
	li3DModElement *ModElement = Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({0.0f,0.0f,-4.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary,"Sphere");
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({0.0f,2.0f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({0.6f,0.6f,0.6f}),MaterialLibrary,"Sphere");
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({2.0f,0.0f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary,"Cone");
	//
	lrMaterialLibrary MaterialLibrary1;
	MaterialLibrary1.AddMaterial("Default",CreateSkinMaterial());
	//
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({-14.0f,-1.0f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary1,"Anything");
	//
	lrMaterialLibrary MaterialLibrary2;
	MaterialLibrary2.AddMaterial("Default",CreateMaterial(SilverFresnel(),0.35,1.0,"Puszcsy"));
	//
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({-1.5f,1.5f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary2,"Box");
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({4.0f,0.0f,-8.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary2,"Sphere");
	//
	lrMaterialLibrary MaterialLibrary31;
	MaterialLibrary31.AddMaterial("Default",CreateMaterial(SilverFresnel(),0.25,0.0,"Empty"));
	lrMaterialLibrary MaterialLibrary32;
	MaterialLibrary32.AddMaterial("Default",CreateMaterial(SilverFresnel(),0.25,0.0,"Puszcsy"));
	lrMaterialLibrary MaterialLibrary33;
	MaterialLibrary33.AddMaterial("Default",CreateMaterial(SilverFresnel(),0.85,1.0,"Empty"));
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({ 0.0f,0.0f,0.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({30.0f,15.0f,20.0f}),MaterialLibrary31,"Box_InsideOut");
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({-2.0f,0.0f,-6.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary32,"Anything");
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({4.0f,0.0f,5.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),0.0),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary33,"Sphere");
	//
	lrMaterialLibrary MaterialLibrary4;
	MaterialLibrary4.AddMaterial("Default",CreateMaterial(GoldFresnel(),0.125,1.0,"Empty"));
	//
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({0.0f,0.0f,-8.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary4,"Sphere");
	//
	lrMaterialLibrary MaterialLibrary5;
	MaterialLibrary5.AddMaterial("Default",CreateMaterial(GoldFresnel(),0.05,1.0,"Empty"));
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({6.0f,1.0f,-8.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({0.6f,0.6f,0.6f}),MaterialLibrary5,"Sphere");
	//
	lrMaterialLibrary MaterialLibrarySk;
	MaterialLibrarySk.AddMaterial("Default",CreateMaterial(SilverFresnel(),0.5,1.0,"Empty"));
	MaterialLibrarySk.AddMaterial("bob_body",CreateMaterial(SilverFresnel(),0.5,1.0,"Empty"));
	//
	Scene3D->GetElementFactory().CreateSkeletalMesh(lmVector3D({0.0f,-3.0f,4.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrarySk,"BobWithLamp");
	//
	lrMaterialLibrary MaterialLibrary6;
	MaterialLibrary6.AddMaterial("Material.001",CreateMaterial(SilverFresnel(),0.05,1.0,"Szercsy"));
	Scene3D->GetElementFactory().CreateStaticMesh(lmVector3D({0.0f,0.0f,8.0f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({1.0f,1.0f,1.0f}),MaterialLibrary6,"LoadedMesh");
	//
	std::string Textures[6] = {"Empty","Puszcsy","Szercsy","Arcsy","Faszcsy","Cukcsy"};
	std::string Models[4] = {"Sphere","Cone","Box","LoadedMesh"};
	lmVector3D Pos = {-6.0f,-4.0f,20.0f};
	for(int i=0;i < 20;i++)
	{
		for(int j=0;j < 20;j++)
		{
			//for(int k=0;k < 20;k++)
			for(int k=0;k < 1;k++)
			{
				lrMaterialLibrary MaterialLibrary5;
				MaterialLibrary5.AddMaterial("Default",CreateMaterial(GoldFresnel(),0.05,1.0,Textures[(i + j + k)%6]));
				MaterialLibrary5.AddMaterial("Material.001",CreateMaterial(SilverFresnel(),0.05,1.0,Textures[(i + j + k)%6]));
				Scene3D->GetElementFactory().CreateStaticMesh(Pos + lmVector3D({i * 0.5f,k * 0.5f,j * 0.5f}),lmQuaternion(lmVector3D({0.0f,1.0f,0.0f}),Angle),lmVector3D({0.25f,0.25f,0.25f}),MaterialLibrary5,Models[(i + j + 2*k)%4]);
			}
		}
	}
	//
	luGetModMesh Getter;
	ModElement->Accept(Getter);
	TestElement = Getter.GetModMesh();
	//
	ModElement = Scene3D->GetElementFactory().CreatePointLight({0.0f,-3.0f,0.0f});
	luGetModLight GetLight;
	ModElement->Accept(GetLight);
	li3DModLight &ModLight = *GetLight.GetModLight();
	//
	ModLight.SetColor(lrColor(1.0,1.0,1.0,1.0));
	ModLight.SetIntensity(25.0);
	//
	ModElement = Scene3D->GetElementFactory().CreatePointLight({4.0f,4.0f,-2.0f});
	ModElement->Accept(GetLight);
	li3DModLight &ModLight2 = *GetLight.GetModLight();
	//
	ModLight2.SetColor(lrColor(1.0,1.0,1.0,1.0));
	ModLight2.SetIntensity(25.0);
	//
	ModElement = Scene3D->GetElementFactory().CreatePointLight({-4.0f,4.0f,-2.0f});
	ModElement->Accept(GetLight);
	li3DModLight &ModLight3 = *GetLight.GetModLight();
	//
	ModLight3.SetColor(lrColor(1.0,1.0,1.0,1.0));
	ModLight3.SetIntensity(25.0);
	//
	ModElement = Scene3D->GetElementFactory().CreatePointLight({-12.0f,0.0f,2.0f});
	ModElement->Accept(GetLight);
	li3DModLight &ModLight4 = *GetLight.GetModLight();
	//
	ModLight4.SetColor(lrColor(1.0,1.0,1.0,1.0));
	ModLight4.SetIntensity(25.0);
	//ModLight.SetDiffuse(L_RED_INDEX,0.5);
	//ModLight.SetDiffuse(L_GREEN_INDEX,0.5);
	//ModLight.SetDiffuse(L_BLUE_INDEX,0.5);
	//
	ModElement = Scene3D->GetElementFactory().CreatePointLight({0.0f,0.0f,0.0f});
	ModElement->Accept(GetLight);
	TestLight = GetLight.GetModLight();
	//
	//TestLight->SetDiffuse(L_RED_INDEX,0.5);
	//TestLight->SetDiffuse(L_GREEN_INDEX,0.5);
	TestLight->SetColor(lrColor(1.0,1.0,0.4,1.0));
	TestLight->SetIntensity(30);
}
