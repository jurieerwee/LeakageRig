/*
 * Rig.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */
#include <boost/log/trivial.hpp>
/*#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file/hpp>
#include <boost/log/utility/setup/common_attributes.hpp>*/

#include "Rig.h"
//#include "Main.h"

//namespace logging = boost::log;
//namespace src = boost::log::sources;
using namespace std;

Rig::Rig(int _fullSpeed) {
	// TODO Auto-generated constructor stub
	this->fullSpeed = _fullSpeed;

	//this->lg = my_logger::get();

}

Rig::Rig()
{
	this->fullSpeed = 0;
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
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Pump start initiated";
	//Close inflow valve.  Assuming no loop.  If there is a loop, change this
	if(!this->closeInflowValveOnly())
		return false;

	//Open outflow valve
	if(!this->openInflowValveOnly())
		return false;

	//Start pump
	if(!this->startPumpOnly())
		return false;

	//BOOST_LOG_SEV(this->lg,NORMAL) << "Pump started successfully";
	return true;
}

bool Rig::startPumpOnly()
{
	//TODO: start pump
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Pump Only started successfully";
	return true;
}

bool Rig::stopPump()
{
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Pump stop initiated";
	if(!this->stopPumpOnly())
		return false;

	if(!this->closeOutflowValveOnly())
		return false;

	//BOOST_LOG_SEV(this->lg,NORMAL) << "Pump stopped successfully";
	return true;
}


bool Rig::stopPumpOnly()
{
	//TODO: Stop pump
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Pump Only stop successfully";
	return true;
}

bool Rig::startTankFill()
{
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Start filling tank initiated";
	if(!this->stopPumpOnly())
		return false;

	if(!this->closeOutflowValveOnly())
		return false;

	if(!this->openInflowValveOnly())
		return false;

	//BOOST_LOG_SEV(this->lg,NORMAL) << "Start filling tank successful";
	return true;
}


bool Rig::stopTankFill()
{
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Stop filling tank initiated";
	if(!this->closeOutflowValveOnly())
		return false;

	//BOOST_LOG_SEV(this->lg,NORMAL) << "Stop filling tank successful";
	return true;
}


bool Rig::openInflowValveOnly()
{
	//TODO: Open inflow valve
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Open inflow valve ONLY successful ";
	return true;
}

bool Rig::openOutflowValveOnly()
{
	//TODO: open outflow valve
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Open outflow valve ONLY successful ";
	return true;
}

bool Rig::closeInflowValveOnly()
{
	//TODO: close inflow valve
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Close inflow valve ONLY successful ";
	return true;
}

bool Rig::closeOutflowValveOnly()
{
	//TODO: close outflow valve
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Close outflow valve ONLY successful ";
	return true;
}

bool Rig::setPumpSpeed(double percentage) //Set pump speed as percentage of fullspeed
{
	//TODO: set pump speed as percentage of full
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Set pump speed to "<< precentage <<;" successful";
	return true;
}

bool Rig::setPumpPressure(double percentage)	//Set pump speed to deliver percentage of fullspeed pressure
{
	//TODO: set pump speed according to full pressure percentage
	//BOOST_LOG_SEV(this->lg,NORMAL) << "Set pump pressure to "<< precentage <<;" successful";
	return true;
}


bool Rig::getSensor_FullTank()	//True if full, false if not full
{
	//Get sensor status from its object
	return false;

}

bool Rig::getSensor_EmptyTank() //True is empty, false if not empty
{
	//Get sensor status from its object
	return false;
}

bool Rig::getSensor_FlowDirection() //True if forward(out) flow, false if reverse flow
{
	//Get sensor status from its object
	return true;
}

bool Rig::resetFlowMeasuring()
{

	//Get sensor status from its object
	return true;
}

double Rig::getFlowMeasure()	//Returns the flow meter reading in liters
{
	//Get sensor status from its object
	return 0;
}
double Rig::getSensor_Pressure() //Returns pressure transducer reading in standard measure.  TODO: Units to be confirmed
{
	//Get sensor status from its object
	return 0;
}
//Sets new full speed rpm and returns old full speed
int Rig::setFullSpeed(int rpm)
{
	int old = this->fullSpeed;
	
	this->fullSpeed = rpm;
	return old;
}

double Rig::getFullPressure()
{
	//TODO:  Return fullPressure as a function of fullSpeed
	return 0.0;
}

//Force sensor states to be updated by probe.
bool Rig::forceSensorUpdate()
{
	//TODO Update all sensor states that otherwise gets updated by interupt.

	return true;
}
