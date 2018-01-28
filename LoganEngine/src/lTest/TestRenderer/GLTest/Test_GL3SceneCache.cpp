
/*
 * TMP
 */
#include <lResourceManager/lLegacyResourceManager/lResourceManager.h>
/*
 * TMP
 */

#include <memory>

#include <cmath>
#include <vector>

#include <iostream>

#include <lTest/Utils/lSDL2GLTestExtLoader.h>
#include <lTest/Utils/lSDL2TestFramework.h>


#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShaderLibrary.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGL3DShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLStaticMeshShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLSkeletalMeshShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLPhysicallyBasedShader.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGLData/lrGLTransformArray.h>

#include <lRenderer/lrRenderer/lrUtils.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGL3InstanceSet.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLRenderUtils.h>

#include "RendererTestUtils.h"
#include "RendererTestShaders.h"


int main(int argc,char *argv[])
{
	lSDL2GLTestFramework TestFramework(3,3,600,600);
	
	lSDL2ExtensionLoader ExtensionLoader;
	liGLExtLoader &ExtLoader = lGetGLExtLoader();

	ExtLoader.SetExtFunctionLoader(&ExtensionLoader);
	ExtLoader.LoadExtensions();

	ExtLoader.PrintExtStatus(std::cout);
	
	//Loading Static Mesh
	std::unique_ptr<lrmStaticMesh> StaticMesh(CreateSphereMesh());
	
	//Loading Skeletal Mesh
	std::vector<lrmSkeletalMesh> SkeletalMeshes;
	md5SkeletonExtrinsic BindPoseSkeleton;
	md5SkeletonHierarchy BpSkeletonMetadata;
	if(!LoadSkeletalMesh(SkeletalMeshes,BindPoseSkeleton,BpSkeletonMetadata))
	{
		return 1;
	}
	
	lrmMd5SkeletonAdapter BindPoseHierarchy(BpSkeletonMetadata,false,false,true);
	
	//Loading Skeletal Animation
	std::vector<md5SkeletonExtrinsic> Frames;
	md5SkeletonHierarchy AnimSkeletonMetadata;
	if(!LoadAnim(Frames,AnimSkeletonMetadata))
	{
		return 1;
	}
	
	lrmMd5SkeletonAdapter SkeletonHierarchy(AnimSkeletonMetadata,true,true,false);
	
	//Initializing static mesh resource
	lrGLStaticMeshResource StaticMeshResource;
	StaticMeshResource.Initialize(*StaticMesh);
	
	lrGLStaticMeshView StaticMeshView(&StaticMeshResource);
	lrGLStaticMaterialGroupView StaticMatGroupView = StaticMeshView.GetMaterialGroup(0);//(&StaticMeshResource.VertexData,&StaticMeshResource.MaterialGroups[0]);
	
	
	lrGLStaticMeshResource StaticMeshResource2;
	StaticMeshResource2.Initialize(SkeletalMeshes[0].BindPoseMesh);
	
	lrGLStaticMeshView StaticMeshView2(&StaticMeshResource2);
	lrGLStaticMaterialGroupView StaticMatGroupView2 = StaticMeshView2.GetMaterialGroup(0);//(&StaticMeshResource.VertexData,&StaticMeshResource.MaterialGroups[0]);
	
	
	//Initializing skeletal mesh resource
	lrGLSkeletalMeshResource SkeletalMeshResource;
	SkeletalMeshResource.Initialize(SkeletalMeshes[0]);
	
	lrGLSkeletalMeshView SkeletalMeshView(&SkeletalMeshResource);
	lrGLSkeletalMaterialGroupView SkeletalMatGroupView = SkeletalMeshView.GetMaterialGroup(0);
	
	GLuint StaticTestShaders[] = 
	{
		lrGLShaderLibrary::LoadShader(TestMvpStaticVertexShader,GL_VERTEX_SHADER,true),
		lrGLShaderLibrary::LoadShader(TestFragmentShader,GL_FRAGMENT_SHADER,true),
	};
	
	lrGLShader StaticTestProgram;
	StaticTestProgram.Initialize(StaticTestShaders,2);
	
	lrGLStaticMeshShader StatStaticMeshShader(StaticTestProgram);
	
	GLuint SkeletalTestShaders[] = 
	{
		lrGLShaderLibrary::LoadShader(TestMvpSkeletalVertexShader,GL_VERTEX_SHADER,true),
		lrGLShaderLibrary::LoadShader(TestFragmentShader,GL_FRAGMENT_SHADER,true),
	};
	
	lrGLShader SkeletalTestProgram;
	SkeletalTestProgram.Initialize(SkeletalTestShaders,2);
	
	lrGLStaticMeshShader SkelStaticMeshShader(SkeletalTestProgram);
	lrGLSkeletalMeshShader SkelSkeletalMeshShader(SkeletalTestProgram);
	
	//Creating Static Mesh Vertex Array Object
	//lrGL3StaticMeshBinding StaticMeshBinding;
	//StaticMeshBinding.Construct(StatStaticMeshShader,StaticMatGroupView);
	
	//Creating Static Mesh Vertex Array Object 2
	//lrGL3StaticMeshBinding StaticMeshBinding2;
	//StaticMeshBinding2.Construct(StatStaticMeshShader,StaticMatGroupView2);
	
	//Creating Skeletal Mesh Vertex Array Object
	//lrGL3SkeletalMeshBinding SkeletalMeshBinding;
	//SkeletalMeshBinding.Construct(SkelStaticMeshShader,SkelSkeletalMeshShader,SkeletalMatGroupView);
	
	//Setting up static instance set 1
	lrGL3StaticInstanceSet StaticInstanceSet1;
	StaticInstanceSet1.Construct(StatStaticMeshShader,StaticMatGroupView);
	StaticInstanceSet1.Resize(3);
	
	lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.0,0.0,0.125},{{0.0,1.0,0.0},0.0},{0.25,0.25,0.25},ModelMatrix);
	StaticInstanceSet1.SetInstance(lrGLMaterialData(),ModelMatrix,0);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.75,0.5,-0.125},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix);
	StaticInstanceSet1.SetInstance(lrGLMaterialData(),ModelMatrix,1);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({-0.75,0.5,-0.125},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix);
	StaticInstanceSet1.SetInstance(lrGLMaterialData(),ModelMatrix,2);
	
	StaticInstanceSet1.InitializeMvpMatrices(lmMatrix4x4(lmMatrix4x4::IDENTITY),lmMatrix4x4(lmMatrix4x4::IDENTITY));
	
	//Setting up static instance set 2
	lrGL3StaticInstanceSet StaticInstanceSet2;
	StaticInstanceSet2.Construct(StatStaticMeshShader,StaticMatGroupView2);
	StaticInstanceSet2.Resize(1);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.75,-0.5,0.0},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix);
	StaticInstanceSet2.SetInstance(lrGLMaterialData(),ModelMatrix,0);
	
	StaticInstanceSet2.InitializeMvpMatrices(lmMatrix4x4(lmMatrix4x4::IDENTITY),lmMatrix4x4(lmMatrix4x4::IDENTITY));
	
	//Setting up static instance set 3
	lrGL3StaticInstanceSet StaticInstanceSet3;
	StaticInstanceSet3.Construct(StatStaticMeshShader,StaticMatGroupView2);
	StaticInstanceSet3.Resize(2);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.5,-0.75,0.0},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix);
	StaticInstanceSet3.SetInstance(lrGLMaterialData(),ModelMatrix,0);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.25,-0.75,0.0},{{0.0,1.0,0.0},0.0},{0.05,0.05,0.05},ModelMatrix);
	StaticInstanceSet3.SetInstance(lrGLMaterialData(),ModelMatrix,1);
	
	StaticInstanceSet3.InitializeMvpMatrices(lmMatrix4x4(lmMatrix4x4::IDENTITY),lmMatrix4x4(lmMatrix4x4::IDENTITY));
	
	//Setting up skeletal instance set (This one will be uploaded in the main loop)
	lrGL3SkeletalInstanceSet SkeletalInstanceSet;
	SkeletalInstanceSet.Construct(SkelStaticMeshShader,SkelSkeletalMeshShader,SkeletalMatGroupView);
	SkeletalInstanceSet.Resize(3);
	
	lmMatrix4x4 ModelMatrix1(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({-0.5,-0.75,0.0},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix1);
	
	lmMatrix4x4 ModelMatrix2(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.0,0.125,0.0},{{0.0,1.0,0.0},3.14},{0.125,0.125,0.125},ModelMatrix2);
	
	lmMatrix4x4 ModelMatrix3(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({1.0,0.125,0.0},{{0.0,1.0,0.0},3.14},{0.125,0.125,0.125},ModelMatrix3);
	
	unsigned int FrameId1 = 0;
	unsigned int FrameId2 = 120;
	
	glViewport(0,0,600,600);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);
	
	bool IsRunning = true;
	while(IsRunning)
	{
		SDL_Event Event;
		while(SDL_PollEvent(&Event))
		{
			if(Event.type == SDL_QUIT)
			{
				IsRunning = false;
			}
		}
		
		FrameId1 = (FrameId1 + 1) % Frames.size();
		FrameId2 = (FrameId2 + 1) % Frames.size();
		
		std::size_t NumTransforms = std::min<std::size_t>(NUM_BONES,SkeletonHierarchy.Size());
		
		SkeletonHierarchy.Invalidate();
		
		lrBoneTransform CurrentTransform1;
		CurrentTransform1.resize(NumTransforms);
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			//CurrentTransform1[i] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			CurrentTransform1[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId1]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		
		SkeletalInstanceSet.SetInstance(lrGLMaterialData(),ModelMatrix1,CurrentTransform1,0);
		
		SkeletonHierarchy.Invalidate();
		
		lrBoneTransform CurrentTransform2;
		CurrentTransform2.resize(NumTransforms);
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			//CurrentTransform2[i] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			CurrentTransform2[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId2]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		
		SkeletalInstanceSet.SetInstance(lrGLMaterialData(),ModelMatrix2,CurrentTransform2,1);
		
		lrBoneTransform CurrentTransform3;
		CurrentTransform3.resize(NumTransforms);
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			//CurrentTransform2[i] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			CurrentTransform3[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId2]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		
		SkeletalInstanceSet.SetInstance(lrGLMaterialData(),ModelMatrix3,CurrentTransform3,2);
		
		SkeletalInstanceSet.InitializeMvpMatrices(lmMatrix4x4(lmMatrix4x4::IDENTITY),lmMatrix4x4(lmMatrix4x4::IDENTITY));
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		StaticTestProgram.UseProgram();
		
		{
			lrGL3DShader Shader3DWrapper(StaticTestProgram);
			lrGLPhysicallyBasedShader ShaderPbWrapper(SkeletalTestProgram);
			
			StaticInstanceSet1.Bind();
			StaticInstanceSet1.DrawInstances(Shader3DWrapper,ShaderPbWrapper);
			
			StaticInstanceSet2.Bind();
			StaticInstanceSet2.DrawInstances(Shader3DWrapper,ShaderPbWrapper);
			
			StaticInstanceSet3.Bind();
			StaticInstanceSet3.DrawInstances(Shader3DWrapper,ShaderPbWrapper);
		}
		
		StaticTestProgram.DisableProgram();
		
		SkeletalTestProgram.UseProgram();
		
		{
			lrGL3DShader Shader3DWrapper(SkeletalTestProgram);
			lrGLPhysicallyBasedShader ShaderPbWrapper(SkeletalTestProgram);
			
			SkeletalInstanceSet.Bind();
			SkeletalInstanceSet.DrawInstances(Shader3DWrapper,SkelSkeletalMeshShader,ShaderPbWrapper);
		}
		
		SkeletalTestProgram.DisableProgram();
		
		TestFramework.SwapWindow();
	}
	
	return 0;
}
