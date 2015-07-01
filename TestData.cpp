/*
 * TestData.cpp
 *
 *  Created on: 01 Jul 2015
 *      Author: Jurie
 */
#include <exception>
#include <vector>

#include "TestData.h"


using namespace std;

TestData::TestData(int numberTestPoints) {
	// TODO Auto-generated constructor stub

	this->dataPoints 		= numberTestPoints;
	//Change to vectors
	/*this->testPressuresAve 	= new double[numberTestPoints];
	this->testPressuresSD 	= new doubel[numberTestPoints];
	this->testFlows 		= new double[numberTestPoints];*/

	this->testPressuresAve 	= {0};
	this->testPressuresSD 	= {0};
	this->testFlows 		= {0};
	this->systemPressure 	= 0;
}

TestData::~TestData() {
	// TODO Auto-generated destructor stub
	/*
	delete [] this->testPressuresAve;
	delete [] this->testPressuresSD;
	delete [] this->testFlows;*/
}

bool TestData::setTestPressure(int index, double ave, double sd)
{
	if(index<0 || index >= this->dataPoints)
		return false;

	this->testPressuresAve[index] = ave;
	this->testPressuresSD[index] = sd;

	return true;
}

bool TestData::setTestFlow(int index, double flow)
{
	if(index<0 || index >= this->dataPoints)
			return false;

	this->testFlows[index] = flow;
	return true;
}

bool TestData::setSystemPressure(double value)
{
	this->systemPressure = value;

	return true;
}
double TestData::getTestPressureAve(int index)
{
	if(index<0 || index >=this->dataPoints)
		throw out_of_range("Get test pressure index out of range");

	return this->testPressuresAve[index];
}
double TestData::getTestPressureSD(int index)
{
	if(index<0 || index >=this->dataPoints)
		throw out_of_range("Get test pressure index out of range");

	return this->testFlows[index];
}
double TestData::getSystemPressure(void)
{
	return this->systemPressure;
}
double TestData::getTestFlow(int index)
{
	if(index<0 || index >=this->dataPoints)
		throw out_of_range("Get test pressure index out of range");

	return this->testFlows[index];
}

int TestData::getDataPoints()
{
	return this->dataPoints;
}
