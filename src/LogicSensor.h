/*
 * LogicSensor.h
 *
 *  Created on: 07 Jul 2015
 *      Author: Jurie
 */

#ifndef LOGICSENSOR_H_
#define LOGICSENSOR_H_

class LogicSensor {
public:
	LogicSensor(int _pin, bool _pull, bool _pullUp);
	virtual ~LogicSensor();
	bool update();
	bool getState();

private:
	const int pin;
	const bool pull;
	const bool pullUp = true;
	bool isHigh;


};

#endif /* LOGICSENSOR_H_ */
