/*
 * LeakageTest.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <vector>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
/*#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file/hpp>
#include <boost/log/utility/setup/common_attributes.hpp>*/
#include <boost/log/trivial.hpp>
#include <boost/program_options/variables_map.hpp>

#include "TestData.h"
#include "Rig.h"
#include "LeakageTest.h"

#include "Main.h"

//namespace logging = boost::log;
//namespace src = boost::sources;
namespace po = boost::program_options;
using namespace std;

namespace LeakageTest
{
//Alarms
volatile bool alarmTrigger;
int continualAlarmInterval;

void  ALRMhandlerOnce(int in)
{
	signal(SIGALRM,SIG_IGN);
	alarmTrigger = true;
}

void  ALRMhandlerCont(int in)
{
	signal(SIGALRM,SIG_IGN);
	alarmTrigger = true;
	signal(SIGALRM,ALRMhandlerCont);
	alarm(continualAlarmInterval);

}


LeakageTest::LeakageTest(Rig *_rig, TestData *_dataset, int _settleTime, int _pressureMeasureInterval, int _pressureTotalCount, const double _testPressures[], int _testPressuresCount):\
		settleTime(_settleTime), pressureMeasureInterval(_pressureMeasureInterval), pressureTotalCount(_pressureTotalCount), testPressuresCount(_testPressuresCount), lg(my_logger::get())
{

	this->rig 		= _rig;		//Note, _rig and _dataset are references, therefore we are taking the addresses of the passed objects. This is easier since we dont have to remember to pass the address everytime.
	this->dataset = _dataset;
	state = INITIAL;
	this->measureCounter =0;
	this->pressureCounter = 0;
	this->alarmActive = false;
	this->testPressures = _testPressures;
//	this->lg = my_logger::get();
}

LeakageTest::~LeakageTest() {
	// TODO Auto-generated destructor stub
}

int LeakageTest::call(void)
{
	int reply = 2;

	switch(this->state)
	{
	case INITIAL:
		reply = this->initial();
		if(1==reply)
		{
			changeState(SET_SPEED);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case SET_SPEED:
		reply = this->setSpeed();
		if(1==reply)
		{
			changeState( SETTLE);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case SETTLE:
		reply = this->settle();
		if(1==reply)
		{
			changeState(MEASURE);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case MEASURE:
		reply = this->measure();
		if(1==reply)
		{
			changeState( FINAL);
		}
		else if(-1==reply)
		{
			changeState( SET_SPEED);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case FINAL:
		reply = this->final();
		if(1==reply)
		{
			return 1;
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	default:
		return 2;
		break;

	}

	return 0;
}

int LeakageTest::initial(void)
{
	bool reply = false;
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Leakage test initialization started";
	this->measureCounter = 0;

	//TODO: Set pump fullspeed

	reply = this->rig->startPump();

	return((reply==true) ?  1 : 2);
}
int LeakageTest::setSpeed(void)
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Setting pump speed for test point " << this->measureCounter << " of " << this->testPressuresCount << " at pressure of " << this->testPressures[this->measureCounter] ;
	double percentage = this->testPressures[this->measureCounter]/this->rig->getFullPressure();

	bool reply = this->rig->setPumpSpeed(percentage);

	return (reply==true ?1:2);
}
int LeakageTest::settle(void)
{
	if(this->alarmActive)
	{
		if(alarmTrigger)
		{
			alarmTrigger = false;
			alarmActive = false;
			return 1;
		}
	}
	else
	{
		//Set timer
		this->alarmActive = true;
		alarmTrigger = false;
		signal(SIGALRM,ALRMhandlerOnce);
		alarm(this->settleTime);
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Settling period of " << this->settleTime << "seconds started";
	}

	return 0;
}
int LeakageTest::measure(void)
{
	if(this->alarmActive)
	{
		if(alarmTrigger)
		{
			alarmTrigger = false;

			this->pressureMeasurements.push_back(this->rig->getSensor_Pressure());
			BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pressure measurement "<< this->pressureCounter << "of " << this->pressureTotalCount << " taken";

			this->pressureCounter ++;
			if(this->pressureCounter >= this->pressureTotalCount)
			{
				alarmActive = false;
				alarm(0);	//cancel alarm

				this->flowRate = this->rig->getFlowMeasure() / (this->pressureMeasureInterval * this->pressureTotalCount) * 60;

				//Code from http://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos (1/7/2015)
				boost::accumulators::accumulator_set<double,boost::accumulators::stats<boost::accumulators::tag::variance>> acc;
				for_each(this->pressureMeasurements.begin(),this->pressureMeasurements.end(),acc);

				dataset->setTestPressure(this->measureCounter,boost::accumulators::mean(acc),sqrt(boost::accumulators::variance(acc)));
				dataset->setTestFlow(this->measureCounter, this->flowRate);

				this->measureCounter++;

				//TODO:  If flow is too low, there is no need to test further.  Need to build that in

				return (this->measureCounter < this->testPressuresCount ? -1 : 1);
			}
		}
	}
	else
	{
		//Set timer
		this->alarmActive = true;
		alarmTrigger = false;
		signal(SIGALRM,ALRMhandlerCont);
		continualAlarmInterval = this->pressureMeasureInterval;
		alarm(this->pressureMeasureInterval);

		//Set meters
		this->pressureCounter =0;

		this->pressureMeasurements.clear();
		this->rig->resetFlowMeasuring();
	}

	return 0;
}
int LeakageTest::final(void)
{
	//Stop pump
	bool reply = this->rig->stopPump();



	return (true==reply?1:2);

}

bool LeakageTest::changeState(ltState newState)
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Changing leakage state from \"" << this->state << "\" to \"" << newState <<"\"";

	this->state = newState;

	return true;
}
}
