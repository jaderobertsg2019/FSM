#include "nfaParser.h"

int NfaParser::findStateInListOfStates(string state1)
{
    for (int i = 0; i < listOfStates.size(); i++)
    {
        if (listOfStates[i]->stateName == state1)
            return i;
    }
    return -1;
}

void NfaParser::updateTransition(string input, string state2, int i)
{
    transitionNode *newTransitionNode = new transitionNode;
    newTransitionNode->transitionInput = input;
    newTransitionNode->transitionStateName = state2;
    listOfStates[i]->listOfTransitions.push_back(newTransitionNode);
}

void NfaParser::updateListOfStates(string state1, string input, string state2)
{
    int i = findStateInListOfStates(state1);
    updateTransition(input, state2, i);
}

void NfaParser::parse_nfa()
{
    // dfa -> q sigma delta start_state f
    parse_q();
    parse_sigma();
    parse_delta();
    parse_start_state();
    parse_f();
}

void NfaParser::parse_q()
{
    // q -> Q EQUAL LBRACE list LBRACE SEMICOLON
    expect("Q");
    expect("=");
    expect("{");
    parse_list("Q");
    expect("}");
    expect(";");
}

void NfaParser::parse_sigma()
{
    // sigma -> SIGMA EQUAL LBRACE list RBRACE SEMICOLON
    expect("SIGMA");
    expect("=");
    expect("{");
    parse_list("SIGMA");
    expect("}");
    expect(";");
}

void NfaParser::parse_delta()
{
    // delta -> DELTA EQUAL LBRACE transitions RBRACE SEMICOLON
    expect("DELTA");
    expect("=");
    expect("{");
    parse_transitions();
    expect("}");
    expect(";");
}

void NfaParser::parse_start_state()
{
    // start_state -> START_STATE EQUAL primary SEMICOLON
    expect("START_STATE");
    expect("=");
    string newPrimary = parse_primary();
    if (find(states, newPrimary) == -1)
    {
        syntax_error();
    }
    expect(";");
    startState = newPrimary;
}

void NfaParser::parse_f()
{
    // f	-> F EQUAL LBRACE list RBRACE SEMICOLON
    expect("F");
    expect("=");
    expect("{");
    if (nfaDef != "};")
    {
        parse_list("F");
    }
    expect("}");
    expect(";");
    if (nfaDef.size() != 0)
    {
        syntax_error();
    }
}

string NfaParser::peek(int i)
{
    return nfaDef.substr(0, i);
}

void NfaParser::parse_list(string listType)
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

void NfaParser::parse_transitions()
{
    // transitions -> transiton COMMA transitions || transition
    if (peek(2) != "};")
    {
        parse_transition();
    }

    if (peek(1) == ",")
    {
        expect(",");
        parse_transitions();
    }
}

void NfaParser::parse_transition()
{
    // transition->DELTA LPAREN primary COMMA,
    expect("DELTA");
    expect("(");
    string state1 = parse_primary();
    if (find(states, state1) == -1)
    {
        syntax_error();
    }
    expect(",");
    string input = parse_primary();
    if (find(inputAlphabet, input) == -1 && input != "$")
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

    updateListOfStates(state1, input, state2);
}

string NfaParser::parse_primary()
{
    // primary -> ID || NUM
    return expect("ID");
}

bool NfaParser::shouldStop(int i)
{
    return (nfaDef[i] == '}' || nfaDef[i] == ';' ||
            nfaDef[i] == ',' || nfaDef[i] == ')' ||
            i == nfaDef.size());
}

string NfaParser::expect(string expected)
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
        string str = nfaDef.substr(0, i);
        nfaDef = nfaDef.substr(i, nfaDef.size());
        return str;
    }
    else
    {
        if (nfaDef.substr(0, expected.size()) == expected)
        {
            string str = nfaDef.substr(0, expected.size());
            nfaDef = nfaDef.substr(expected.size(), nfaDef.size());
            return str;
        }
        else
        {
            syntax_error();
            return NULL;
        }
    }
}

void NfaParser::syntax_error()
{
    cout << "SYNTAX ERROR !";
    exit(1);
}

void NfaParser::clearWhiteSpace()
{
    std::string::iterator end_pos = remove(nfaDef.begin(), nfaDef.end(), ' ');
    nfaDef.erase(end_pos, nfaDef.end());
    end_pos = remove(nfaDef.begin(), nfaDef.end(), '\t');
    nfaDef.erase(end_pos, nfaDef.end());
}

int NfaParser::find(vector<string> list, string str)
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