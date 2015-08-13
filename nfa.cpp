//Christopher Ray
//Marina Blanton
//CSE 30151
//31 January 2015

//nfa.cpp
//Implementation for nfa class

#include "nfa.h"

nfa::nfa(){}		//basic constructor

int nfa::build_nfa(char * buffer)
{
	//defining local variables
	std::string tempString;		//will convert buffer to std::string for easier comparison and to push back on to the vectors
	char * input = strtok(buffer, " ,:");		//tokenizing buffer
	std::vector<std::string> transBuffer;		//vector used to hold tokenized transition information
	transition temp;		//temporary transition struct that will be pushed on to transition vector

	if(!(strcmp(input, "A")))		//building alphabet
	{
		input = strtok(NULL, " ,:");
		while(input != NULL)		//go through tokenized buffer and add elements to alphabet vector
		{
			tempString = input;
			alphabet.push_back(tempString);
			input = strtok(NULL, " ,:");
		}
	}
	else if(!(strcmp(input, "Q")))		//building states vector
	{
		input = strtok(NULL, " ,:");
		while(input != NULL)		//go through tokenized buffer and add elements to states vector
		{
			tempString = input;
			states.push_back(tempString);
			input = strtok(NULL, " ,:");
		}
	}
	else if(!(strcmp(input, "T")))		//building transition vector
	{
		input = strtok(NULL, " ,:");
		while(input != NULL)		//go through tokenized buffer and add elements to transition vector
		{
			tempString = input;		//converting buffer to std::string
			transBuffer.push_back(tempString);
			if(transBuffer.size() == 3)		//parsing transBuffer for transition information
			{
				temp.currentState = transBuffer[0];
				temp.input = transBuffer[1];
				temp.nextState = transBuffer[2];

				transBuffer.clear();

				transitions.push_back(temp);		//adding new transition struct to transition vector
			}
			input = strtok(NULL, " ,:");
		}
	}
	else if(!(strcmp(input, "S")))		//building start state vector
	{
		input = strtok(NULL, " ,:");
		while(input != NULL)		//go through tokenized buffer and add elements to start state vector
		{
			tempString = input;
			startState.push_back(tempString);
			currentStates.push_back(tempString);
			input = strtok(NULL, " ,:");
		}

	}
	else if(!(strcmp(input, "F")))		//building final state vector
	{
		input = strtok(NULL, " ,:");
		while(input != NULL)		//go through tokenized buffer and add elements to final state vector
		{
			tempString = input;
			finalState.push_back(tempString);
			input = strtok(NULL, " ,:");
		}
	}
	else		//catch any invalid input lines
	{
		std::cout << "nfa:  Error building nfa:  " << buffer << std::endl;
		return -1;
	}

	return 0;
}

int nfa::readInput(char * input)
{
	//defining variables
	char * tokenInput;		//used to tokenize input
	std::string tempString;		//will convert C-style string to std::string for easy comparison and for compatibility with the vectors

	tokenInput = strtok(input, " ,\n");		//tokeninze input
	if(tokenInput == NULL)		//catch any NULL inputs
	{
		std::cout << "nfa:  Error, NULL input" << std::endl;
		return -1;
	}

	while(tokenInput != NULL)		//iterate until there is nothing left to tokenize
	{
		tempString = tokenInput;		//converting to std::string
		if(std::find(alphabet.begin(), alphabet.end(), tempString) != alphabet.end())		//look for input in alphabet
		{
			changeState(tempString, 0);		//perform any necessary transitions
			std::cout << tempString;		//display the input
			display_currentStates();		//display the current states
			tokenInput = strtok(NULL, " ,");
		}
		else		//catch any invalid inputs
		{
			std::cout << "nfa:  Error, input not in alphabet" << tempString << std::endl;
			return -1;
		}
	}

	//print ACCEPT or REJECT depending on whether or not we are in an accept state
	if(testAccept())
	{
		std::cout << "ACCEPT" << std::endl;
	}
	else
	{
		std::cout << "REJECT" << std::endl;
	}

	return 0;
}

void nfa::changeState(std::string myInput, int mode)
{
	//defining variables
	int stop = 0;		//used to break out of while loop if no transitions were made in an iteration
	std::vector<std::string> tempStates = currentStates;		//temporary vector holding the current states
	std::vector<transition>::iterator i;		//used to iterate through transition vector
	std::vector<std::string> restore = currentStates;			//temporary vector used to restore the current states if there were no initial e transitions

	currentStates.clear();		//clear the current states (will build new vector with each transition)

	while(stop == 0)		//iterate until no more new states can be reached
	{
		stop = 1;
		for(i = transitions.begin(); i != transitions.end(); ++i)		//iterate through transition vector
		{
			if((find(tempStates.begin(), tempStates.end(), (*i).currentState)) != tempStates.end())
			{
				if(myInput == (*i).input)
				{
					if((find(currentStates.begin(), currentStates.end(), (*i).nextState)) == currentStates.end())
					{
						currentStates.push_back((*i).nextState);		//add a new state to the currentStates vector if there is a valid transition
						stop = 0;
					}

					if(((find(currentStates.begin(), currentStates.end(), (*i).currentState)) == currentStates.end()) && ("e" == myInput))
					{
						currentStates.push_back((*i).currentState);		//keep a state in the currentStates vector if the transition was made via the empty string
						stop = 0;
					}
				}
			}
		}

		tempStates = currentStates;		//update tempStates for possible next iteration
		myInput = "e";		//we should now only transition via the empty string once the first iteration is done
	}

	if((mode == 1) && (currentStates.size() == 0))
	{
		currentStates = restore;		//restore the currentStates vector if there were no empty string transitions possible from the start state
	}
}

void nfa::reset_currentStates()
{
	currentStates = startState;
}

/*
 * The following functions all have the same basic algorithm:
 * Iterate through the vector
 * Display each element in a specified format
 */

void nfa::display_currentStates()
{
	std::vector<std::string>::iterator i;

	std::cout << "; ";
	for(i = currentStates.begin(); i != currentStates.end(); ++i)
	{
		std::cout << *i;
		if((i + 1) != currentStates.end())
		{
			std::cout << ",";
		}
	}
	std::cout << std::endl;
}

int nfa::testAccept()
{
	std::vector<std::string>::iterator i;

	for(i = finalState.begin(); i != finalState.end(); ++i)
	{
		if(find(currentStates.begin(), currentStates.end(), (*i)) != currentStates.end())
		{
			return 1;		//return 1 if there is a final state in the currentStates vector
		}
	}

	return 0;		//return 0 if there is no final state in the currentStates vector
}

void nfa::display_alphabet()
{
	std::vector<std::string>::iterator i;

	std::cout << "Alphabet:  ";
	for(i = alphabet.begin(); i != alphabet.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
}

void nfa::display_states()
{
	std::vector<std::string>::iterator i;

	std::cout << "States:  ";
	for(i = states.begin(); i != states.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
}

void nfa::display_transitions()
{
	std::vector<transition>::iterator i;

	std::cout << "Transitions:  " << std::endl;
	for(i = transitions.begin(); i != transitions.end(); ++i)
	{
		std::cout << (*i).currentState << " " << (*i).input << " " << (*i).nextState << std::endl;
	}
}

void nfa::display_startState()
{
	std::vector<std::string>::iterator i;

	std::cout << "Start State(s):  ";
	for(i = startState.begin(); i != startState.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
}

void nfa::display_finalState()
{
	std::vector<std::string>::iterator i;

	std::cout << "Final State(s):  ";
	for(i = finalState.begin(); i != finalState.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
}

void nfa::myDebugger()
{
	std::cout << "Debugging" << std::endl;
	display_alphabet();
	display_states();
	display_transitions();
	display_startState();
	display_finalState();
}


