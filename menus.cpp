#include "menus.h"

void Menus::setMenu(MenuType menuType)
{
    switch (menuType)
    {
    case MAIN:
        currentMenu = mainMenu;
        break;
    case INPUT_DFA:
        currentMenu = inputDfaMenu;
        break;
    case INPUT_NFA:
        currentMenu = inputNfaMenu;
        break;
    case DFA_OPS:
        currentMenu = dfaOpsMenu;
        break;
    case NFA_OPS:
        currentMenu = nfaOpsMenu;
        break;
    case W_IN:
        currentMenu = wInMenu;
        break;
    }
}

void Menus::printCurrentMenu()
{
    for (int i = 0; i < currentMenu.size(); i++)
    {
        cout << currentMenu[i] << endl;
    }
}