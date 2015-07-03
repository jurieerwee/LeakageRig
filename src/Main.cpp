/*
 * Main.cpp
 *
 *  Created on: 29 Jun 2015
 *      Author: jurie
 */
#define BOOST_LOG_DYN_LINK 1
 
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdexcept>
#include <boost/log/trivial.hpp>
/*#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>*/
#include <iostream>
#include <map>
#include <string>

/*namespace logging = boost::log;
namespace src = boost::log::sources;*/

#include <unistd.h>

#include "TestData.h"
#include "Rig.h"
#include "LeakageTest.h"
#include "Main.h"



using namespace std;


State mainState = PRE_START;

//BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt<>)

int main(int argc, const char* argv[])
{
	Main process;

	return process.mainProcess(argc, argv);

}

//Start of class Main
Main::Main()
{
}

int Main::mainProcess(int argc, const char* argv[])
{
	//Call inits
//	this->initLogger();
	State mainState = PRE_START;
	TestData dataset(10);
	Rig rig = Rig(100.);
	double testPressures[] = {100.,150.,200.,250.};
	LeakageTest leakageTest = LeakageTest(&rig, &dataset, 60, 30, 6,testPressures,4);
	
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Initialization DONE";
	BOOST_LOG_TRIVIAL(trace) << "Initialization DONE";
	int reply =2;

	//BOOST_LOG_SEV(this->lg,NORMAL) << "Entering continual loop";

//	try
	//{
		//Continuous loop
		while(1)
		{
			switch(mainState)
			{
			case PRE_START:
				//Prestart code
				this->nextState();
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
				reply = leakageTest.call();
				if(2==reply)
				{
					//Crap...
					
				}
				else if(1==reply)
					this->nextState();
				else if(-1)
					this->prevState();
				
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


//	}
	//catch(const exception e)
	//{

	//}

	//Decontruction

	//BOOST_LOG_SEV(this->lg, NORMAL) << "Program at end. Final log.";

	return 1;
}

//Change state in an orderly manner to another state
bool Main::changeState(State newState)
{
	//Log change state. New and old
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Change mainState from \"" << mainState << "\" to \""<< newState << "\"";

	mainState = newState;

	return true;
}


/*This function controls the flow of states from the one to the next.  */
bool Main::nextState()
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
bool Main::prevState()
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
/*
bool Main::initLogger()
{
	this->lg = my_logger::get();
	return true;
}
*/

