/*
 * LeakageTest.h
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */

#ifndef LEAKAGETEST_H_
#define LEAKAGETEST_H_

#include "Rig.h"

class LeakageTest {
public:
	LeakageTest(Rig _rig, int _settleTime, int _pressureMeasureInterval, int _pressureTotalCount);
	virtual ~LeakageTest();

	int call();

private:
	enum State
	{
		INITIAL,
		SET_SPEED,
		SETTLE,
		MEASURE,
		FINAL
	};
	const Rig rig;
	State state;
	int measureCounter; //Counter for the number of measurements to take
	const double fullPressure;
	double testPressures[]; //List of pressures to measure at
	const int settleTime;	//Measured in seconds
	const int pressureMeasureInterval; //Measured in seconds (Time between pressure measurements)
	const int pressureTotalCount;	//Number of pressure measurements to take. Single test time will be pressureMeasureInterval*pressureTotalCount
	int pressureCounter;

	//Data
	int pressureMeasurements[]; //Array of measurements taken during measurement period.
	double flowRate;	//liters per minute

	//Alarms
	bool alarmActive;


	int initial(void);
	int setSpeed(void);
	int settle(void);
	int measure(void);
	int final(void);


};





#endif /* LEAKAGETEST_H_ */
