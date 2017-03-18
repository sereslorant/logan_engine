


#include "lSDL2_ApiAdapter.h"
/*
int lSDL2ApiAdapter::GetError()
{
    return Error;
}
*/
/*
void lSDL2ApiAdapter::SwapBuffers()
{
    SDL_GL_SwapWindow(Window);
}
*/
/*
extern "C" {

liResourceManager *lrmCreateResourceManager(lmScalar *def_diffuse,lmScalar *def_specular,lmScalar def_shininess);

liSimulationWorld *lpCreateSimulationWorld();

bool lrmLoadText(const std::string &filename,std::string &destination);
}
*//*
lSDL2ApiAdapter::lSDL2ApiAdapter(const string& settingsFile):Window(nullptr),RenderingContext(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Error = ERR_OK;

    Settings = new lSDL2Settings(settingsFile);

    int Flags = 0;
    Settings->SetFlags(&Flags);

    if((Settings->GetRenderingAPI() == lSDL2Settings::LRAPI_OPENGL) || (Settings->GetRenderingAPI() == lSDL2Settings::LRAPI_OPENGL_ES))
    {
        lSDL2GLContext *GLContext;

        if(Settings->GetRenderingAPI() == lSDL2Settings::LRAPI_OPENGL_ES)
        {
            GLContext = new lSDL2GLContext(2,0,true);
        }
        else
        {
            GLContext = new lSDL2GLContext(3,3,false);
        }

        GLContext->SetAttributes();

        Window = SDL_CreateWindow(Settings->GetTitle().c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,Settings->GetWidth(),Settings->GetHeight(),Flags);
        if(Window == nullptr)
        {
            Error |= ERR_COULDNT_CREATE_WINDOW;
            return;
        }

        GLContext->CreateContext(Window);
        if(GLContext->GetError() != lSDL2GLContext::ERR_OK)
        {
            Error |= ERR_COULDNT_CREATE_RENDERING_CONTEXT;
            delete GLContext;
            return;
        }

        RenderingContext = GLContext;
    }

    SDL_ShowWindow(Window);

    /*
    LIB_Math = SDL_LoadObject(LIB_MATH_NAME.c_str());

	if(LIB_Math != nullptr)
	{
		std::cout << "Math DLL loaded" << std::endl;
	}
	else
	{std::cout << SDL_GetError() << std::endl;}
	* /

    LIB_ResourceManager = SDL_LoadObject(LIB_RESOURCE_MANAGER_NAME.c_str());

	if(LIB_ResourceManager != nullptr)
	{
		std::cout << "Resource manager DLL loaded" << std::endl;

		CreateResourceManager = (lfptrCreateResourceManager) SDL_LoadFunction(LIB_ResourceManager,"lrmCreateResourceManager");

		if(CreateResourceManager != nullptr)
		{
			std::cout << "lrmCreateResourceManager function pointer loaded" << std::endl;
		}
		else
		{
			std::cout << SDL_GetError() << std::endl;
			//CreateResourceManager = lrmCreateResourceManager;
		}

		//lLoadText = (lfptrLoadText) SDL_LoadFunction(LIB_ResourceManager,"lrmLoadText");

		if(CreateResourceManager != nullptr)
		{
			std::cout << "lrmLoadText function pointer loaded" << std::endl;
		}
		else
		{
			std::cout << SDL_GetError() << std::endl;
			//CreateResourceManager = lrmCreateResourceManager;
		}
	}
	else
	{
		std::cout << SDL_GetError() << std::endl;
		//CreateResourceManager = lrmCreateResourceManager;
	}

    LIB_PhysicsEngine = SDL_LoadObject(LIB_PHYSICS_ENGINE_NAME.c_str());

	if(LIB_PhysicsEngine != nullptr)
	{
		std::cout << "Physics engine DLL loaded" << std::endl;

		//CreateCollisionDetector	= (lfptrCreateCollisionDetector)SDL_LoadFunction(LIB_PhysicsEngine,"lpCreateDiscreteCollisionDetector");
		//CreateInteractionModel	= (lfptrCreateInteractionModel)SDL_LoadFunction(LIB_PhysicsEngine,"lpCreateImpulseInteractionModel");
		//CreateIntegrator		= (lfptrCreateIntegrator)SDL_LoadFunction(LIB_PhysicsEngine,"lpCreateRK4Integrator");
		CreateSimulationWorld	= (lfptrCreateSimulationWorld)SDL_LoadFunction(LIB_PhysicsEngine,"lpCreateSimulationWorld");
	}
	else
	{std::cout << SDL_GetError() << std::endl;}
	/*
	CreateResourceManager = lrmCreateResourceManager;
	lLoadText = lrmLoadText;

	CreateSimulationWorld = lpCreateSimulationWorld;
	* /
    Input = new lSDL2Input;

    float DefDiffuse[3]     = {0.0,0.0,0.0};
    float DefSpecular[3]    = {0.0,0.0,0.0};
    float DefShininess      = 100.0;

    FrameLimiter = new lSDL2FrameLimiter(60);

    ResourceManager = CreateResourceManager(DefDiffuse,DefSpecular,DefShininess);

    RenderingContext->CreateRenderer(&Renderer);
    //ResourceManager->BindRenderer(Renderer);
    Renderer->SetRM(ResourceManager);
    Renderer->CreateGuiRenderer(&GuiRenderer);
}

lSDL2ApiAdapter::~lSDL2ApiAdapter()
{
	delete Input;
	delete FrameLimiter;
	delete Renderer;
	delete ResourceManager;

	//SDL_UnloadObject(LIB_Math);

    delete Settings;
    delete RenderingContext;

    SDL_UnloadObject(LIB_ResourceManager);
    SDL_UnloadObject(LIB_PhysicsEngine);

    SDL_DestroyWindow(Window);
    SDL_Quit();
}
*/
