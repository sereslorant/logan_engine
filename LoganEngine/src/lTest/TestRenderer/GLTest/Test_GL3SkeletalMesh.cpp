

/*
 * TMP
 */
#include <lResourceManager/lLegacyResourceManager/lResourceManager.h>
/*
 * TMP
 */


#include <iostream>

#include <lTest/TestRenderer/lSDL2GLTestExtLoader.h>
#include <lTest/TestRenderer/lSDL2TestFramework.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3Shaders/lrGL3ShaderLibrary.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3Shaders/lrGL3SkeletalMeshShader.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>

#include "GL3TestUtils.h"

/*
 * TODO: Itt kell majd minden olyat tesztelni, ami a static meshek kirajzolásához kellenek.
 */

const char *TestVertexShader = R"(
	#version 330
	
	const int MAX_WEIGHTS = 4;
	
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	
	uniform mat4 BoneTransform[128];
	
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	
	in ivec4 BoneId;
	in vec4  Weight;
	
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	
	void main()
	{
		mat4 Transform[MAX_WEIGHTS];
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			if(BoneId[i] > -1)
			{
				Transform[i] = BoneTransform[BoneId[i]];
			}
			else
			{
				Transform[i] = mat4(0.0);
			}
		}
		
		vec4 v4Vertex = vec4(Vertex,1.0);

		vec3 AnimVertex = vec3(0.0,0.0,0.0);
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			AnimVertex += (Weight[i] * (Transform[i] * v4Vertex)).xyz;
		}
		
		fVertex = (/*ModelMatrix**/vec4(Vertex,1.0)).xyz;
		fNormal = /*NormalMatrix* */Normal;
		fTexCoord = TexCoord;
		//gl_Position = /* MvpMatrix* */vec4(Vertex / 4.0,1.0) - vec4(0.0,0.5,0.0,0.0);
		gl_Position = /* MvpMatrix* */vec4(AnimVertex / 4.0,1.0) - vec4(0.0,0.5,0.0,0.0);
	}
)";

const char *TestFragmentShader = R"(
	#version 330
	
	in vec3 fVertex;
	in vec3 fNormal;
	in vec2 fTexCoord;
	
	out vec4 FragColor;
	
	void main()
	{
		FragColor = vec4((1.0 + fNormal)/2,1.0);
		//FragColor = vec4(fTexCoord,0.0,1.0);
	}
)";

int main(int argc,char *argv[])
{
	lSDL2GLTestFramework TestFramework(3,3,600,600);
	
	lSDL2ExtensionLoader ExtensionLoader;
	liGLExtLoader &ExtLoader = lGetGLExtLoader();
	
	ExtLoader.SetExtFunctionLoader(&ExtensionLoader);
	ExtLoader.LoadExtensions();
	
	ExtLoader.PrintExtStatus(std::cout);
	
	std::vector<lrmSkeletalMesh> SkeletalMeshes;
	md5File File;
	
	std::fstream mesh;
	mesh.open("bob_lamp_update_export.md5mesh");
	
	if(!mesh.is_open())
	{
		std::cout << "Error couldn't open mesh file!" << std::endl;
	}
	
	md5LoadFile(mesh,File);
	
	SkeletalMeshes.resize(File.Meshes.size());
	for(unsigned int i=0;i < SkeletalMeshes.size();i++)
	{
		md5ExtractSkeletalMesh(File,true,i,SkeletalMeshes[i]);
	}
	
	md5SkeletonExtrinsic BindPoseSkeleton;
	md5Utility::Skeleton_YUp(File.BindPoseSkeleton,BindPoseSkeleton);
	
	md5Anim Anim;
	
	std::fstream anim;
	anim.open("retardaltkodik_with_lamp.md5anim");
	
	if(!anim.is_open())
	{
		std::cout << "Error couldn't open anim file!" << std::endl;
	}
	
	md5LoadAnim(anim,Anim);
	
	std::vector<md5SkeletonExtrinsic> Frames;
	Frames.resize(Anim.NumFrames);
	for(unsigned int i=0;i < Frames.size();i++)
	{
		md5ExtractAnimFrame(Anim,true,i,Frames[i]);
	}
	
	lrmSkeletalMesh *SkeletalMesh = &SkeletalMeshes[0];
	
	lrGLSkeletalMeshResource SkeletalMeshResource;
	SkeletalMeshResource.Initialize(*SkeletalMesh);
	
	lrGLSkeletalMeshView SkeletalMeshView(&SkeletalMeshResource);
	lrGLSkeletalMaterialGroupView MatGroupView = SkeletalMeshView.GetMaterialGroup(0);
	
	GLuint TestShaders[] = 
	{
		lrGL3ShaderLibrary::LoadShader(TestVertexShader,GL_VERTEX_SHADER),
		lrGL3ShaderLibrary::LoadShader(TestFragmentShader,GL_FRAGMENT_SHADER),
	};
	
	lGL3SkeletalMeshShader TestProgram;
	TestProgram.Initialize(TestShaders,2);
	
	glViewport(0,0,600,600);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);
	
	unsigned int FrameId = 0;
	
	lrmMd5SkeletonAdapter BindPoseHierarchy(File.BindPoseSkeletonMetadata,false,false,true);
	lrmMd5SkeletonAdapter SkeletonHierarchy(Anim.SkeletonHierarchy,true,true,false);
	
	lrGLVertexArray VertexArray;
	VertexArray.BindVertexArray();
	
	MatGroupView.BindAttributes(TestProgram);
	MatGroupView.BindBoneData(TestProgram);
	MatGroupView.BindIndexBuffer();
	
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
		
		lmMatrix4x4 BoneTransform[256];
		for(int i=0;i < SkeletonHierarchy.Size();i++)
		{
			BoneTransform[i] = SkeletonHierarchy.GetTransformation(i,Frames[FrameId]) * BindPoseHierarchy.GetInverseTransformation(i,BindPoseSkeleton);
		}
		glUniformMatrix4fv(TestProgram.GetBoneTransformLocation(),256,GL_FALSE,BoneTransform[0][0]);
		
		FrameId = (FrameId + 1) % Frames.size();
		
		MatGroupView.Draw();
		
		TestProgram.DisableProgram();
		
		TestFramework.SwapWindow();
	}
	
	return 0;
}
