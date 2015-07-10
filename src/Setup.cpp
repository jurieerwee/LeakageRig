/*
 * Setup.cpp
 *
 *  Created on: 09 Jul 2015
 *      Author: Jurie
 */



#include <iostream>
#include <map>
#include <string>
#include <signal.h>
#include <vector>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/program_options/variables_map.hpp>

#include "Setup.h"
#include "Rig.h"
#include "TestData.h"
#include "Main.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace po = boost::program_options;

using namespace std;

//Alarms
volatile bool alarmTrigger;
int continualAlarmInterval =0;

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

Setup::Setup(Rig* _rig, TestData *_dataset, int _pressureMeasureInterval, int _pressureTotalCount) : rig(_rig) , dataset(_dataset), lg(my_logger::get())\
		, pressureMeasureInterval(_pressureMeasureInterval), pressureTotalCount(_pressureTotalCount)
{
	// TODO Auto-generated constructor stub

}

Setup::~Setup() {
	// TODO Auto-generated destructor stub
}

int Setup::call()
{
	int reply =2;

	switch(this->state)
	{
	case START_TANK_FILL:
		reply = this->startTankFill();
		if(1==reply)
		{
			this->changeState(WAIT_TANK_FILL);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case WAIT_TANK_FILL:
		reply = this->waitTankFill();
		if(1==reply)
		{
			this->changeState(STOP_TANK_FILL);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case STOP_TANK_FILL:
		reply = this->stopTankFill();
		if(1==reply)
		{
			this->changeState(PRE_PRESSURE_TEST);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case PRE_PRESSURE_TEST:
		reply = this->prePressureTest();
		if(1==reply)
		{
			this->changeState(PRESSURE_TEST);
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case PRESSURE_TEST:
		reply = this->pressureTest();
		if(1==reply)
		{
			this->changeState(POST_PRESSURE_TEST);
			return 1;
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	case POST_PRESSURE_TEST:
		reply = this->postPressureTest();
		if(1==reply)
		{
			this->changeState(START_TANK_FILL);
			return 1;
		}
		else if(2==reply)
		{
			return 2;
		}
		break;
	default:
		BOOST_LOG_SEV(this->lg,logging::trivial::error) << "Setup in unknown state!";
		return 2;

	}

	return 0;
}

bool Setup::changeState(stpState newState)
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Changing setup state from \"" << this->state << "\" to \"" << newState <<"\"";

	this->state = newState;
	return true;
}

int Setup::startTankFill()
{
	return this->rig->startTankFill()?1:2;
}

int Setup::waitTankFill()
{
	return this->rig->getSensor_FullTank()?1:0;
}

int Setup::stopTankFill()
{
	return this->rig->stopTankFill()?1:2;
}

int Setup::prePressureTest()
{
	this->pressureMeasurements.clear();
	this->alarmActive = true;
	alarmTrigger = false;
	continualAlarmInterval = this->pressureMeasureInterval;
	this->measurementCounter = 0;
	signal(SIGALRM,ALRMhandlerCont);
	alarm(this->pressureMeasureInterval);

	return 1;
}

int Setup::pressureTest()
{
	if(alarmTrigger)
	{
		alarmTrigger = false;
		//Measure
		this->pressureMeasurements.push_back(this->rig->getSensor_Pressure());
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pressure measurement "<< this->measurementCounter << "of " << this->pressureTotalCount << " taken";

		this->measurementCounter ++;
		if(this->measurementCounter>=this->pressureTotalCount)
		{
			return 1;
		}
	}

		return 0;
}

int Setup::postPressureTest()
{
	//Stop alarm
	this->alarmActive = false;
	alarm(0);

	//Save
	boost::accumulators::accumulator_set<double,boost::accumulators::stats<boost::accumulators::tag::variance>> acc;
	for_each(this->pressureMeasurements.begin(),this->pressureMeasurements.end(),acc);

	dataset->setSystemPressure(boost::accumulators::mean(acc),sqrt(boost::accumulators::variance(acc)));

	return 1;
}
