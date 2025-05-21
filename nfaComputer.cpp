#include "nfaComputer.h"
#include <algorithm>

int NfaComputer::findInAlphabet(NfaParser M, string w_ch)
{
    for (int i = 0; i < M.inputAlphabet.size(); i++)
    {
        if (M.inputAlphabet[i] == w_ch)
        {
            return i;
        }
    }
    return -1;
}

int NfaComputer::findCurrentStateInList(NfaParser M, string currentState)
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

vector<int> NfaComputer::findAllTransitions(NfaParser M, int i, string w_ch)
{
    vector<int> temp;
    for (int j = 0; j < M.listOfStates[i]->listOfTransitions.size(); j++)
    {
        if (w_ch == M.listOfStates[i]->listOfTransitions[j]->transitionInput)
        {
            temp.push_back(j);
        }
    }
    return temp;
}

vector<string> NfaComputer::checkEpsilonTransitions(vector<string> currentStates, NfaParser M)
{
    string nextState = "";
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int k = 0; k < currentStates.size(); k++)
        {
            int i = findCurrentStateInList(M, currentStates[k]);
            vector<int> indicesOfEpTrans;
            indicesOfEpTrans = findAllTransitions(M, i, "$");

            for (int j = 0; j < indicesOfEpTrans.size(); j++)
            {
                nextState = M.listOfStates[i]->listOfTransitions[indicesOfEpTrans[j]]->transitionStateName;
                bool unique = true;
                for (int i = 0; i < currentStates.size(); i++)
                {
                    if (currentStates[i] == nextState)
                    {
                        unique = false;
                    }
                }
                if (unique)
                {
                    currentStates.push_back(nextState);
                    changed = true;
                }
            }
        }
    }
    return currentStates;
}

vector<string> NfaComputer::readInputCharacter(vector<string> currentStates, string w, NfaParser M)
{
    string nextState = "";
    string w_ch = w.substr(0, 1);
    if (findInAlphabet(M, w_ch) == -1)
    {
        currentStates.clear();
        currentStates.push_back("USER_INPUT_INVALID");
    }
    else
    {
        vector<string> originalCurrentStates = currentStates;
        vector<string> newCurrentStates;
        for (int k = 0; k < originalCurrentStates.size(); k++)
        {
            string tempLol = originalCurrentStates[k];
            int i = findCurrentStateInList(M, tempLol);
            vector<int> indicesOfChTrans;
            indicesOfChTrans = findAllTransitions(M, i, w_ch);
            for (int j = 0; j < indicesOfChTrans.size(); j++)
            {
                nextState = M.listOfStates[i]->listOfTransitions[indicesOfChTrans[j]]->transitionStateName;
                newCurrentStates.push_back(nextState);
            }
        }
        currentStates = newCurrentStates;
    }
    return currentStates;
}

string NfaComputer::startExecution(string w, NfaParser M)
{
    vector<string> currentStates;
    currentStates.push_back(M.startState);
    while (w.length() != 0)
    {
        // check for epsilon transitions until there are no more epsilon transitions
        currentStates = checkEpsilonTransitions(currentStates, M);
        // now read a character from the input
        currentStates = readInputCharacter(currentStates, w, M);
        if (currentStates.size() == 1 && currentStates[0] == "USER_INPUT_INVALID")
        {
            return currentStates[0];
        }
        w = w.substr(1, w.length());
        currentStates.erase(unique(currentStates.begin(), currentStates.end()), currentStates.end());
    }
    // check one more time before bed
    currentStates = checkEpsilonTransitions(currentStates, M);
    // sort so it looks pretty
    sort(currentStates.begin(), currentStates.end());
    currentStates.erase(unique(currentStates.begin(), currentStates.end()), currentStates.end());

    for (int i = 0; i < currentStates.size(); i++)
    {
        if (std::find(M.finalStates.begin(), M.finalStates.end(), currentStates[i]) != M.finalStates.end())
        {
            return "true";
        }
    }
    return "false";
}