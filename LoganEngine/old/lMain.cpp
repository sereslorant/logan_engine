#include <iostream>

/** \mainpage A Logan engine kezdetleges fizikai motorjának prototípusa.
 * Az lInterface-ben az SDL2-t használó interfész, az lRendererben 1-2 class és
 * az lResourceManagerben a fájlbetöltő classok nincsenek dokumentálva.
 *
 * Érdeklődőknek az OpenGL-es renderer működésének megértéséhez érdemes megnézni a thecplusplusguy OpenGL és GLSL videóit: https://www.youtube.com/user/thecplusplusguy/playlists
 *
 * \todo sok classhoz elférne egy detailed description, aminek a megírására a vizsgaidőszakban nincs idő. :/
 *
 * \bug Az ütközésellenőrző a mesh-labda collisionnél bugos.
 */

#include "lGameModes/lGame.h"

#include "lGameModes/lMainMenu/lMainMenu.h"
#include "lGameModes/lTechDemo/lTechDemo.h"

using namespace std;

class lProgram
{
private:
    liApiAdapter *ApiAdapter;

    std::string NextMap;

    lGame *LoadGameMode()
    {
        if(NextMap == "MainMenu")
        {
            lGame *game = new lMainMenu(ApiAdapter,NextMap);
            return game;
        }
        if(NextMap == "TechDemo")
        {
            lGame *game = new lTechDemo(ApiAdapter,NextMap);
            return game;
        }
        else
        {
            //return new lGame(ApiAdapter,NextMap);
        }
        /*
        break;

        default:
            cerr < "Error while loading game mode: game mode not found\n";
        }
        */
    }

public:
    lProgram(liApiAdapter *api_adapter)
        :ApiAdapter(api_adapter),NextMap("MainMenu")
    {

    }

    ~lProgram()
    {

    }

    void Start()
    {
        int RetValue = 0;
        lGame *Game;

        while(true)
        {
            if(RetValue == -1)
            {
                break;
            }
            else
            {
                Game = LoadGameMode();
            }

            RetValue = Game->MainLoop();

            delete Game;
        }
    }
};

#include "lApiAdapter/lSDL2ApiAdapter/lSDL2ApiAdapter.h"

#include "lTest/lTest.h"

int main(int argc, char **argv)
{
	//Test();
	//SDL_Delay(60000);

    lSDL2ApiAdapter *ApiAdapter = new lSDL2ApiAdapter("Settings.conf");
    ApiAdapter->PrintStatus(cerr);

    if(ApiAdapter->GetError() != lSDL2ApiAdapter::ERR_OK)
    {
        delete ApiAdapter;
        return 1;
    }

    lProgram Program(ApiAdapter);
    Program.Start();

    delete ApiAdapter;
    return 0;
}
