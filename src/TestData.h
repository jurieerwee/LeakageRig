/*
 * TestData.h
 *
 *  Created on: 01 Jul 2015
 *      Author: Jurie
 */

#ifndef TESTDATA_H_
#define TESTDATA_H_

#include <vector>

class TestData {
public:
	TestData();
	TestData(int numberTestPoints);
	virtual ~TestData();

	bool setTestPressure(int index, double ave, double sd);
	bool setTestFlow(int index, double flow);
	bool setSystemPressure(double value);
	double getTestPressureAve(int index);
	double getTestPressureSD(int index);
	double getSystemPressure(void);
	double getTestFlow(int index);
	int getDataPoints();


private:
	std::vector<double> testPressuresAve;
	std::vector<double> testPressuresSD;
	std::vector<double> testFlows;
	double systemPressure;
	int dataPoints;	//Number of test data points
};

#endif /* TESTDATA_H_ */
