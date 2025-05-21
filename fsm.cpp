#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "dfaComputer.h"
#include "dfaParser.h"
#include "nfaComputer.h"
#include "nfaParser.h"
using namespace std;

int main()
{
	string userIn = "";

	while (userIn != "q")
	{
		cout << "Options: " << endl;
		cout << "D: input a dfa" << endl;
		cout << "N: input an nfa" << endl;
		cout << "Q: quit" << endl;
		cin >> userIn;
		userIn = tolower(userIn[0]);

		string txt;
		if (userIn == "d")
		{
			cout << "enter name of dfa text file." << endl;
			cin >> txt;
			// read dfa.txt
			fstream newfile;
			newfile.open(txt, ios::in);
			DfaParser dfaparser;
			if (newfile.is_open())
			{
				dfaparser.dfaDef = "";
				// concatenate each line of dfa.txt to dfaDef, ignoring all newlines in text file
				string line;
				while (getline(newfile, line))
				{
					dfaparser.dfaDef += line;
				}
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
				 *		transition	-> DELTA LPAREN primary COMMA,
				 *		primary		-> ID}
				 *
				 *		S = dfa
				 */
				string userInDFA = "";

				while (userInDFA != "q")
				{
					cout << "Options: " << endl;
					cout << "W: run the DFA on an input string w" << endl;
					cout << "Q: quit" << endl;
					cin >> userInDFA;
					userInDFA = tolower(userInDFA[0]);

					string w;
					if (userInDFA == "w")
					{
						cout << "run M on w" << endl;
						DfaComputer executor;
						cin >> w;
						if (executor.startExecution(w, dfaparser))
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
			else
			{
				cout << "something went wrong with the text file" << endl;
			}
		}
		else if (userIn == "n")
		{
			cout << "enter name of nfa text file." << endl;
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
				nfaparser.clearWhiteSpace(); // remove spaces and tabs

				nfaparser.parse_nfa();

				string userInNFA = "";
				while (userInNFA != "q")
				{
					cout << "Options: " << endl;
					cout << "W: run the NFA on an input string w" << endl;
					cout << "Q: quit" << endl;
					cin >> userInNFA;
					userInNFA = tolower(userInNFA[0]);

					string w;
					if (userInNFA == "w")
					{
						cout << "run M on w" << endl;
						NfaComputer executor;
						cin >> w;
						string result = executor.startExecution(w, nfaparser);
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
			else
			{
				cout << "something went wrong with the text file" << endl;
			}
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