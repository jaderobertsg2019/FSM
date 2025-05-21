#include "dfaParser.h"

int DfaParser::findStateInListOfStates(string state1)
{
    for (int i = 0; i < listOfStates.size(); i++)
    {
        if (listOfStates[i]->stateName == state1)
            return i;
    }
    return -1;
}

void DfaParser::updateTransition(string input, string state2, int i)
{
    transitionNode *newTransitionNode = new transitionNode;
    newTransitionNode->transitionInput = input;
    newTransitionNode->transitionStateName = state2;
    listOfStates[i]->listOfTransitions.push_back(newTransitionNode);
}

void DfaParser::updateListOfStates(string state1, string input, string state2)
{
    int i = findStateInListOfStates(state1);
    updateTransition(input, state2, i);
}

void DfaParser::fill_out_states_x_input()
{
    for (int i = 0; i < states.size(); i++)
    {
        for (int j = 0; j < inputAlphabet.size(); j++)
        {
            states_x_input_node *newNode = new states_x_input_node;
            newNode->stateName = states[i];
            newNode->input = inputAlphabet[j];
            states_x_input.push_back(newNode);
        }
    }
}

void DfaParser::parse_dfa()
{
    // dfa -> q sigma delta start_state f
    parse_q();
    parse_sigma();
    fill_out_states_x_input();
    parse_delta();
    parse_start_state();
    parse_f();
}

void DfaParser::parse_q()
{
    // q -> Q EQUAL LBRACE list LBRACE SEMICOLON
    expect("Q");
    expect("=");
    expect("{");
    parse_list("Q");
    expect("}");
    expect(";");
}

void DfaParser::parse_sigma()
{
    // sigma -> SIGMA EQUAL LBRACE list RBRACE SEMICOLON
    expect("SIGMA");
    expect("=");
    expect("{");
    parse_list("SIGMA");
    expect("}");
    expect(";");
}

void DfaParser::parse_delta()
{
    // delta -> DELTA EQUAL LBRACE transitions RBRACE SEMICOLON
    expect("DELTA");
    expect("=");
    expect("{");
    parse_transitions();
    expect("}");
    expect(";");
}

void DfaParser::parse_start_state()
{
    // start_state -> START_STATE EQUAL primary SEMICOLON
    expect("START_STATE");
    expect("=");
    string temp = parse_primary();
    if (find(states, temp) == -1)
    {
        syntax_error();
    }
    else
    {
        startState = temp;
    }
    expect(";");
}

void DfaParser::parse_f()
{
    // f	-> F EQUAL LBRACE list RBRACE SEMICOLON
    expect("F");
    expect("=");
    expect("{");
    if (dfaDef != "};")
    {
        parse_list("F");
    }
    expect("}");
    expect(";");
    if (dfaDef.size() != 0)
    {
        syntax_error();
    }
}

void DfaParser::parse_list(string listType)
{
    // list -> primary COMMA list || primary
    string newPrimary = parse_primary();
    if (listType == "Q")
    {
        states.push_back(newPrimary);
        stateNode *temp = new stateNode;
        temp->stateName = newPrimary;
        listOfStates.push_back(temp);
    }
    else if (listType == "SIGMA")
    {
        if (newPrimary.size() == 1)
        {
            inputAlphabet.push_back(newPrimary);
        }
        else
        {
            syntax_error();
        }
    }
    else if (listType == "F")
    {
        finalStates.push_back(newPrimary);
        if (find(states, newPrimary) == -1)
        {
            syntax_error();
        }
    }
    if (peek(1) == ",")
    {
        expect(",");
        parse_list(listType);
    }
}

void DfaParser::parse_transitions()
{
    // transitions -> transiton COMMA transitions || transition
    parse_transition();
    if (peek(1) == ",")
    {
        expect(",");
        parse_transitions();
    }
    if (!states_x_input.empty())
    {
        syntax_error();
    }
}

void DfaParser::validateDeltaInput(pairToRemove *temp)
{
    bool syntaxError = true;
    for (int i = 0; i < states_x_input.size(); i++)
    {
        if (states_x_input[i]->stateName == temp->stateNameToRemove && states_x_input[i]->input == temp->inputToRemove)
        {
            syntaxError = false;
            states_x_input.erase(states_x_input.begin() + i);
        }
    }
    if (syntaxError)
    {
        syntax_error();
    }
}

void DfaParser::parse_transition()
{
    pairToRemove *temp = new pairToRemove;
    // transition->DELTA LPAREN primary COMMA,
    expect("DELTA");
    expect("(");
    string state1 = parse_primary();
    temp->stateNameToRemove = state1;
    if (find(states, state1) == -1)
    {
        syntax_error();
    }
    expect(",");
    string input = parse_primary();
    temp->inputToRemove = input;
    if (find(inputAlphabet, input) == -1)
    {
        syntax_error();
    }
    expect(")");
    expect("=");
    string state2 = parse_primary();
    if (find(states, state2) == -1)
    {
        syntax_error();
    }

    validateDeltaInput(temp);
    updateListOfStates(state1, input, state2);
}

string DfaParser::parse_primary()
{
    // primary -> ID || NUM
    return expect("ID");
}

void DfaParser::clearWhiteSpace()
{
    std::string::iterator end_pos = remove(dfaDef.begin(), dfaDef.end(), ' ');
    dfaDef.erase(end_pos, dfaDef.end());
    end_pos = remove(dfaDef.begin(), dfaDef.end(), '\t');
    dfaDef.erase(end_pos, dfaDef.end());
}

bool DfaParser::shouldStop(int i)
{
    return (dfaDef[i] == '}' || dfaDef[i] == ';' ||
            dfaDef[i] == ',' || dfaDef[i] == ')' ||
            i == dfaDef.size());
}

string DfaParser::expect(string expected)
{
    if (expected == "ID")
    {
        int i = 0;
        while (true)
        {
            if (shouldStop(i))
            {
                break;
            }
            i++;
        }
        if (i == 0)
        {
            syntax_error();
            return NULL;
        }
        string str = dfaDef.substr(0, i);
        dfaDef = dfaDef.substr(i, dfaDef.size());
        return str;
    }
    else
    {
        if (dfaDef.substr(0, expected.size()) == expected)
        {
            string str = dfaDef.substr(0, expected.size());
            dfaDef = dfaDef.substr(expected.size(), dfaDef.size());
            return str;
        }
        else
        {
            syntax_error();
            return NULL;
        }
    }
}

void DfaParser::syntax_error()
{
    cout << "SYNTAX ERROR !";
    exit(1);
}

string DfaParser::peek(int i)
{
    return dfaDef.substr(0, i);
}

int DfaParser::find(vector<string> list, string str)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i] == str)
        {
            return i;
        }
    }
    return -1;
}