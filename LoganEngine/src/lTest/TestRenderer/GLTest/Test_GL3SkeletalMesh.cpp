

/*
 * TMP
 */
#include <lResourceManager/lLegacyResourceManager/lResourceManager.h>
/*
 * TMP
 */


#include <iostream>

#include <lTest/Utils/lSDL2GLTestExtLoader.h>
#include <lTest/Utils/lSDL2TestFramework.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShaderLibrary.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLStaticMeshShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLSkeletalMeshShader.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3SkeletalMeshVAO.h>

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
	
	std::vector<lrmSkeletalMesh> SkeletalMeshes;
	md5SkeletonExtrinsic BindPoseSkeleton;
	md5SkeletonHierarchy BpSkeletonMetadata;
	if(!LoadSkeletalMesh(SkeletalMeshes,BindPoseSkeleton,BpSkeletonMetadata))
	{
		return 1;
	}
	
	lrmMd5SkeletonAdapter BindPoseHierarchy(BpSkeletonMetadata,false,false,true);
	
	std::vector<md5SkeletonExtrinsic> Frames;
	md5SkeletonHierarchy AnimSkeletonMetadata;
	if(!LoadAnim(Frames,AnimSkeletonMetadata))
	{
		return 1;
	}
	
	lrmMd5SkeletonAdapter SkeletonHierarchy(AnimSkeletonMetadata,true,true,false);
	
	lrGLSkeletalMeshResource SkeletalMeshResource;
	SkeletalMeshResource.Initialize(SkeletalMeshes[0]);
	
	lrGLSkeletalMeshView SkeletalMeshView(&SkeletalMeshResource);
	lrGLSkeletalMaterialGroupView MatGroupView = SkeletalMeshView.GetMaterialGroup(0);
	
	GLuint TestShaders[] = 
	{
		lrGLShaderLibrary::LoadShader(TestSkeletalVertexShader,GL_VERTEX_SHADER,true),
		lrGLShaderLibrary::LoadShader(TestFragmentShader,GL_FRAGMENT_SHADER,true),
	};
	
	lrGLShader TestProgram;
	TestProgram.Initialize(TestShaders,2);
	
	lrGLStaticMeshShader StaticMeshShader(TestProgram);
	lrGLSkeletalMeshShader SkeletalMeshShader(TestProgram);
	
	glViewport(0,0,600,600);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);
	
	unsigned int FrameId = 0;
	
	lrGL3SkeletalMeshVAO SkeletalMeshBinding;
	SkeletalMeshBinding.Construct(StaticMeshShader,SkeletalMeshShader,MatGroupView);
	
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
		
		SkeletonHierarchy.Invalidate();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		TestProgram.UseProgram();
		
		lmMatrix4x4 BoneTransform[NUM_BONES];
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			BoneTransform[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		glUniformMatrix4fv(SkeletalMeshShader.GetBoneTransformLocation(),NUM_BONES,GL_FALSE,BoneTransform[0][0]);
		
		FrameId = (FrameId + 1) % Frames.size();
		
		SkeletalMeshBinding.Bind();
		SkeletalMeshBinding.Draw();
		
		TestProgram.DisableProgram();
		
		TestFramework.SwapWindow();
	}
	
	return 0;
}
