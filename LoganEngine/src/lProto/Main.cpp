
#include "../lGame/lConsole.h"
#include "lGameModes/lPacMan/lPacMan.h"

#include <SDL2/SDL_opengl.h>

#include "../lInterfaces/lRenderer/li2DRenderer.h"

class lrLayer : public liLayer
{
private:
	bool Enabled = false;

	virtual void DrawScene() = 0;

public:

	virtual void Enable() override
	{
		Enabled = true;
	}

	virtual void Disable() override
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
	virtual ~lrLayer() override{}
};


class lrViewport : public liViewport
{
protected:
	int X;
	int Y;
	int Width;
	int Height;

public:

	virtual int GetX() override
	{
		return X;
	}

	virtual int GetY() override
	{
		return Y;
	}

	virtual void SetX(int x) override
	{
		X = x;
	}

	virtual void SetY(int y) override
	{
		Y = y;
	}

	virtual int GetWidth() override
	{
		return Width;
	}

	virtual int GetHeight() override
	{
		return Height;
	}

	virtual void SetWidth(int width) override
	{
		Width = width;
	}

	virtual void SetHeight(int height) override
	{
		Height = height;
	}

	lrViewport(int x,int y,int width,int height)
		:X(x),Y(y),Width(width),Height(height)
	{}

	virtual ~lrViewport() override
	{}
};


class lffGL2D_SceneDrawer : public li2DSceneDrawer, public li2DSceneVisitor
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
	lffGL2D_SceneDrawer(){}
	virtual ~lffGL2D_SceneDrawer() override {}
};

class lffGL2D_Layer : public lrLayer,public li2DLayer
{
private:
	li2DScene *Scene = nullptr;
	li2DCamera *Camera = nullptr;

	virtual void DrawScene() override
	{
		if((Scene == nullptr) || (Camera == nullptr))
		{return;}
		//
		glDisable(GL_DEPTH_TEST);
		//
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
		//
		glMatrixMode(GL_MODELVIEW);
		//
		lffGL2D_SceneDrawer SceneDrawer;
		Scene->Draw(SceneDrawer);
	}

public:

	virtual void Accept(liLayerVisitor &layer_visitor) override
	{
		layer_visitor.Visit2DLayer(*this);
	}

	virtual void SetScene(li2DScene *scene) override
	{
		Scene = scene;
	}

	virtual void SetCamera(li2DCamera *camera) override
	{
		Camera = camera;
	}

	lffGL2D_Layer(){}
	virtual ~lffGL2D_Layer() override {}
};

class lrGLViewport : public lrViewport
{
private:
	std::list<lrLayer *> Layers;

public:

	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = new lffGL2D_Layer;
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}

	void Draw()
	{
		glViewport(X,Y,Width,Height);
		//
		for(lrLayer *Layer : Layers)
		{
			Layer->Draw();
		}
	}

	lrGLViewport(int x,int y,int width,int height)
		:lrViewport(x,y,width,height)
	{}

	virtual ~lrGLViewport() override
	{
		for(lrLayer *Layer : Layers)
		{
			delete Layer;
		}
	}
};

class lrFramebuffer : public liFramebuffer
{
protected:
	unsigned int Width;
	unsigned int Height;

public:
	//
	virtual unsigned int GetWidth()
	{
		return Width;
	}
	//
	virtual unsigned int GetHeight()
	{
		return Height;
	}

	lrFramebuffer(unsigned int width,unsigned int height)
		:Width(width),Height(height)
	{}
	virtual ~lrFramebuffer() override
	{}
};

class lffGLFramebuffer : public lrFramebuffer
{
private:
	std::list<lrGLViewport *> Viewports;

public:

	virtual liViewport *CreateViewport(int x,int y,int width,int height) override
	{
		lrGLViewport *NewViewport = new lrGLViewport(x,y,width,height);
		Viewports.push_back(NewViewport);
		//
		return NewViewport;
	}

	void Draw()
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			Viewport->Draw();
		}
	}

	lffGLFramebuffer(unsigned int width,unsigned int height)
		:lrFramebuffer(width,height)
	{}

	virtual ~lffGLFramebuffer() override
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			delete Viewport;
		}
	}
};

class lffGL2D_Renderer : public li2DRenderer
{
private:
	//
	//li2DScene *Scene = nullptr;
	//lffGL2D_Layer Layer;
	lffGLFramebuffer MainFramebuffer;
	//
public:
	/*
	virtual void SetScene(li2DScene *scene) override
	{
		Layer.SetScene(scene);
	}

	virtual void SetCamera(li2DCamera *camera) override
	{
		Layer.SetCamera(camera);
	}
	*/
	virtual liFramebuffer &GetMainFramebuffer() override
	{
		return MainFramebuffer;
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
		MainFramebuffer.Draw();
	}

	lffGL2D_Renderer()
		:MainFramebuffer(800,600)
	{
		//glClearColor(1,1,1,1);
		glShadeModel(GL_SMOOTH);
		//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Layer.Enable();
	}

	virtual ~lffGL2D_Renderer() override
	{
    	//
	}
};

#include "lGame/lP2Game.h"
#include "lGame/lP2World2D.h"

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
		Renderer = new lffGL2D_Renderer;
		//
		#ifdef PAC_MAN
			liGameMode *GameMode = new lPM_Game(ApiAdapter.GetInput(),*Renderer,0.125);//new lGameMode();
			Game = new lGame(*Console,GameMode,*Renderer);
		#else
			liWorld2D *World = new lP2World2D;
			liGameMode *GameMode = new lP2ProtoGameMode(ApiAdapter.GetInput(),*World,*Renderer);
			Game = new lSimulationGame<liWorld2D>(*Console,GameMode,*Renderer,World);
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
