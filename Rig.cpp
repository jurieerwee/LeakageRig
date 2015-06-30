/*
 * Rig.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */

#include "Rig.h"

Rig::Rig() {
	// TODO Auto-generated constructor stub


}

Rig::~Rig() {
	// TODO Auto-generated destructor stub
}

bool Rig::shutdown()	//TODO: Check that procedure is correct
{
	int i = 5;
	while(!this->stopPumpOnly() && i-- >0);

	if(0==i)
		return false;

	bool reply = false;
	reply = this->closeOutflowValveOnly();
	reply &= this->closeInflowValveOnly();

	return reply;
}

bool Rig::startPump()
{
	//Close inflow valve.  Assuming no loop.  If there is a loop, change this
	if(!this->closeInflowValveOnly())
		return false;

	//Open outflow valve
	if(!this->openInflowValveOnly())
		return false;

	//Start pump
	if(!this->startPumpOnly())
		return false;

	return true;
}

bool Rig::startPumpOnly()
{
	//TODO: start pump
	return true;
}

bool Rig::stopPump()
{
	if(!this->stopPumpOnly())
		return false;

	if(!this->closeOutflowValveOnly())
		return false;

	return true;
}


bool Rig::stopPumpOnly()
{
	//TODO: Stop pump
	return true;
}

bool Rig::startTankFill()
{
	if(!this->stopPumpOnly())
		return false;

	if(!this->closeOutflowValveOnly())
		return false;

	if(!this->openInflowValveOnly())
		return false;

	return true;
}


bool Rig::stopTankFill()
{
	if(!this->closeOutflowValveOnly())
		return false;

	return true;
}


bool Rig::openInflowValveOnly()
{
	//TODO: Open inflow valve
	return true;
}

bool Rig::openOutflowValveOnly()
{
	//TODO: open outflow valve
	return true;
}

bool Rig::closeInflowValveOnly()
{
	//TODO: close inflow valve
	return true;
}

bool Rig::closeOutflowValveOnly()
{
	//TODO: close outflow valve
	return true;
}

bool Rig::setPumpSpeed(double percentage) //Set pump speed as percentage of fullspeed
{
	//TODO: set pump speed as percentage of full
	return true;
}

bool Rig::setPumpPressure(double percentage)	//Set pump speed to deliver percentage of fullspeed pressure
{
	//TODO: set pump speed according to full pressure percentage
	return true;
}


bool Rig::getSensor_FullTank()	//True if full, false if not full
{
	//Get sensor status from its object

}

bool Rig::getSensor_EmptyTank() //True is empty, false if not empty
{
	//Get sensor status from its object
}

bool Rig::getSensor_FlowDirection() //True if forward(out) flow, false if reverse flow
{
	//Get sensor status from its object
}

bool Rig::resetFlowMeasuring()
{

	//Get sensor status from its object
	return true;
}

double Rig::getFlowMeasure()	//Returns the flow meter reading in liters
{
	//Get sensor status from its object
}
double Rig::getSensor_Pressure() //Returns pressure transducer reading in standard measure.  TODO: Units to be confirmed
{
	//Get sensor status from its object
}
