/*
 * Main.h
 *
 *  Created on: 29 Jun 2015
 *      Author: jurie
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <iostream>
#include <map>
#include <string>

#include "Rig.h"
#include "TestData.h"
#include "LeakageTest.h"

namespace src = boost::log::sources;
namespace logging = boost::log;

enum severity_level
{
    NORMAL,
    NOTIFICATION,
    WARNING,
    ERROR,
    CRITICAL
};

// The operator is used when putting the severity level to log
logging::formatting_ostream& operator<<
(
    logging::formatting_ostream& strm,
    severity_level level
)
{
    static const char* strings[] =
    {
        "NORM",
        "NTFY",
        "WARN",
        "ERRR",
        "CRIT"
    };

    //severity_level level = manip.get();
    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

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
	static std::map<State, std::string> strings;
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
//	return "Hi";
}

//BOOST_LOG_GLOBAL_LOGGER(my_logger, src::severity_logger)

class Main{

//Defines the main states of the state machine
public :
	Main();
	//virtual ~Main();
	int mainProcess(int argc, const char* argv[]);

private:

	bool nextState();
	bool changeState(State newState);
	bool prevState();
	bool initLogger();

	//src::severity_logger_mt<severity_level>& lg;

};

#endif /* MAIN_H_ */
