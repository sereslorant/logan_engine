#ifndef L_GAME
#define L_GAME

#include "../lInterfaces/lRenderer/liRenderer.h"

#include "../lGooey/liGooObjectFactory.h"

#include "../lApiAdapter/lDLLFunctions.h"

#include "../lInterfaces/lGameMode/liAgent.h"
#include "../lApiAdapter/lApiAdapter.h"

class lGame
{
private:
    std::string &NextMap;

    bool IsRunning;
    int RetValue;

    bool IsPaused;

protected:
    liApiAdapter *ApiAdapter;

    //Néhány pointer, hogy ne kelljen félméteres kódokat írni.
    lInput          	*Input;
    liFrameLimiter   	*FrameLimiter;
    liRenderer      	*Renderer;
    liGuiRenderer    	*GuiRenderer;
    liResourceManager   *RM;

    liGooObjectFactory	*GooObjectFactory;

    liSimulationWorld   *SimulationWorld;

    std::list<liAgent *> RunningAgents;
    std::list<liAgent *> PausedAgents;

    double dt   = 1.0/60.0;

    liFrustum   *Frustum;
    liCamera    *Camera;
    lrColor     BgColor;

    //liFramebuffer *MainFramebuffer;

    liGooGUI		*HUD;

    bool GetPaused()
    {
        return IsPaused;
    }

    virtual void Logic() = 0;

public:

    void SetNextMap(const std::string &next_map);

    void QuitGame();
    void QuitProgram();

    void SetPaused(bool is_paused);

    int MainLoop();

    lGame(liApiAdapter *api_adapter,std::string &next_map);
    virtual ~lGame();
};

#endif // L_GAME
