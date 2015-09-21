/*
 * FlowMeter.h
 *
 *  Created on: 21 Sep 2015
 *      Author: Jurie
 */

#ifndef SRC_FLOWMETER_H_
#define SRC_FLOWMETER_H_

#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <deque>
#include <time.h>

using namespace std;

namespace FlowInst
{
	deque<double> instRate;
	double 	runningSum;
	unsigned int		length;	//The number of elements taken into running average
	struct 	timespec flowTime;
	struct 	timespec flowTimePrev;
	int		counter =0;

	volatile bool dir;	//True = forward, false = backwards

	void flowInterrupt(void);
	void dirInterruptRise(void);
	void dirInterruptFall(void);
	struct timespec diff(struct timespec start, struct timespec end);

}

class FlowMeter {
public:
	FlowMeter(int _flowPin, int _dirPin, bool _pull, bool _pullUp,  double _factor);
	virtual ~FlowMeter();
	void init(void);
	bool getDir(void);
	double getRunningAve(void);
	bool clearCounter(void);
	int getCounter(void);
	double getLastAve(int count);

private:
	const int flowPin;
	const int dirPin;
	const bool pull;
	const bool pullUp;
	const int factor;
};

#endif /* SRC_FLOWMETER_H_ */
