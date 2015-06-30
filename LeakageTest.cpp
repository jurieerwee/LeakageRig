/*
 * LeakageTest.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */
#include <unistd.h>
#include <signal.h>

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


LeakageTest::LeakageTest(Rig _rig, int _settleTime, int _pressureMeasureInterval, int _pressureTotalCount):\
		rig(_rig), settleTime(_settleTime), pressureMeasureInterval(_pressureMeasureInterval), pressureTotalCount(_pressureTotalCount)
{
	// TODO Auto-generated constructor stub
	state = INITIAL;
	this->measureCounter =0;
	this->pressureCounter = 0;
	this->timerActive = false;
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

	reply = rig.startPump();

	return((reply==true) ?  1 : 2);
}
int LeakageTest::setSpeed(void)
{
	double percentage = this->testPressures[this->measureCounter++]/this->fullPressure;

	bool reply = rig.setPumpSpeed(percentage);

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

			this->testPressures[this->pressureCounter] = rig.getSensor_Pressure();

			if(++this->pressureCounter >= this->pressureTotalCount)
			{
				alarmActive = false;
				alarm(0);	//cancel alarm

				this->flowRate = rig.getFlowMeasure() / (this->pressureMeasureInterval * this->pressureTotalCount) * 60;

				return 1;
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
		rig.resetFlowMeasuring();
	}

	return 0;
}
int LeakageTest::final(void)
{
	//Stop pump
	bool reply = rig.stopPump();


	//TODO: Save data in correct places

	//this->testPressures[]
	//this->flowRate

	return (true==reply?1:2);

}
