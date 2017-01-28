
#include <iostream>

#include "lP2_Program.h"

#include "../lRenderer/lGLRenderer/lGL3Renderer/lrGL3Renderer.h"

#include "../lApiAdapter/lSDL2_ApiAdapter/lSDL2_ApiAdapter.h"

static const std::string DEF_TITLE = "LoganEngine";
static const bool DEF_FULLSCREEN = false;
static const unsigned int DEF_WIDTH = 800;
static const unsigned int DEF_HEIGHT = 600;
static const lSDL2_Settings::lRenderingApi DEF_RENDERING_API = lSDL2_Settings::OPENGL;

liGameInstantiator *CreateGameInstantiator();

int main(int argc, char *argv[])
{
	lSDL2_Settings Settings;
	//
	Settings.Title = DEF_TITLE;
	Settings.Fullscreen = DEF_FULLSCREEN;
	Settings.Width = DEF_WIDTH;
	Settings.Height = DEF_HEIGHT;
	Settings.RenderingApi = DEF_RENDERING_API;
	//
	lSDL2_ApiAdapter ApiAdapter(Settings);
	lSDL2_GLExtFunctionLoader GLExtFunctionLoader;
	//
	liGLExtLoader &GLExtLoader = lGetGLExtLoader();
	//
	GLExtLoader.SetExtFunctionLoader(&GLExtFunctionLoader);
	GLExtLoader.LoadExtensions();
	GLExtLoader.PrintExtStatus(std::cout);
	//
	lrmResourceManager ResourceManager;
	//
	//lffGLRenderer Renderer(800,600,ResourceManager);
	lrGL3Renderer Renderer(800,600);
	//
	//#ifdef PAC_MAN
	//lPM_GameInstantiator GameInstantiator;
	//#else
	//lP2_GameInstantiator GameInstantiator;
	//#endif
	//
	liGameInstantiator *GameInstantiator = CreateGameInstantiator();
	//
	lP2_Program Program(ApiAdapter,ResourceManager,Renderer,*GameInstantiator);
	//
	Program.MainLoop();
	//fclose(stdin);
	//
	delete GameInstantiator;
	//
	return 0;
}
