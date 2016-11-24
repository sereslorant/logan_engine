
#include "lMenu.h"

unsigned int lMenu::GetSize()
{
    return nMenuOptions;
}

unsigned int lMenu::GetActive()
{
    return ActiveMenuOption;
}

void lMenu::SetActive(unsigned int c)
{
    //int Div = c/nMenuOptions;
    while (c > nMenuOptions)
    {
        c -= nMenuOptions;
    }
    ActiveMenuOption = c;
}

void lMenu::GoUp()
{
    if(ActiveMenuOption == 0)
    {
        ActiveMenuOption = nMenuOptions-1;
    }
    else
    {
        ActiveMenuOption--;
    }
}

void lMenu::GoDown()
{
    ActiveMenuOption++;
    if(ActiveMenuOption >= nMenuOptions)
    {
        ActiveMenuOption = 0;
    }
}

lMenu::lMenu(unsigned int n_menu_options)
    :nMenuOptions(n_menu_options),ActiveMenuOption(0)//,MenuActions(nMenuOptions)
{

}

lMenu::~lMenu()
{

}
