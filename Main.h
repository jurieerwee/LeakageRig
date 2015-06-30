/*
 * Main.h
 *
 *  Created on: 29 Jun 2015
 *      Author: jurie
 */

#ifndef MAIN_H_
#define MAIN_H_

enum State
{
	PRE_START,
	SETUP,
	PREISOLATE,
	ISOLATE_TEST,
	LEAKAGE_TEST,
	DATA_PROCESSING,
	DATA_UPLOAD,
	POST_PROCESS
};

class Main{

//Defines the main states of the state machine
public :
	Main();
	virtual ~Main();
	int mainProcess(int argc, const char* argv[]);

private:

	bool nextState();
	bool changeState(State newState);


};

#endif /* MAIN_H_ */
