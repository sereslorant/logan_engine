

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

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3StaticMeshVAO.h>

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
	
	lrmStaticMesh *StaticMesh = new lrmStaticMesh;
	lrmResourceManager::GenerateSphere(*StaticMesh,64,32);
	
	lrGLStaticMeshResource StaticMeshResource;
	StaticMeshResource.Initialize(*StaticMesh);
	
	lrGLStaticMeshView StaticMeshView(&StaticMeshResource);
	lrGLStaticMaterialGroupView MatGroupView = StaticMeshView.GetMaterialGroup(0);//(&StaticMeshResource.VertexData,&StaticMeshResource.MaterialGroups[0]);
	
	GLuint TestShaders[] = 
	{
		lrGLShaderLibrary::LoadShader(TestStaticVertexShader,GL_VERTEX_SHADER,true),
		lrGLShaderLibrary::LoadShader(TestFragmentShader,GL_FRAGMENT_SHADER,true),
	};
	
	lrGLShader TestProgram;
	TestProgram.Initialize(TestShaders,2);
	
	lrGLStaticMeshShader TestProgramWrapper(TestProgram);
	
	glViewport(0,0,600,600);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);
	
	lrGL3StaticMeshVAO StaticMeshBinding;
	StaticMeshBinding.Construct(TestProgramWrapper,MatGroupView);
	
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
		
		StaticMeshBinding.Bind();
		StaticMeshBinding.Draw();
		
		TestProgram.DisableProgram();
		
		TestFramework.SwapWindow();
	}
	
	return 0;
}
