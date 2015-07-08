/*
 * LogicOut.cpp
 *
 *  Created on: 07 Jul 2015
 *      Author: Jurie
 */

#include <wiringPi.h>

#include "LogicOut.h"


LogicOut::LogicOut(int _pin, bool _activeHigh):	pin(_pin), activeHigh(_activeHigh)
{
	pinMode(this->pin, INPUT);
	digitalWrite(this->pin,this->activeHigh==this->activeState);
}

LogicOut::~LogicOut() {
	// TODO Auto-generated destructor stub
}

bool LogicOut::setActive(bool act)
{
	digitalWrite(this->pin,this->activeHigh==act);

	this->activeState = act;

	return true;
}
bool LogicOut::getState()
{
	return this->activeState;
}

