

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


#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3Shaders/lrGL3ShaderLibrary.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3Shaders/lrGL3StaticMeshShader.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>

#include "GL3TestUtils.h"

/*
 * TODO: Itt kell majd minden olyat tesztelni, ami a static meshek kirajzolásához kellenek.
 */

const char *TestVertexShader = R"(
	#version 330
	
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	
	void main()
	{
		fVertex = (/*ModelMatrix**/vec4(Vertex,1.0)).xyz;
		fNormal = /*NormalMatrix* */Normal;
		fTexCoord = TexCoord;
		gl_Position = /* MvpMatrix* */vec4(Vertex / 4.0,1.0);
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
	
	lrmStaticMesh *StaticMesh = new lrmStaticMesh;
	lrmResourceManager::GenerateSphere(*StaticMesh,64,32);
	
	lrGLStaticMeshResource StaticMeshResource;
	StaticMeshResource.Initialize(*StaticMesh);
	
	lrGLStaticMeshView StaticMeshView(&StaticMeshResource);
	lrGLStaticMaterialGroupView MatGroupView = StaticMeshView.GetMaterialGroup(0);//(&StaticMeshResource.VertexData,&StaticMeshResource.MaterialGroups[0]);
	
	GLuint TestShaders[] = 
	{
		lrGL3ShaderLibrary::LoadShader(TestVertexShader,GL_VERTEX_SHADER),
		lrGL3ShaderLibrary::LoadShader(TestFragmentShader,GL_FRAGMENT_SHADER),
	};
	
	lrGL3StaticMeshShader TestProgram;
	TestProgram.Initialize(TestShaders,2);
	
	glViewport(0,0,600,600);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);
	
	lrGLVertexArray VertexArray;
	VertexArray.BindVertexArray();
	
	MatGroupView.BindAttributes(TestProgram);
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
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		TestProgram.UseProgram();
		
		MatGroupView.Draw();
		
		TestProgram.DisableProgram();
		
		TestFramework.SwapWindow();
	}
	
	return 0;
}
