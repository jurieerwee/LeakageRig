/*
 * Main.cpp
 *
 *  Created on: 29 Jun 2015
 *      Author: jurie
 */
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <exception>

#include <unistd.h>

#include "Main.h"


using namespace std;


State mainState = PRE_START;

int main(int argc, const char* argv[])
{
	Main process;

	return process.mainProcess(argc, argv);

}

//Start of class Main

int mainProcess(int argc, const char* argv[])
{
	//Call inits
	State mainState = PRE_START;





	try
	{
		//Continuous loop
		while(1)
		{
			switch(mainState)
			{
			case PRE_START:
				//Prestart code
				nextState();
				break;
			case SETUP:
				//Setup code
				break;
			case PREISOLATE:
				//PreIsolate case (WAITING)
				break;
			case ISOLATE_TEST:
				//Isolation testing
				break;
			case LEAKAGE_TEST:
				//Run leakage test
				break;
			case DATA_PROCESSING:
				//Run data processing
				break;
			case DATA_UPLOAD:
				//Upload data to server
				break;
			case POST_PROCESS:
				//Run post process.  Wait shutdown.  Could send to Prestart againg
				break;
			default:
				//Entered an unknown state!! STOP CORRECTLY
				break;
			}

		}


	}
	catch(const exception e)
	{

	}

	//Decontruction



	return 1;
}

//Change state in an orderly manner to another state
bool changeState(State newState)
{
	//Log change state. New and old

	mainState = newState;

	return true;
}


/*This function controls the flow of states from the one to the next.  */
bool nextState()
{
	switch(mainState)
	{
	case PRE_START:
		changeState(SETUP);
		break;
	case SETUP:
		changeState(PREISOLATE);
		break;
	case PREISOLATE:
		changeState(ISOLATE_TEST);
		break;
	case ISOLATE_TEST:
		changeState(LEAKAGE_TEST);
		break;
	case LEAKAGE_TEST:
		changeState(DATA_PROCESSING);
		break;
	case DATA_PROCESSING:
		changeState(DATA_UPLOAD);
		break;
	case DATA_UPLOAD:
		changeState(POST_PROCESS);
		break;
	case POST_PROCESS:
		changeState(PRE_START);		//TODO: Check that this is correct.  Returns to start
		break;
	default:
		return false;
		break;
	}

	return true;

}


/*Only some state are allowed to change to a previous state.   The others will return false and will not change state.*/
bool prevState()
{
	switch(mainState)
	{
	case PRE_START:
		return false;
		break;
	case SETUP:
		changeState(PREISOLATE);		//Allowed to prevState. Is this correct?
		break;
	case PREISOLATE:
		return false;
		break;
	case ISOLATE_TEST:
		changeState(PREISOLATE);
		break;
	case LEAKAGE_TEST:
		return false;
		break;
	case DATA_PROCESSING:
		return false;
		break;
	case DATA_UPLOAD:
		return false;
		break;
	case POST_PROCESS:
		return false;
		break;
	default:
		return false;
		break;
	}

	return true;

}


