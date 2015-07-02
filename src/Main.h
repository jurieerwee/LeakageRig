/*
 * Main.h
 *
 *  Created on: 29 Jun 2015
 *      Author: jurie
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <boost/log/sources/logger.hpp>
#include <iostream>
#include <map>
#include <string>

#include "Rig.h"
#include "LeakageTest.h"

namespace src = boost::log:sources;

enum State	//Remember to update following naming when changing state definitions
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
//Code to print state name from : http://stackoverflow.com/questions/3342726/c-print-out-enum-value-as-text  (2/7/2015)
std::ostream& operator<<(std::ostream& out, const State value)
{
	static std::map<Errors, std::string> strings;
	if (strings.size() == 0){
#define INSERT_ELEMENT(p) strings[p] = #p
		INSERT_ELEMENT(PRE_START);
		INSERT_ELEMENT(SETUP);
		INSERT_ELEMENT(PREISOLATE);
		INSERT_ELEMENT(ISOLATE_TEST);
		INSERT_ELEMENT(LEAKAGE_TEST);
		INSERT_ELEMENT(DATA_PROCESSING);
		INSERT_ELEMENT(DATA_UPLOAD);
		INSERT_ELEMENT(POST_PROCESS);
#undef INSERT_ELEMENT
	}
	return out << strings[value];
}


class Main{

//Defines the main states of the state machine
public :
	//Main();
	//virtual ~Main();
	int mainProcess(int argc, const char* argv[]);

private:

	bool nextState();
	bool changeState(State newState);
	bool prevState();
	bool initLogger();

	src::logger_mt &lg;

};

#endif /* MAIN_H_ */
