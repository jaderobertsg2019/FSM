#include "dfaComputer.h"

int DfaComputer::findCurrentStateInList(DfaParser M, string currentState)
{
    for (int i = 0; i < M.listOfStates.size(); i++)
    {
        if (currentState == M.listOfStates[i]->stateName)
        {
            return i;
        }
    }
    return -1;
}

int DfaComputer::findTransitionInList(DfaParser M, int i, string w_ch)
{
    for (int j = 0; j < M.listOfStates[i]->listOfTransitions.size(); j++)
    {
        if (w_ch == M.listOfStates[i]->listOfTransitions[j]->transitionInput)
        {
            return j;
        }
    }
    return -1;
}

bool DfaComputer::startExecution(string w, DfaParser M)
{
    string currentState = M.startState;
    string nextState = "";
    while (w.length() != 0)
    {
        string w_ch = w.substr(0, 1);
        w = w.substr(1, w.length());
        int i = findCurrentStateInList(M, currentState);
        int j = findTransitionInList(M, i, w_ch);
        if (i == -1 || j == -1)
        {
            cout << "invalid input string";
            exit(-1);
        }
        currentState = M.listOfStates[i]->listOfTransitions[j]->transitionStateName;
    }

    for (int i = 0; i < M.finalStates.size(); i++)
    {
        if (M.finalStates[i] == currentState)
        {
            return true;
        }
    }
    return false;
}