#ifndef L_MAIN_MENU
#define L_MAIN_MENU

#include "../lGame.h"

#include "lMenu.h"
#include "lwMenu.h"

#include <vector>
#include <string>

class lMainMenu : public lGame
{
private:

    enum lMenuState
    {
        MAIN_MENU,
        LOAD_MENU,
        OPTIONS,
    };
    /*
    class lMainMenuLogic : public lMenuLogic
    {
    private:
        lGame *Game;
        lMenuState &MenuState;

    public:

        virtual void Logic() override
        {
            switch(Menu->GetActive())
            {
            case 0:
                Game->SetNextMap("TestMap");
                Game->QuitGame();

                // *GameData = new lSSGameData(ShipDatabase);
                //(*GameData)->SetDefault();

                SDL_Delay(200);
                break;
            case 1:
                MenuState = LOAD_MENU;
                SDL_Delay(200);
                break;
            case 2:
                MenuState = OPTIONS;
                SDL_Delay(200);
                break;
            case 3:
                Game->QuitProgram();
                break;
            }
        }

        lMainMenuLogic(lMenu *menu,lGame *game,lMenuState &menu_state):lMenuLogic(menu),Game(game),MenuState(menu_state)
        {

        }
    };
    */

    liGooObject *MenuBg;

    lMenu *Menu;
    lwMenu *MenuGraphics;

    lMenu *LoadMenu;
    lwMenu *LoadMenuGraphics;

	lMenuState MenuState;


	class lEnableMenuCallback : public liEventCallback
	{
	private:
		lMainMenu *MainMenu;
	public:

		virtual void Callback() override
		{
			MainMenu->Enabled = true;
		}

		lEnableMenuCallback(lMainMenu *main_menu)
			:MainMenu(main_menu)
		{}

		virtual ~lEnableMenuCallback() override
		{}
	};

	bool Enabled = true;
	lEnableMenuCallback *EnableMenuCallback;
	liTimer *EnableMenuTimer;

protected:
	void Logic();

public:
    lMainMenu(liApiAdapter *api_adapter,std::string &next_map);
    ~lMainMenu();
};

#endif
