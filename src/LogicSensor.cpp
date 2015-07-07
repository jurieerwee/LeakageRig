/*
 * LogicSensor.cpp
 *
 *  Created on: 07 Jul 2015
 *      Author: Jurie
 */

#include "LogicSensor.h"
#include <wiringPi.h>
#include <stdlib.h>

using namespace std;

LogicSensor::LogicSensor(int _pin, bool _pull, bool _pullUp): pin(_pin), pull(_pull), pullUp(_pullUp){
	// TODO Auto-generated constructor stub
	pinMode(this->pin, INPUT);
	pullUpDnControl(this->pin, this->pull ?(this->pullUp?PUD_UP:PUD_DOWN):PUD_OFF);
	this->update();
}



LogicSensor::~LogicSensor() {
	// TODO Auto-generated destructor stub
}

bool LogicSensor::update()
{
	this->isHigh =  (bool)digitalRead(this->pin) ;
	return true;
}

bool LogicSensor::getState()
{
	return this->isHigh;
}

