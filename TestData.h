/*
 * TestData.h
 *
 *  Created on: 01 Jul 2015
 *      Author: Jurie
 */

#ifndef TESTDATA_H_
#define TESTDATA_H_

class TestData {
public:
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
	double *testPressuresAve;
	double *testPressuresSD;
	double *testFlows;
	double systemPressure;
	int dataPoints;	//Number of test data points
};

#endif /* TESTDATA_H_ */
