/*
 * Rig.h
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */

#ifndef RIG_H_
#define RIG_H_

#include <boost/log/trivial.hpp>
/*#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>*/

//#include "Main.h"
#include "LogicSensor.h"
#include "Pump.h"
#include "LogicOut.h"

namespace src = boost::log::sources;

class Rig {
public:
	Rig(int _fullSpeed) ;
	Rig();
	virtual ~Rig();
	
	//Operate rig
	bool startPump();
	bool startPumpOnly();
	bool stopPump();
	bool stopPumpOnly();
	bool startTankFill();
	bool stopTankFill();
	bool openInflowValveOnly();
	bool openOutflowValveOnly();
	bool closeInflowValveOnly();
	bool closeOutflowValveOnly();
	bool setPumpSpeed(double percentage); //Set pump speed as percentage of fullspeed
	bool setPumpPressure(double percentage);	//Set pump speed to deliver percentage of fullspeed pressure
	
	//Set parameters
	int setFullSpeed(int rpm);
	//Get paramenters
	double getFullPressure();
	
	//Get and set data and sensor info
	bool getSensor_FullTank();	//True if full, false if not full
	bool getSensor_EmptyTank(); //True is empty, false if not empty
	bool getSensor_FlowDirection(); //True if forward(out) flow, false if reverse flow
	bool resetFlowMeasuring();
	double getFlowMeasure();	//Returns the flow meter reading in liters
	double getSensor_Pressure(); //Returns pressure transducer reading in standard measure.  TODO: Units to be confirmed
	bool forceSensorUpdate();

	bool shutdown();  //STOP ALL
	
private:
	
	int fullSpeed = 0;	//pump fullspeed in rpm
	src::severity_logger_mt<>& lg;

	//Hardware
	LogicSensor tankFullSensor;
	LogicSensor tankEmptySensor;
	Pump		pump;
	LogicOut	outflowValve;
	LogicOut	inflowValve;

};

#endif /* RIG_H_ */
