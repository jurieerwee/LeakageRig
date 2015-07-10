/*
 * TestData.cpp
 *
 *  Created on: 01 Jul 2015
 *      Author: Jurie
 */
#include <stdexcept>
#include <vector>

#include "TestData.h"


using namespace std;

TestData::TestData()
{
	this->dataPoints 		= 10;
	this->testPressuresAve 	= {0};
	this->testPressuresSD 	= {0};
	this->testFlows 		= {0};
	this->systemPressureAve = 0;
	this->systemPressureSD	= 0;
}

TestData::TestData(int numberTestPoints) {
	// TODO Auto-generated constructor stub

	this->dataPoints 		= numberTestPoints;
	//Change to vectors
	/*this->testPressuresAve 	= new double[numberTestPoints];
	this->testPressuresSD 	= new doubel[numberTestPoints];
	this->testFlows 		= new double[numberTestPoints];*/

	this->testPressuresAve.clear();
	this->testPressuresSD.clear();
	this->testFlows.clear();
	this->systemPressureAve = 0;
	this->systemPressureSD	= 0;
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

	this->testPressuresAve.push_back(ave);
	this->testPressuresSD.push_back(sd);

	return true;
}

bool TestData::setTestFlow(int index, double flow)
{
	if(index<0 || index >= this->dataPoints)
			return false;

	this->testFlows.push_back(flow);
	return true;
}

bool TestData::setSystemPressure(double ave, double sd)
{
	this->systemPressureAve = ave;
	this->systemPressureSD = sd;

	return true;
}
double TestData::getTestPressureAve(int index)
{
	if(index<0 || index >=this->dataPoints)
		throw out_of_range ("Get test pressure index out of range");

	return this->testPressuresAve[index];
}
double TestData::getTestPressureSD(int index)
{
	if(index<0 || index >=this->dataPoints)
		throw out_of_range ("Get test pressure index out of range");

	return this->testFlows[index];
}
double TestData::getSystemPressure(void)
{
	return this->systemPressureAve;
}
double TestData::getTestFlow(int index)
{
	if(index<0 || index >=this->dataPoints)
		throw out_of_range ("Get test pressure index out of range");

	return this->testFlows[index];
}

int TestData::getDataPoints()
{
	return this->dataPoints;
}
