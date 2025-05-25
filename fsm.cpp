#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "menus.h"
#include "dfaComputer.h"
#include "dfaParser.h"
#include "nfaComputer.h"
#include "nfaParser.h"
using namespace std;

void dfaParsing(DfaParser dfaparser)
{
	dfaparser.clearWhiteSpace(); // remove spaces and tabs
	dfaparser.parse_dfa();
	/*
	 * parse dfa according to the CFG G (V, SIGMA, R, S)
	 * where V = {dfa, q, sigma, delta, start_state, f, list, transitions, transition, primary},
	 *		SIGMA = {Q, SIGMA, DELTA, START_STATE, F, EQUAL, LBRACE, RBRACE, SEMICOLON, LPAREN, RPAREN, ID, NUM},
	 *		R = {
	 *
	 *		dfa			-> q sigma delta start_state f,
	 *		q			-> Q EQUAL LBRACE list LBRACE SEMICOLON,
	 *		sigma		-> SIGMA EQUAL LBRACE list RBRACE SEMICOLON,
	 *		delta		-> DELTA EQUAL LBRACE transitions RBRACE SEMICOLON,
	 *		start_state	-> START_STATE EQUAL primary SEMICOLON,
	 *		f			-> F EQUAL LBRACE list RBRACE SEMICOLON,
	 *		list		-> primary COMMA list || primary,
	 *		transitions	-> transiton COMMA transitions || transition,
	 *		transition	-> DELTA LPAREN primary COMMA primary RPAREN EQUALS primary,
	 *		primary		-> ID || NUM}
	 *
	 *		S = dfa
	 */
}

void dfaComputing(Menus menus, DfaParser dfaparser)
{
	string userInDFA = "";
	while (userInDFA != "q")
	{
		menus.setMenu(Menus::DFA_OPS);
		menus.printCurrentMenu();
		cin >> userInDFA;
		userInDFA = tolower(userInDFA[0]);

		string w;
		if (userInDFA == "w")
		{
			menus.setMenu(Menus::W_IN);
			menus.printCurrentMenu();
			DfaComputer computer;
			cin >> w;
			if (computer.startExecution(w, dfaparser))
				cout << "M accepts w" << endl;
			else
				cout << "M does not accept w" << endl;
		}
		else if (userInDFA == "q")
		{
			cout << "done with this dfa." << endl;
		}
		else
		{
			cout << "invalid input, try again" << endl;
		}
	}
}

void dfaHandling(Menus menus)
{
	string txt;
	menus.setMenu(Menus::INPUT_DFA);
	menus.printCurrentMenu();
	cin >> txt;
	// read dfa.txt
	fstream newfile;
	newfile.open(txt, ios::in);
	if (newfile.is_open())
	{
		DfaParser dfaparser;
		dfaparser.dfaDef = "";
		// concatenate each line of dfa.txt to dfaDef, ignoring all newlines in text file
		string line;
		while (getline(newfile, line))
		{
			dfaparser.dfaDef += line;
		}
		dfaParsing(dfaparser);
		dfaComputing(menus, dfaparser);
	}
	else
	{
		cout << "something went wrong with the text file" << endl;
	}
}

void nfaParsing(NfaParser nfaparser)
{
	nfaparser.clearWhiteSpace(); // remove spaces and tabs
	nfaparser.parse_nfa();
}

void nfaComputing(Menus menus, NfaParser nfaparser)
{
	string userInNFA = "";
	while (userInNFA != "q")
	{
		menus.setMenu(Menus::NFA_OPS);
		menus.printCurrentMenu();
		cin >> userInNFA;
		userInNFA = tolower(userInNFA[0]);

		string w;
		if (userInNFA == "w")
		{
			menus.setMenu(Menus::W_IN);
			menus.printCurrentMenu();
			NfaComputer computer;
			cin >> w;
			string result = computer.startExecution(w, nfaparser);
			if (result == "true")
			{
				cout << "M accepts w" << endl;
			}
			else if (result == "false")
			{
				cout << "M does not accept w" << endl;
			}
			else
			{
				cout << "input string contained one or more characters not present in SIGMA" << endl;
			}
		}
		else if (userInNFA == "q")
		{
			cout << "done with this nfa." << endl;
		}
		else
		{
			cout << "invalid input, try again" << endl;
		}
	}
}

void nfaHandling(Menus menus)
{
	string txt;
	menus.setMenu(Menus::INPUT_NFA);
	menus.printCurrentMenu();
	cin >> txt;
	// read nfa.txt
	fstream newfile;
	newfile.open(txt, ios::in);
	NfaParser nfaparser;
	if (newfile.is_open())
	{
		nfaparser.nfaDef = "";
		// concatenate each line of dfa.txt to dfaDef, ignoring all newlines in text file
		string line;
		while (getline(newfile, line))
		{
			nfaparser.nfaDef += line;
		}
		nfaParsing(nfaparser);
		nfaComputing(menus, nfaparser);
	}
	else
	{
		cout << "something went wrong with the text file" << endl;
	}
}

int main()
{
	string userIn = "";
	Menus menus;
	while (userIn != "q")
	{
		menus.setMenu(Menus::MAIN);
		menus.printCurrentMenu();
		cin >> userIn;
		userIn = tolower(userIn[0]);

		if (userIn == "d")
		{
			dfaHandling(menus);
		}
		else if (userIn == "n")
		{
			nfaHandling(menus);
		}
		else if (userIn == "q")
		{
			cout << "goodbye";
		}
		else
		{
			cout << "invalid input, try again" << endl;
		}
	}
}