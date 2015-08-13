//Christopher Ray
//Marina Blanton
//CSE 30151
//31 January 2015

//nfa.h
//Header file containing class definition for nfa class.

#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>

struct transition		//defining a transition structure
{
	std::string currentState;		//the current state (the one to transition from)
	std::string input;				//the transition input
	std::string nextState;			//the state to transition to if the given input is detected
};

class nfa		//defining the nfa object
{
	public:
		nfa();		//basic constructor
		int build_nfa(char *);		//function for building the nfa from the given input file
		void myDebugger();			//function I used to determine if nfa was properly built
		int readInput(char *);		//function for reading/interpretting transition input
		void display_currentStates();		//function for displaying the current states
		void reset_currentStates();			//function for resetting to start state
		void changeState(std::string, int);		//function for transitioning between states

	private:
		std::vector<std::string> alphabet;		//vector containing the nfa's alphabet
		std::vector<std::string> states;		//vector containing the nfa's states
		std::vector<transition> transitions;		//vector containing the nfa's transitions
		std::vector<std::string> startState;		//vector containing the start state
		std::vector<std::string> finalState;		//vector containing the accept states
		std::vector<std::string> currentStates;		//vector containing the current states

		void display_alphabet();		//function for displaying alphabet (only used to debug)
		void display_states();			//function for displaying nfa states (only for debugging)
		void display_transitions();		//function for displaying nfa transitions (only for debugging)
		void display_startState();		//function for displaying start state (only for debugging)
		void display_finalState();		//function for displaying final states (only for debugging)
		int testAccept();		//function that checks if the nfa is in an accept state
};

#endif
