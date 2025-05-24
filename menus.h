#ifndef __MENUS_H__
#define __MENUS_H__

#include <iostream>
#include <vector>
using namespace std;

class Menus
{
private:
    const vector<string> mainMenu = {
        "Options: ",
        "D: input a dfa",
        "N: input an nfa",
        "Q: quit"};

    const vector<string> inputDfaMenu = {
        "enter name of dfa text file"};

    const vector<string> inputNfaMenu = {
        "enter name of nfa text file"};

    const vector<string> dfaOpsMenu = {
        "Options: ",
        "W: run the DFA on an input string w",
        "Q: quit"};

    const vector<string> nfaOpsMenu = {
        "Options: ",
        "W: run the NFA on an input string w",
        "Q: quit"};

    const vector<string> wInMenu = {
        "run M on w"};

    vector<string>
        currentMenu;

public:
    enum MenuType
    {
        MAIN,
        INPUT_DFA,
        INPUT_NFA,
        DFA_OPS,
        NFA_OPS,
        W_IN
    };
    void setMenu(MenuType menu);
    void printCurrentMenu();
};

#endif