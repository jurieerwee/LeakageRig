/*
 * LeakageTest.h
 *
 *  Created on: 30 Jun 2015
 *      Author: Jurie
 */

#ifndef LEAKAGETEST_H_
#define LEAKAGETEST_H_

#include <vector>
#include <boost/log/trivial.hpp>
/*#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>*/
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <map>
#include <string>

#include "TestData.h"
#include "Rig.h"
//#include "Main.h"

namespace src = boost::log::sources;
namespace logging = boost::log;
namespace po = boost::program_options;
using namespace std;


namespace LeakageTest
{
enum ltState
	{
		INITIAL,
		SET_SPEED,
		SETTLE,
		MEASURE,
		FINAL
	};

inline std::ostream& operator<<(std::ostream& out, const ltState value)
	{
		static std::map<ltState, std::string> strings;
		if (strings.size() == 0){
	#define INSERT_ELEMENT(p) strings[p] = #p
			INSERT_ELEMENT(INITIAL);
			INSERT_ELEMENT(SET_SPEED);
			INSERT_ELEMENT(SETTLE);
			INSERT_ELEMENT(MEASURE);
			INSERT_ELEMENT(FINAL);
	#undef INSERT_ELEMENT
		}
		return out << strings[value];
}


class LeakageTest {
public:
	LeakageTest(Rig * _rig, TestData * _dataset, int _settleTime, int _pressureMeasureInterval, int _pressureTotalCount, const vector<double> _testPressures, int _testPressuresCount);
	virtual ~LeakageTest();

	int call();



	
	
private:	

	Rig *rig;
	TestData *dataset;
	ltState state;
	int measureCounter; //Counter for the number of measurements to take
	//const double fullPressure;
	const vector<double> testPressures; //List of pressures to measure at
	const int testPressuresCount; //Length of testPressures
	const int settleTime;	//Measured in seconds
	const int pressureMeasureInterval; //Measured in seconds (Time between pressure measurements)
	const int pressureTotalCount;	//Number of pressure measurements to take. Single test time will be pressureMeasureInterval*pressureTotalCount
	int pressureCounter;

	//Data
	vector<int> pressureMeasurements; //Array of measurements taken during measurement period.
	double flowRate;	//liters per minute

	//Alarms
	bool alarmActive;

	src::severity_logger_mt<>& lg;


	int initial(void);
	int setSpeed(void);
	int settle(void);
	int measure(void);
	int final(void);
	bool changeState(ltState newState);


};



}

#endif /* LEAKAGETEST_H_ */
