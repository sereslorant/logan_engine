// wattafakk
//DisIsDatFack 8=================D
#include "../lGame/lConsole.h"
#include "lGameModes/lPacMan/lPacMan.h"




class lrViewport
{
private:
	int X;
	int Y;
	int Width;
	int Height;

public:

	int GetX()
	{
		return X;
	}

	int GetY()
	{
		return Y;
	}

	void SetX(int x)
	{
		X = x;
	}

	void SetY(int y)
	{
		Y = y;
	}

	int GetWidth()
	{
		return Width;
	}

	int GetHeight()
	{
		return Height;
	}

	void SetWidth(int width)
	{
		Width = width;
	}

	void SetHeight(int height)
	{
		Height = height;
	}

	lrViewport(int x,int y,int width,int height)
		:X(x),Y(y),Width(width),Height(height)
	{}

	virtual ~lrViewport()// override
	{}
};


#include <SDL2/SDL_opengl.h>

#include "../lInterfaces/lRenderer/li2DRenderer.h"

class lrLayer
{
private:
	bool Enabled = false;

	virtual void DrawScene() = 0;

public:

	void Enable()
	{
		Enabled = true;
	}

	void Disable()
	{
		Enabled = false;
	}

	void Draw()
	{
		if(Enabled)
		{
			DrawScene();
		}
	}

	lrLayer(){}
	virtual ~lrLayer(){}
};

class lGL2D_SceneDrawer : public li2DSceneDrawer,public li2DSceneVisitor
{
public:
	//
	virtual void VisitRectangle(li2DRectangle &rectangle) override
	{
		glBegin(GL_QUADS);

		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1] + rectangle.GetHeight());
		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1] + rectangle.GetHeight());

		glEnd();
	}
	//
	virtual void Draw(li2DElement &element) override
	{
		if(!element.IsHidden())
		{
			glColor3f(element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue());

			element.Accept(*this);
		}
	}
	//
	lGL2D_SceneDrawer(){}
	virtual ~lGL2D_SceneDrawer(){}
};

class lGL2D_Layer : public lrLayer
{
private:
	li2DScene *Scene = nullptr;
	li2DCamera *Camera = nullptr;

	virtual void DrawScene() override
	{
		if(Scene->HasBackgroundColor())
		{
			glClearColor(Scene->GetBackgroundColor().GetRed(),Scene->GetBackgroundColor().GetGreen(),Scene->GetBackgroundColor().GetBlue(),Scene->GetBackgroundColor().GetAlpha());
			glClear(GL_COLOR_BUFFER_BIT);
		}
		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(Camera->GetPosition()[0],
				Camera->GetPosition()[0] + Camera->GetWidth(),
				Camera->GetPosition()[1] + Camera->GetHeight(),
				Camera->GetPosition()[1],
				-1,1);

		glMatrixMode(GL_MODELVIEW);

		lGL2D_SceneDrawer SceneDrawer;
		Scene->Draw(SceneDrawer);
	}

public:

	void SetScene(li2DScene *scene)
	{
		Scene = scene;
	}

	void SetCamera(li2DCamera *camera)
	{
		Camera = camera;
	}

	lGL2D_Layer(){}
	virtual ~lGL2D_Layer() override {}
};

class lGL2D_Renderer : public li2DRenderer
{
private:
	//
	//li2DScene *Scene = nullptr;
	lGL2D_Layer Layer;
	//
public:

	virtual void SetScene(li2DScene *scene) override
	{
		Layer.SetScene(scene);
	}

	virtual void SetCamera(li2DCamera *camera) override
	{
		Layer.SetCamera(camera);
	}
	//
	virtual void Render() override
	{
		/*
		if(Scene->HasBackgroundColor())
		{
			glClearColor(Scene->GetBackgroundColor().GetRed(),Scene->GetBackgroundColor().GetGreen(),Scene->GetBackgroundColor().GetBlue(),Scene->GetBackgroundColor().GetAlpha());
			glClear(GL_COLOR_BUFFER_BIT);
		}
		//
        glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
        glOrtho(0,800,600,0,-1,1);
        //
		Scene->Draw();
		*/
		Layer.Draw();
	}

	lGL2D_Renderer()
	{
		glClearColor(1,1,1,1);
		glViewport(0,0,800,600);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_DEPTH_TEST);
		//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Layer.Enable();
	}

	virtual ~lGL2D_Renderer() override
	{
    	//
	}
};

#include "lGame/lP2Game.h"
#include "lGame/lP2World2D.h"
#include "../lRenderer/lr2DScene/lr2DScene.h"

#include <SDL2/SDL.h>

#include "../lInterfaces/liApiAdapter.h"

#include <iostream>

class lP2_Program
{
private:
	liApiAdapter &ApiAdapter;

	liConsole *Console;
	liThread *ConsoleThread = nullptr;

	liGame *Game;
	li2DRenderer *Renderer;

public:

	void MainLoop()
	{
		if(ConsoleThread != nullptr)
			{ConsoleThread->Start();}
		//
		ApiAdapter.GetFrameLimiter().SetFPS(60);
		//
		bool IsRunning = true;
		while(IsRunning)
		{
			ApiAdapter.GetFrameLimiter().StartFrameLimiter();
			//
			ApiAdapter.PollInput();
			if(ApiAdapter.GetInput().GetQuit())
				{IsRunning = false;}
			//
			Game->Step();
			//
			Renderer->Render();
			ApiAdapter.SwapBuffers();
			//
			ApiAdapter.GetFrameLimiter().CheckFrameLimiter();
		}
		//
		//ConsoleThread->Kill();
	}

    lP2_Program(liApiAdapter &api_adapter,bool has_console = false)
		:ApiAdapter(api_adapter)
    {
		if(has_console)
		{
			lConsole *RunnableConsole = new lConsole(std::cin,std::cout,std::cerr);
			Console = RunnableConsole;
			//
			ConsoleThread = ApiAdapter.GetThreadFactory().CreateThread();
			ConsoleThread->SetRunnable(RunnableConsole);
		}
		else
		{
			Console = new lConsoleNullObject;
		}
		//
		Renderer = new lGL2D_Renderer;
		li2DScene *Scene = new lr2DScene;
		Scene->SetBackgroundColor(lrColor(1.0,1.0,1.0,1.0));
		Renderer->SetScene(Scene);
		li2DCamera *Camera = new lr2DCamera({0.0,0.0},800,600);
		Renderer->SetCamera(Camera);
		#ifdef PAC_MAN
			liGameMode *GameMode = new lPM_Game(ApiAdapter.GetInput(),Scene->GetElementFactory(),0.125);//new lGameMode();
			Game = new lGame(*Console,GameMode,Scene,Camera);
		#else
			liWorld2D *World = new lP2World2D;
			liGameMode *GameMode = new lP2ProtoGameMode(ApiAdapter.GetInput(),*World,*Scene,*Camera);
			Game = new lP2ProtoGame(*Console,GameMode,Scene,Camera,World);
		#endif
    }

    ~lP2_Program()
    {
    	delete Game;
		delete Renderer;
		//
		if(ConsoleThread != nullptr)
			{delete ConsoleThread;}
		//
		delete Console;
    }
};

#include "../lApiAdapter/lSDL2_ApiAdapter/lSDL2_ApiAdapter.h"

static const std::string DEF_TITLE = "A kurva anyadat";
static const bool DEF_FULLSCREEN = false;
static const unsigned int DEF_WIDTH = 800;
static const unsigned int DEF_HEIGHT = 600;
static const lSDL2_Settings::lRenderingApi DEF_RENDERING_API = lSDL2_Settings::OPENGL;

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

	lP2_Program Program(ApiAdapter);

	Program.MainLoop();
	//fclose(stdin);

	return 0;
}
