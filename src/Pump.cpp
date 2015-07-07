/*
 * Pump.cpp
 *
 *  Created on: 07 Jul 2015
 *      Author: Jurie
 */

#include "Pump.h"

Pump::Pump(int _fullSpeed): pumpFullSpeed(_fullSpeed),pumpSpeed(_fullSpeed){
	// TODO Auto-generated constructor stub

}

Pump::~Pump() {
	// TODO Auto-generated destructor stub
}

bool Pump::setSpeed(int speed)
{
	if(speed>this->pumpFullSpeed)
		return false;

	this->pumpSpeed = speed;

	//TODO:  Send new speed to pump
	return true;
}
int Pump::getSpeed()
{

	return this->pumpSpeed;
}
int Pump::getFullSpeed()
{

	return this->pumpFullSpeed;
}
bool Pump::setPumpOn(bool set)
{
	this->pumpOn = set;

	//TODO:  Turn pump on/off

	return true;
}

bool Pump::getPumpOn()
{
	return this->pumpOn;

}
