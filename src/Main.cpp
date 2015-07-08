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
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/manipulators/to_log.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <signal.h>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace po = boost::program_options;

#include <unistd.h>

#include "TestData.h"
#include "Rig.h"
#include "LeakageTest.h"
#include "Main.h"



using namespace std;



int main(int argc, const char* argv[])
{
	mainSpace::Main process;

	return process.mainProcess(argc, argv);

}

namespace mainSpace
{
//State mainState = PRE_START;
//Start of class Main
Main::Main()  : lg(my_logger::get())
{
	//this->lg = my_logger::get();
}

bool terminateNow = false;

void terminateC(int sig)
{
	terminateNow = true;

}

int Main::mainProcess(int argc, const char* argv[])
{
	//Call inits
	this->initLogger();
	this->initOptions();
	TestData dataset(10);
	//Rig rig = Rig(this->vm["pumpFullSpeed"].as<int>());
	Rig rig(this->vm);
	double testPressures[] = {100.,150.,200.,250.};
	
	LeakageTest::LeakageTest leakageTest = LeakageTest::LeakageTest(&rig, &dataset, this->vm["settleTime"].as<int>(),this->vm["pressureMeasureInterval"].as<int>(),\
			this->vm["pressureMeasureCount"].as<int>(),&this->vm["testPressure"].as<vector<double>>()[0],this->vm["testPressure"].as<vector<double>>().size());

	//src::severity_logger_mt<severity_level> lg;

	BOOST_LOG_SEV(lg,logging::trivial::info) << "Initialization DONE";
	//BOOST_LOG_TRIVIAL(trace) << "Initialization DONE";
	int reply =2;


	BOOST_LOG_SEV(lg,logging::trivial::info) << "Entering continual loop";

	//TODO: REmove die
	this->mainState = LEAKAGE_TEST;

	signal(SIGINT,terminateC);

	//Continuous loop
		while(1)
		{
			if(terminateNow)
			{
				BOOST_LOG_SEV(lg,logging::trivial::warning) << "Termination initiated";
				rig.shutdown();

				if(this->mainState == LEAKAGE_TEST)
				{
					BOOST_LOG_SEV(lg,logging::trivial::warning) << "Termination call during LEAKAGE TEST.  Ending test prematurely, but continuing to next state";
					terminateNow = false;
					this->nextState();
				}

				break;
			}

			switch(this->mainState)
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
					BOOST_LOG_SEV(lg,logging::trivial::error) << "Leakage test returned error!";
					exit(1);
					
				}
				else if(1==reply)
					this->nextState();
				else if(-1==reply)
					this->prevState();
				
				break;
			case DATA_PROCESSING:
				//Run data processing
				exit(0);
				break;
			case DATA_UPLOAD:
				//Upload data to server
				break;
			case POST_PROCESS:
				//Run post process.  Wait shutdown.  Could send to Prestart again
				break;
			default:
				//Entered an unknown state!! STOP CORRECTLY
				break;
			}

		}




	//Decontruction

	BOOST_LOG_SEV(this->lg, logging::trivial::info) << "Program at end. Final log.";

	return 1;
}

//Change state in an orderly manner to another state
bool Main::changeState(State newState)
{
	//Log change state. New and old
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Change mainState from \"" << this->mainState << "\" to \""<< newState << "\"";

	this->mainState = newState;

	return true;
}


/*This function controls the flow of states from the one to the next.  */
bool Main::nextState()
{

	switch(this->mainState)
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

	switch(this->mainState)
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

bool Main::initLogger()
{
	// http://stackoverflow.com/questions/15853981/boost-log-2-0-empty-severity-level-in-logs (6/7/2015)
	//boost::log::register_simple_formatter_factory< logging::trivial::severity_level, char >("Severity");
	//logging::register_simple_formatter_factory<severity_level, char>("Severity");
	logging::add_file_log(logging::keywords::file_name="sample%N.log",logging::keywords::format = "[%TimeStamp%] <%Severity%>: %Message%", logging::keywords::auto_flush = true);
	logging::add_console_log(cout);
	logging::add_common_attributes();
	//this->lg = src::severity_logger_mt<severity_level>;
	return true;
}

bool Main::initOptions()
{
	po::options_description desc("System parameters");
	desc.add_options()
			("pumpFullSpeed",po::value<int>()->default_value(3700),"The pumps full speed in rpm")
			("pumpFullPressure",po::value<double>(),"The pump's 'pressure assosiated with full speed")
			("settleTime",po::value<int>()->default_value(60),"Leakage test settle time in seconds")
			("pressureMeasureInterval",po::value<int>()->default_value(30),"Time in seconds between pressure measurements")
			("pressureMeasureCount",po::value<int>()->default_value(6),"Number of pressure measurements to sample")
			("testPressure",po::value<vector<double>>()->composing(),"Test points for leakage test (pressures)")
			("tankFullPin",po::value<int>(),"WiringPi pin connected to tankFull sensor")
			("tankFullNO",po::value<int>()->default_value(1),"Set to 1 if N.C. sensor, else 0")
			("tankEmptyPin",po::value<int>(),"WiringPi pin connected to tankEmpty sensor")
			("tankEmptyNO",po::value<int>()->default_value(1),"Set to 1 if N.C. sensor, else 0")
			("inflowValvePin",po::value<int>(),"WiringPi pin connected to inflow valve")
			("inflowValveActv",po::value<int>()->default_value(1),"Set 1 if activated is HIGH, 0 if activated is LOW")
			("outflowValvePin",po::value<int>(),"WiringPi pin connected to outflow valve")
			("outflowValveActv",po::value<int>()->default_value(1),"Set 1 if activated is HIGH, 0 if activated is LOW")
			;
	ifstream ifs("config.cfg");
	if(!ifs)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::error) << "Configuration file not loaded. Exiting";
		exit(1);
	}

	po::store(po::parse_config_file(ifs,desc),this->vm);
	po::notify(this->vm);

	//Pin check- checking that no device is set to the same pin
	if(this->vm["tankFullPin"].as<int>()==this->vm["tankEmptyPin"].as<int>() ||this->vm["tankFullPin"].as<int>()==this->vm["outflowValvePin"].as<int>() ||this->vm["tankFullPin"].as<int>()==this->vm["inflowValvePin"].as<int>()\
	  ||this->vm["tankEmptyPin"].as<int>()==this->vm["outflowValvePin"].as<int>() ||this->vm["tankEmptyPin"].as<int>()==this->vm["inflowValvePin"].as<int>()\
	  ||this->vm["outflowValvePin"].as<int>()==this->vm["inflowValvePin"].as<int>())
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::error) << "WiringPi pin assignment conflict";
		exit(1);
	}

	return true;
}

}


