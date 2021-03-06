/*
 * Rig.h
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */

#ifndef RIG_H_
#define RIG_H_

#include <signal.h>
#include <boost/log/trivial.hpp>
/*#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>*/
#include <boost/program_options/variables_map.hpp>

//#include "Main.h"
#include "LogicSensor.h"
#include "Pump.h"
#include "LogicOut.h"
#include "AnalogIn.h"
#include "FlowMeter.h"

namespace src = boost::log::sources;
namespace po = boost::program_options;

class Rig {
public:
	Rig(int _fullSpeed) ;
	Rig(po::variables_map &vm);
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
	bool openReleaseValveOnly();
	bool closeInflowValveOnly();
	bool closeOutflowValveOnly();
	bool closeReleaseValveOnly();
	bool setPumpSpeed(double percentage); //Set pump speed as percentage of fullspeed
	
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
	int getPumpSpeed();
	bool getPumpRunning();
	bool getPumpErrStatus();
	int getFlowCounter();		//Returns the pulse counter that gets reset by ResetFlowMeasuring
	double getLastFlow(int n);	//Returns the flow average of last n pulses in liters/minute
	bool getEmerBtn();	//True if emergency.

	bool shutdown();  //STOP ALL
	
private:
	
	int fullSpeed = 0;	//pump fullspeed in rpm
	src::severity_logger_mt<>& lg;

	//Hardware
	LogicSensor tankFullSensor;
	LogicSensor tankEmptySensor;
	Pump		pump;
	LogicOut	outflowValve;
	LogicOut	releaseValve;
	LogicOut	inflowValve;
	AnalogIn	analogIn;
	const int	pressureCh;	//Channel of the ADC corresponding to Pressure
	FlowMeter	flow1;
	LogicSensor emerBtn;	//Emergency button

};



#endif /* RIG_H_ */
