
#include "../lGame/lConsole.h"

#include <iostream>

#include <SDL2/SDL.h>

#include "../lApiAdapter/lSDL2_ApiAdapter/lSDL2_Input/lSDL2_Input.h"
#include "../lApiAdapter/lSDL2_ApiAdapter/lSDL2_Thread/lSDL2_Thread.h"


#include "../lInterfaces/lRenderer/li2DRenderer.h"
#include "../lInterfaces/lGame/liGame.h"

#include "lGameModes/lPacMan/lPacMan.h"

#include <list>

#include <SDL2/SDL_opengl.h>

#include "../lRenderer/lr2DScene/lr2DScene.h"




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


class lGL2D_Renderer : public li2DRenderer
{
private:
	/*
	class lGL2D_DrawableFactory : public liDrawableFactory
	{
	private:
		lGL2D_Renderer &Renderer;

	public:

		virtual li2DElement *CreateRect(float x,float y,float width,float height) override
		{
			lr2DRectangle *Rect = new lr2DRectangle({x,y},width,height);
			Renderer.DisplayList.push_back(Rect);

			return Rect;
		}

		lGL2D_DrawableFactory(lGL2D_Renderer &renderer)
			:Renderer(renderer)
		{}

		virtual ~lGL2D_DrawableFactory() override
		{}
	};
	*/

	//std::list<lr2DRectangle *> DisplayList;
	//lGL2D_DrawableFactory DrawableFactory;
	li2DScene *Scene = nullptr;

public:
	/*
	virtual liDrawableFactory &GetDrawableFactory() override
	{
		return DrawableFactory;
	}
	*/
	virtual void SetScene(li2DScene *scene) override
	{
		Scene = scene;
	}
	//
	virtual void Render() override
	{
		if(Scene->HasBackgroundColor())
		{
			glClearColor(Scene->GetBackgroundColor().GetRed(),Scene->GetBackgroundColor().GetGreen(),Scene->GetBackgroundColor().GetBlue(),Scene->GetBackgroundColor().GetAlpha());
			glClear(GL_COLOR_BUFFER_BIT);
		}

        glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
        glOrtho(0,800,600,0,-1,1);
        /*
		for(lr2DRectangle *Rect : DisplayList)
		{
			Rect->Draw();
		}
		*/
		Scene->Draw();
	}

	lGL2D_Renderer()
		//:DrawableFactory(*this)
	{
		glClearColor(1,1,1,1);
		glViewport(0,0,800,600);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_DEPTH_TEST);
		//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual ~lGL2D_Renderer() override
	{
    	/*
    	for(lr2DRectangle *I : DisplayList)
		{
			delete I;
		}
		*/
	}
};

#include "../lGame/lGame.h"
/*
class lGameMode : public liGameMode
{
public:
	virtual void Logic(double dt) override
	{
		std::cout << "8==========D" << std::endl;
	}

	lGameMode(){}
	virtual ~lGameMode() override{}
};
*/
#include "lGame/liWorld2D.h"

class lP2ProtoGameMode : public liGameMode
{
protected:
	liInput &Input;
	//
	liWorld2D &World;
	//li2DRenderer::liDrawableFactory &DrawableFactory;
	li2DScene &Scene;
	//
	constexpr static lmScalar RADIUS = 25.0;
	//
	class lAgent
	{
	protected:
		//
		liBody2D *Body;
		li2DElement *Element;
		//
	public:
		//
		virtual void Step(double dt)
		{
			Element->SetPosition(Body->GetPosition() - lmVector2D({RADIUS,RADIUS}));
		}
		//
		lAgent(liBody2D *body,li2DElement *element)
			:Body(body),Element(element)
		{
			lGetCircle GetCircle;
			Body->GetCollisionShape()->Accept(&GetCircle);
			//
			liCircle *Circle_I = GetCircle.GetCircle();
			Circle_I->SetRadius(RADIUS);
		}
		//
		virtual ~lAgent(){}
	};
	//
	class lPlayerAgent : public lAgent
	{
	protected:
		liController *Controller;
		float Speed = 100.0;
		//
	public:
		//
		virtual void Step(double dt) override
		{
			lmVector2D NewVelocity = {0.0,0.0};
			if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) > 0.01)
			{
				NewVelocity += {0.0, Speed};
			}
			//
			if((Controller->GetAxis(1)->GetValue() / (float)Controller->GetAxis(1)->GetMaxValue()) < -0.01)
			{
				NewVelocity += {0.0,-Speed};
			}
			//
			if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) > 0.01)
			{
				NewVelocity += { Speed,0.0};
			}
			//
			if((Controller->GetAxis(0)->GetValue() / (float)Controller->GetAxis(0)->GetMaxValue()) < -0.01)
			{
				NewVelocity += {-Speed,0.0};
			}
			//
			Body->SetVelocity(NewVelocity);
			//
			lAgent::Step(dt);
		}
		//
		lPlayerAgent(liBody2D *body,li2DElement *element,liController *controller)
			:lAgent(body,element),Controller(controller)
		{}
		//
		virtual ~lPlayerAgent() override
		{}
	};
	//
	std::list<lAgent *> Agents;
	//
public:
	//
	virtual void Logic(double dt) override
	{
		for(lAgent *Agent : Agents)
		{
			Agent->Step(dt);
		}
	}
	//
	lP2ProtoGameMode(liInput &input,liWorld2D &world,li2DScene &scene)
		:Input(input),World(world),Scene(scene)
	{
		if(Input.GetNumControllers() != 0)
		{
			lmVector2D Position = {0.0,0.0};
			lmVector2D Velocity = {0.0,0.0};
			Agents.push_back(new lPlayerAgent(World.CreateBody(Position,Velocity),Scene.GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0),Input.GetController(0)));
		}
		//
		for(unsigned int i=0;i < 5;i++)
		{
			lmVector2D Position = {100 + i*80.0,100.0};
			lmVector2D Velocity = {0.0,0.0};
			Agents.push_back(new lAgent(World.CreateBody(Position,Velocity),Scene.GetElementFactory().CreateRectangle(Position,RADIUS*2.0,RADIUS*2.0)));
		}
	}
	//
	virtual ~lP2ProtoGameMode() override
	{
		for(lAgent *Agent : Agents)
		{
			delete Agent;
		}
	}
};

class lP2ProtoGame : public lGame
{
protected:
	//
	liWorld2D *World;
	bool SimulationPaused = false;
	//
public:
	//
	virtual void Step() override
	{
		if(!SimulationPaused)
		{
			World->Step(dt);
		}
		//
		GameMode->Logic(dt);
	}
	//
	lP2ProtoGame(liConsole &console,liGameMode *game_mode,li2DScene *scene,liWorld2D *world)
		:lGame(console,game_mode,scene),World(world)
	{
		//
	}
	//
	virtual ~lP2ProtoGame() override
	{
		delete World;
	}
};

#include "lGame/lP2World2D.h"

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
			ConsoleThread = new lSDL2_Thread(RunnableConsole);
			//ConsoleThread->SetRunnable(Console);
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
		//*
		liGameMode *GameMode = new lPM_Game(ApiAdapter.GetInput(),Scene->GetElementFactory(),0.125);//new lGameMode();
		Game = new lGame(*Console,GameMode,Scene);
		// */
		/*
		liWorld2D *World = new lP2World2D;
		liGameMode *GameMode = new lP2ProtoGameMode(ApiAdapter.GetInput(),*World,*Scene);
		Game = new lP2ProtoGame(*Console,GameMode,Scene,World);
		// */
    }

    ~lP2_Program()
    {
    	delete Game;
		delete Renderer;
		//
		if(ConsoleThread != nullptr)
			{delete ConsoleThread;}
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
	//
	Program.MainLoop();
	//fclose(stdin);
	//
	return 0;
}
