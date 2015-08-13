//Christopher Ray
//Marina Blanton
//CSE 30151
//31 January 2015

//main.cpp
//Main driver program for the program "nfa"

#include "nfa.h"

using namespace std;

int main(int argc, char * argv[])
{
	//checking for correct number of arguments
	if(argc <= 1)
	{
		cout << "nfa:  Not enough arguments" << endl;
		cout << "usage:  ./nfa InputFile" << endl;
		return 1;
	}
	else if(argc > 2)
	{
		cout << "nfa:  Too many arguments" << endl;
		cout << "usage:  ./nfa InputFile" << endl;
		return 1;
	}

	//defining variables
	fstream inputFile;		//input file stream
	char buffer[1000];		//C-string buffer
	nfa myNFA;		//nfa bject
	int numInputs = 0;		//number of tape inputs for the nfa
	int i = 0;

	memset(buffer, '\0', sizeof(buffer));		//clearing buffer

	inputFile.open(argv[1]);		//opening the input stream
	if(!(inputFile.is_open()))		//end the program if there was an error opening the stream
	{
		cout << "nfa:  Error opening " << argv[1] << endl;
		return 1;
	}

	inputFile.getline(buffer, sizeof(buffer));		//read from the stream
	while(1)
	{
		if(!(inputFile.good()))
		{
			if(inputFile.eof())
			{
				memset(buffer, '\0', sizeof(buffer));		//end the loop if we have reached the end of the file
				break;
			}
			else
			{
				cout << "nfa:  Error with " << argv[1] << " stream" << endl;		//end the program if there was a read error
				return 1;
			}
		}

		if(myNFA.build_nfa(buffer) < 0)		//end the program if there was an error building some part of the nfa
		{
			return 1;
		}

		memset(buffer, '\0', sizeof(buffer));		//clearing buffer for next set of reads
		inputFile.getline(buffer, sizeof(buffer));		//read from the stream
	}

	inputFile.close();		//closing the input stream

	cin >> numInputs;		//reading in the number of tape inputs from the standard input
	if(!(cin.good()))		//checking if the stream is good
	{
		cout << "nfa:  Error, invalid argument # input:  " << numInputs << std::endl;
		cout << "Please input an integer" << endl;
		return 1;
	}

	for(i = 0; i < numInputs; i++)		//iterate through the tape inputs
	{
		cin >> buffer;		//read in an input
		if(!(cin.good()))		//checking if there was a read error
		{
			cout << "nfa:  Error with input arguments" << endl;
			cout << "Argument example:  1,1,1" << endl;
			return 1;
		}

		//display initial state of the machine plus any transitions due to the empty string
		myNFA.changeState("e", 1);
		myNFA.display_currentStates();

		//attempt to interpret tape input
		if(myNFA.readInput(buffer) < 0)
		{
			return 1;		//end the program if there was an error with the tape input
		}
		else
		{
			myNFA.reset_currentStates();		//reset nfa for new tape input
			cout << endl;
		}

		memset(buffer, '\0', sizeof(buffer));		//clear buffer
	}

	//myNFA.myDebugger();

	return 0;
}
