

#include <iostream>

#include "lP2_Program.h"

//#include "../lTest/lffGLRenderer/lffGLRenderer.h"
#include "../lRenderer/lGLRenderer/lGL3Renderer/lrGL3Renderer.h"

#include "../lApiAdapter/lSDL2_ApiAdapter/lSDL2_ApiAdapter.h"

static const std::string DEF_TITLE = "LoganEngine";
static const bool DEF_FULLSCREEN = false;
static const unsigned int DEF_WIDTH = 800;
static const unsigned int DEF_HEIGHT = 600;
static const lSDL2_Settings::lRenderingApi DEF_RENDERING_API = lSDL2_Settings::OPENGL;

class lP2_GameInstantiator : public liGameInstantiator
{
public:
	virtual liGame *CreateGame(liApiAdapter &api_adapter,lrmResourceManager &resource_manager,liRenderer &renderer,liConsole &console) override
	{
		liGame *Game;
		#ifdef PAC_MAN
			liGameMode *GameMode = new lPM_Game(api_adapter.GetInput(),renderer,0.125);//new lGameMode();
			Game = new lGame(console,GameMode,renderer);
		#else
			liWorld2D *World = new lP2World2D;
			liGameMode *GameMode = new lP2_TestGameMode(api_adapter.GetInput(),*World,renderer);
			Game = new lSimulationGame<liWorld2D>(console,GameMode,renderer,World);
		#endif
		return Game;
	}

	lP2_GameInstantiator(){}
	virtual ~lP2_GameInstantiator(){}
};

int main(int argc, char *argv[])
{
	lSDL2_Settings Settings;
	//
	Settings.Title = DEF_TITLE;
	Settings.Fullscreen = DEF_FULLSCREEN;
	Settings.Width = DEF_WIDTH;
	Settings.Height = DEF_HEIGHT;
	Settings.RenderingApi = DEF_RENDERING_API;

	lSDL2_ApiAdapter ApiAdapter(Settings);
	lSDL2_GLExtFunctionLoader GLExtFunctionLoader;

	liGLExtLoader &GLExtLoader = lGetGLExtLoader();

	GLExtLoader.SetExtFunctionLoader(&GLExtFunctionLoader);
	GLExtLoader.LoadExtensions();
	GLExtLoader.PrintExtStatus(std::cout);

	lrmResourceManager ResourceManager;

	//lffGLRenderer Renderer(800,600,ResourceManager);
	lrGL3Renderer Renderer(800,600,ResourceManager);

	lP2_GameInstantiator GameInstantiator;

	lP2_Program Program(ApiAdapter,ResourceManager,Renderer,GameInstantiator);

	Program.MainLoop();
	//fclose(stdin);

	return 0;
}
