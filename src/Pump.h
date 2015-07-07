/*
 * Pump.h
 *
 *  Created on: 07 Jul 2015
 *      Author: Jurie
 */

#ifndef SRC_PUMP_H_
#define SRC_PUMP_H_

class Pump {
public:
	Pump(int _fullSpeed);
	virtual ~Pump();

	bool setSpeed(int speed);
	int getSpeed();
	int getFullSpeed();
	bool setPumpOn(bool set);
	bool getPumpOn();

private:
	//Setup
	int pumpFullSpeed;
	//State
	bool pumpOn;
	int pumpSpeed;
};

#endif /* SRC_PUMP_H_ */
