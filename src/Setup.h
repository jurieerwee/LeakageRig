/*
 * Setup.h
 *
 *  Created on: 09 Jul 2015
 *      Author: Jurie
 */

#ifndef SRC_SETUP_H_
#define SRC_SETUP_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Rig.h"
#include "TestData.h"

using namespace std;

enum stpState
{
	START_TANK_FILL,
	WAIT_TANK_FILL,
	STOP_TANK_FILL,
	PRE_PRESSURE_TEST,
	PRESSURE_TEST,
	POST_PRESSURE_TEST
};

inline std::ostream& operator<<(std::ostream& out, const stpState value )
	{
		static std::map<stpState, std::string> strings;
		if (strings.size() == 0){
	#define INSERT_ELEMENT(p) strings[p] = #p
			INSERT_ELEMENT(START_TANK_FILL);
			INSERT_ELEMENT(WAIT_TANK_FILL);
			INSERT_ELEMENT(STOP_TANK_FILL);
			INSERT_ELEMENT(PRE_PRESSURE_TEST);
			INSERT_ELEMENT(PRESSURE_TEST);
			INSERT_ELEMENT(POST_PRESSURE_TEST);
	#undef INSERT_ELEMENT
		}
		return out << strings[value];
}

class Setup {
public:
	Setup(Rig* _rig, TestData *_dataset, int _pressureMeasureInterval, int _pressureTotalCount);
	virtual ~Setup();

	int call();

private:
	int startTankFill();
	int waitTankFill();
	int stopTankFill();
	int prePressureTest();
	int pressureTest();
	int postPressureTest();

	bool changeState(stpState newState);

	Rig *rig;
	TestData *dataset;
	stpState state = START_TANK_FILL;
	src::severity_logger_mt<>& lg;

	 int pressureMeasureInterval=10;
	 int pressureTotalCount=1;

	 int measurementCounter =0;

	 //Data
	 vector<int> pressureMeasurements; //Array of measurements taken during measurement period.

	//Alarms
	bool alarmActive = false;


};

#endif /* SRC_SETUP_H_ */
