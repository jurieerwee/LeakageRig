/*
 * Rig.h
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */

#ifndef RIG_H_
#define RIG_H_



class Rig {
public:
	Rig();
	virtual ~Rig();

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


	bool getSensor_FullTank();	//True if full, false if not full
	bool getSensor_EmptyTank(); //True is empty, false if not empty
	bool getSensor_FlowDirection(); //True if forward(out) flow, false if reverse flow
	bool resetFlowMeasuring();
	double getFlowMeasure();	//Returns the flow meter reading in liters
	double getSensor_Pressure(); //Returns pressure transducer reading in standard measure.  TODO: Units to be confirmed






};

#endif /* RIG_H_ */
