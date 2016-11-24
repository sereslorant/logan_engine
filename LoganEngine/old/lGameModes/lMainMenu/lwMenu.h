#ifndef LW_VERTICAL_MENU
#define LW_VERTICAL_MENU

#include "../../lGooey/lGooey.h"

class lwMenu
{
protected:
    lMenu *Menu;
    unsigned int PrevMenuOption;

    lGooColor ButtonColor;
    lGooColor ActiveColor;

    std::vector<liGooObject *> MenuButtons;

public:
    lwMenu(lMenu *menu,const lGooColor &button_color,const lGooColor &active_color,const std::vector<liGooObject *> &menu_buttons)
        :Menu(menu),ButtonColor(button_color),ActiveColor(active_color),MenuButtons(menu_buttons)
    {
        PrevMenuOption = Menu->GetActive();
        MenuButtons[Menu->GetActive()]->SetColor(ActiveColor);
    }

    virtual ~lwMenu()
    {
        for(auto I : MenuButtons)
        {

        }
    }

    void Hide()
    {
        for(auto I : MenuButtons)
        {
            I->GetAnimation("Hide")->Trigger();
        }
    }

    void Show()
    {
        for(auto I : MenuButtons)
        {
            I->GetAnimation("Show")->Trigger();
        }
    }

    void Update()
    {
        if(PrevMenuOption != Menu->GetActive())
        {
            MenuButtons[PrevMenuOption]->SetColor(ButtonColor);
            MenuButtons[Menu->GetActive()]->SetColor(ActiveColor);

            PrevMenuOption = Menu->GetActive();
        }
    }
};

#endif
