/*
 * Rig.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/program_options/variables_map.hpp>
#include <wiringPi.h>

#include "Rig.h"
#include "Pump.h"
#include "Main.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace po = boost::program_options;

using namespace std;

/*Rig::Rig(int _fullSpeed) : tankFullSensor(22,true,true,true), tankEmptySensor(21,true,true,true), pump(4000), lg(my_logger::get()),\
		inflowValve(23,false), outflowValve(24,false)
{
	// TODO Auto-generated constructor stub
	this->fullSpeed = _fullSpeed;

	//this->lg = my_logger::get();

}*/

Rig::Rig(po::variables_map &vm) : tankFullSensor(vm["tankFullPin"].as<int>(),true,true,vm["tankFullNO"].as<int>()), tankEmptySensor(vm["tankEmptyPin"].as<int>(),true,true,vm["tankEmptyNO"].as<int>()), pump(vm["pumpFullSpeed"].as<int>(),vm["dacID"].as<int>(),vm["startPin"].as<int>(),vm["runningPin"].as<int>(),vm["errStatusPin"].as<int>()), lg(my_logger::get()),\
		inflowValve(vm["inflowValvePin"].as<int>(),false), outflowValve(vm["outflowValvePin"].as<int>(),false), analogIn(vm["adcID"].as<int>()), pressureCh(vm["pressureCh"].as<int>()),\
		flow1(vm["flow1Pin"].as<int>(),vm["flow1dirPin"].as<int>(), vm["flow1Pull"].as<bool>(), vm["flow1PullUp"].as<bool>(),vm["flow1Factor"].as<double>(),vm["flow1RunLength"].as<int>()),\
		releaseValve(vm["releaseValvePin"].as<int>(),false), emerBtn(vm["emergencyBtnPin"].as<int>(), emerBtn(vm["emergencyBtnPull"].as<bool>(), emerBtn(vm["emergencyBtnPullUp"].as<bool>(), emerBtn(vm["emergencyBtnNO"].as<bool>())
{
	this->fullSpeed = vm["pumpFullSpeed"].as<int>();
	analogIn.setScale(this->pressureCh, vm["pressureOffset"].as<double>(), vm["pressureScale"].as<double>());
	wiringPiSetup();	//Call it here, so that it is only called once

}
/*
Rig::Rig(): lg(my_logger::get())
{
	this->fullSpeed = 0;
}*/

Rig::~Rig() {
	// TODO Auto-generated destructor stub
}

bool Rig::shutdown()	//TODO: Check that procedure is correct
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Rig shutdown initiated";
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
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pump start initiated";
	//Close inflow valve.  Assuming no loop.  If there is a loop, change this
	if(!this->closeInflowValveOnly())
		return false;

	//Open outflow valve
	if(!this->openOutflowValveOnly())
		return false;

	//Start pump
	if(!this->startPumpOnly())
		return false;

	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pump started successfully";
	return true;
}

bool Rig::startPumpOnly()
{
	if(this->pump.setPumpOn(true))
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pump Only started successfully";
		return true;
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::warning) << "Pump Only start unsuccessfully";
		return false;
	}
}

bool Rig::stopPump()
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pump stop initiated";
	if(!this->stopPumpOnly())
		return false;

	if(!this->closeOutflowValveOnly())
		return false;

	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pump stopped successfully";
	return true;
}


bool Rig::stopPumpOnly()
{
	if(this->pump.setPumpOn(false))
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Pump Only stop successfully";
		return true;
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::warning) << "Pump Only stop unsuccessfully";
		return false;
	}
}

bool Rig::startTankFill()
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Start filling tank initiated.  Opening all valves";
	if(!this->stopPumpOnly())
		return false;
	//Open all valves allow for pump priming and removal of air
	if(!this->openInflowValveOnly())
		return false;

	sleep(1);	//Sleep to limit simultanious ruch current

	if(!this->openOutflowValveOnly())
			return false;

	sleep(1); //Sleep to limit simultanious ruch current

	if(!this->openReleaseValveOnly())
			return false;

	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Start filling tank successful";
	return true;
}


bool Rig::stopTankFill()
{
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Stop filling tank initiated";
	bool msg = true;
	if(!this->closeInflowValveOnly())
		msg =  false;

	if(!this->closeInflowValveOnly())
		msg = false;

	if(!this->closeInflowValveOnly())
		msg =  false;

	if(msg)
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Stop filling tank successful";

	return msg;
}


bool Rig::openInflowValveOnly()
{
	bool reply = this->inflowValve.setActive(true);
	if (reply)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Open inflow valve ONLY successful ";
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Open inflow valve ONLY unsuccessful ";
		return false;
	}

	return true;
}

bool Rig::openOutflowValveOnly()
{
	bool reply = this->outflowValve.setActive(true);
	if (reply)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Open outflow valve ONLY successful ";
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Open outflow valve ONLY unsuccessful ";
		return false;
	}
	return true;
}

bool Rig::openReleaseValveOnly()
{
	bool reply = this->releaseValve.setActive(true);
	if (reply)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Open release valve ONLY successful ";
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Open release valve ONLY unsuccessful ";
		return false;
	}
	return true;
}

bool Rig::closeInflowValveOnly()
{
	bool reply = this->inflowValve.setActive(false);
	if (reply)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Close inflow valve ONLY successful ";
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Close inflow valve ONLY unsuccessful ";
		return false;
	}
	return true;
}

bool Rig::closeOutflowValveOnly()
{
	bool reply = this->outflowValve.setActive(false);
	if (reply)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Close outflow valve ONLY successful ";
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Close outflow valve ONLY unsuccessful ";
		return false;
	}
	return true;
}

bool Rig::closeReleaseValveOnly()
{
	bool reply = this->releaseValve.setActive(false);
	if (reply)
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Close release valve ONLY successful ";
	}
	else
	{
		BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Close release valve ONLY unsuccessful ";
		return false;
	}
	return true;
}

bool Rig::setPumpSpeed(double percentage) //Set pump speed as percentage of fullspeed
{
	this->pump.setSpeedPrec(percentage);
	BOOST_LOG_SEV(this->lg,logging::trivial::info) << "Set pump speed to "<< percentage << " successful";
	return true;
}


int Rig::getPumpSpeed()
{
	return this->pump.getSpeed();
}

bool Rig::getPumpRunning()
{
	return this->pump.getPumpRunning();
}

bool Rig::getPumpErrStatus()
{
	return this->pump.getPumpErrStatus();
}

bool Rig::getSensor_FullTank()	//True if full, false if not full
{
	return this->tankFullSensor.getState();
}

bool Rig::getSensor_EmptyTank() //True is empty, false if not empty
{
	return !this->tankEmptySensor.getState();
}

bool Rig::getSensor_FlowDirection() //True if forward(out) flow, false if reverse flow
{
	return this->flow1.getDir();
}

bool Rig::resetFlowMeasuring()
{
	//TODO Refine
	this->flow1.clearCounter();
	return true;
}

double Rig::getFlowMeasure()	//Returns the flow meter reading in liters/minute
{
	return this->flow1.getRunningAve();	//Note, returns running average.
}

int Rig::getFlowCounter()		//Returns the pulse counter that gets reset by ResetFlowMeasuring
{
	return this->flow1.getCounter();
}

double Rig::getLastFlow(int n)	//Returns the flow average of last n pulses in liters/minute
{
	return this->flow1.getLastAve(n);
}


double Rig::getSensor_Pressure() //Returns pressure transducer reading in standard measure.  TODO: Units to be confirmed
{
	//NBNB: Not that with current setup, this instruction will return the previous conversion and triggers the next.
	return this->analogIn.readChannel(this->pressureCh);

}

bool Rig::getEmerBtn()	//Returns emergency button's state.  Always does an update.
{
	this->emerBtn.update();
	return this->emerBtn.getState();
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
	this->tankEmptySensor.update();
	this->tankFullSensor.update();
	this->pump.statusUpdate();

	return true;
}
