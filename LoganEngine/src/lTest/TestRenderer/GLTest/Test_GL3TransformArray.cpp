
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

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGLData/lrGLTransformArray.h>

#include <lRenderer/lrRenderer/lrUtils.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3StaticMeshVAO.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3SkeletalMeshVAO.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLRenderUtils.h>

#include "RendererTestUtils.h"
#include "RendererTestShaders.h"

using lrBoneTransform = std::vector<lmMatrix4x4>;

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
	lrGL3StaticMeshVAO StaticMeshBinding;
	StaticMeshBinding.Construct(StatStaticMeshShader,StaticMatGroupView);
	
	//Creating Skeletal Mesh Vertex Array Object
	lrGL3SkeletalMeshVAO SkeletalMeshBinding;
	SkeletalMeshBinding.Construct(SkelStaticMeshShader,SkelSkeletalMeshShader,SkeletalMatGroupView);
	
	lrGLTransformArray StaticMeshInstances(2);
	
	lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.0,0.0,0.125},{{0.0,1.0,0.0},0.0},{0.25,0.25,0.25},ModelMatrix);
	StaticMeshInstances.SetInstance(0,ModelMatrix);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.75,0.5,-0.125},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix);
	StaticMeshInstances.SetInstance(1,ModelMatrix);
	
	StaticMeshInstances.InitializeMvpMatrices(lmMatrix4x4(lmMatrix4x4::IDENTITY),lmMatrix4x4(lmMatrix4x4::IDENTITY));
	
	lrGLTransformArray SkeletalMeshInstances(3);
	std::vector<lrBoneTransform> BoneTransforms;
	BoneTransforms.resize(3);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({-0.5,-0.75,0.0},{{0.0,1.0,0.0},0.0},{0.125,0.125,0.125},ModelMatrix);
	SkeletalMeshInstances.SetInstance(0,ModelMatrix);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({0.0,0.125,0.0},{{0.0,1.0,0.0},3.14},{0.125,0.125,0.125},ModelMatrix);
	SkeletalMeshInstances.SetInstance(1,ModelMatrix);
	
	ModelMatrix = lmMatrix4x4(lmMatrix4x4::IDENTITY);
	lrUtils::GetModelMatrix({1.0,0.125,0.0},{{0.0,1.0,0.0},3.14},{0.125,0.125,0.125},ModelMatrix);
	SkeletalMeshInstances.SetInstance(2,ModelMatrix);
	
	SkeletalMeshInstances.InitializeMvpMatrices(lmMatrix4x4(lmMatrix4x4::IDENTITY),lmMatrix4x4(lmMatrix4x4::IDENTITY));
	
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
		
		lrBoneTransform &CurrentTransform1 = BoneTransforms[0];
		
		CurrentTransform1.resize(NumTransforms);
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			//CurrentTransform1[i] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			CurrentTransform1[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId1]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		
		SkeletonHierarchy.Invalidate();
		
		lrBoneTransform &CurrentTransform2 = BoneTransforms[1];
		
		CurrentTransform2.resize(NumTransforms);
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			//CurrentTransform2[i] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			CurrentTransform2[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId2]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		
		lrBoneTransform &CurrentTransform3 = BoneTransforms[2];
		
		CurrentTransform3.resize(NumTransforms);
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			//CurrentTransform3[i] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			CurrentTransform3[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId2]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		StaticTestProgram.UseProgram();
		
		StaticMeshBinding.Bind();
		for(int i=0;i < StaticMeshInstances.GetNumInstances();i++)
		{
			lrGL3DShader Shader3DWrapper(StaticTestProgram);
			lrGLRenderUtils::UploadUniformTransform(Shader3DWrapper,StaticMeshInstances.GetInstance(i));
			
			StaticMeshBinding.Draw();
		}
		
		StaticTestProgram.DisableProgram();
		
		SkeletalTestProgram.UseProgram();
		
		SkeletalMeshBinding.Bind();
		for(int i=0;i < SkeletalMeshInstances.GetNumInstances();i++)
		{
			lrGL3DShader Shader3DWrapper(SkeletalTestProgram);
			lrGLRenderUtils::UploadUniformTransform(Shader3DWrapper,SkeletalMeshInstances.GetInstance(i));
			
			lrBoneTransform &CurrentTransform = BoneTransforms[i];
			glUniformMatrix4fv(SkelSkeletalMeshShader.GetBoneTransformLocation(),CurrentTransform.size(),GL_FALSE,CurrentTransform[0][0]);
			
			SkeletalMeshBinding.Draw();
		}
		
		SkeletalTestProgram.DisableProgram();
		
		TestFramework.SwapWindow();
	}
	
	return 0;
}
