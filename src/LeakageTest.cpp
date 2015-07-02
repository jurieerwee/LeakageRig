/*
 * LeakageTest.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <boost/accumulators/accumulators.hpp>
	//using boost::accumulators;
#include <boost/accumulators/statistics.hpp>
	//using boost::stats;

#include "TestData.h"
#include "LeakageTest.h"
#include "Rig.h"


using namespace std;

//Alarms
volatile bool alarmTrigger;

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
}


LeakageTest::LeakageTest(Rig _rig, TestData _dataset, int _settleTime, int _pressureMeasureInterval, int _pressureTotalCount, double _testPressures[], int _testPressuresCount):\
		settleTime(_settleTime), pressureMeasureInterval(_pressureMeasureInterval), pressureTotalCount(_pressureTotalCount), testPressuresCount(_testPressuresCount)
{

	this->rig 		= _rig;
	this->dataset = _dataset; 
	state = INITIAL;
	this->measureCounter =0;
	this->pressureCounter = 0;
	this->alarmActive = false;
	this->testPressures = _testPressures;
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
			this->state = SET_SPEED;
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
			this->state = SETTLE;
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
			this->state = MEASURE;
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
			this->state = FINAL;
		}
		else if(-1==reply)
		{
			this->state = SET_SPEED;
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

	//TODO: Set pump fullspeed

	reply = this->rig.startPump();

	return((reply==true) ?  1 : 2);
}
int LeakageTest::setSpeed(void)
{
	double percentage = this->testPressures[this->measureCounter++]/this->rig.getFullPressure();

	bool reply = this->rig.setPumpSpeed(percentage);

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

			this->pressureMeasurements.push_back(this->rig.getSensor_Pressure());

			if(++this->pressureCounter >= this->pressureTotalCount)
			{
				alarmActive = false;
				alarm(0);	//cancel alarm

				this->flowRate = this->rig.getFlowMeasure() / (this->pressureMeasureInterval * this->pressureTotalCount) * 60;

				//Code from http://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos (1/7/2015)
				boost::accumulators::accumulator_set<double,boost::accumulators::stats<boost::accumulators::tag::variance>> acc;
				for_each(this->pressureMeasurements.begin(),this->pressureMeasurements.end(),acc);

				dataset.setTestPressure(this->measureCounter,boost::accumulators::mean(acc),sqrt(boost::accumulators::variance(acc)));
				dataset.setTestFlow(this->measureCounter, this->flowRate);

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
		alarm(this->pressureMeasureInterval);

		//Set meters
		this->pressureCounter =0;
		this->pressureMeasurements.clear();
		this->rig.resetFlowMeasuring();
	}

	return 0;
}
int LeakageTest::final(void)
{
	//Stop pump
	bool reply = this->rig.stopPump();



	return (true==reply?1:2);

}
