#ifndef L_MENU_H
#define L_MENU_H

#include <vector>
#include <list>


class lMenu
{
private:
    unsigned int nMenuOptions;
    unsigned int ActiveMenuOption;

public:
    unsigned int GetSize();
    unsigned int GetActive();

    void SetActive(unsigned int c);

    void GoUp();
    void GoDown();

    lMenu(unsigned int n_menu_options);
    ~lMenu();
};

class lMenuLogic
{
protected:
    lMenu *Menu;

public:

    virtual void Logic() = 0;

    lMenuLogic(lMenu *menu)
    {
        Menu = menu;
    }

    virtual ~lMenuLogic()
    {

    }
};

#endif // L_MENU_H
