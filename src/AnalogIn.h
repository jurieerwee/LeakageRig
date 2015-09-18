/*
 * AnalogIn.h
 *
 *  Created on: 21 Aug 2015
 *      Author: Jurie
 */

//Controls the entire ADC IC



#ifndef ANALOGIN_H_
#define ANALOGIN_H_

class AnalogIn
{
public:
	AnalogIn();
	virtual ~AnalogIn();
	int readChannel(int channel);
	bool activateChannel(int channel);
	bool setup();

private:



	int adc;
};

#endif /* ANALOGIN_H_ */

