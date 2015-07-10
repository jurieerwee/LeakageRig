/*
 * Isolate.cpp
 *
 *  Created on: 10 Jul 2015
 *      Author: Jurie
 */

#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <boost/log/trivial.hpp>


#include "Isolate.h"
#include "Alarms.h"
#include "Rig.h"
#include "Main.h"

namespace logging = boost::log;
using namespace std;



Isolate::Isolate(Rig *_rig, int _testDuration, int _flowTolerance): rig(_rig), testDuration(_testDuration), flowTolerance(_flowTolerance), lg(my_logger::get())
{
	// TODO Auto-generated constructor stub

}

Isolate::~Isolate() {
	// TODO Auto-generated destructor stub
}

int Isolate::call()
{
	int reply =2;
	switch(this->state)
	{
	case PRE_TEST:
		reply = this->preTest();
		if(1==reply)
		{
			this->changeState(TEST);
		}
		else
		{
			return 2;
		}
		break;
	case TEST:
		reply = this->test();
		if(1==reply)
		{
			this->changeState(POST_TEST);
		}
		else if( 2==reply)
		{
			return 2;
		}
		break;
	case POST_TEST:
		reply = this->postTest();
		this->changeState(PRE_TEST);
		return reply;
		break;
	default:
		return 2;

	}

	return 0;
}

int Isolate::preTest()
{
	//Initialize
	this->flowVolume = 0;

	//Start
	this->rig->startTankFill();	//This works, since the method doest check if the tank is full.  Might need to change this to openInflowValveOnly()
	this->rig->resetFlowMeasuring();

	alarms::alarmActive = true;
	alarms::alarmTrigger = false;
	signal(SIGALRM,alarms::ALRMhandlerOnce);
	alarm(this->testDuration);

	return 1;

}

int Isolate::test()
{
	if(alarms::alarmTrigger)
	{
		alarms::alarmActive = false;
		alarms::alarmTrigger = false;

		this->flowVolume = this->rig->getFlowMeasure();
		//TODO:  Check how to use the flow direction indicator
		return 1;
	}

	return 0;
}

int Isolate::postTest()
{
	this->rig->stopTankFill();
	//TODO:  Check how to use the flow direction indicator

	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Flow volume measured was " << this->flowVolume << ". Tolerance: " << this->flowTolerance;
	if(this->flowVolume < this->flowTolerance)
	{
		return 1;
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::warning) << "Pipe not isolated. First isolate pipe before continuing";
		return -1;
	}


}

bool Isolate::changeState(isoState newState)
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Changing isolate state from \"" << this->state << "\" to \"" << newState <<"\"";

	this->state = newState;

	return true;
}
