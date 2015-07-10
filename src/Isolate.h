/*
 * Isolate.h
 *
 *  Created on: 10 Jul 2015
 *      Author: Jurie
 */

#ifndef SRC_ISOLATE_H_
#define SRC_ISOLATE_H_

#include <unistd.h>
#include <iostream>
#include <boost/log/trivial.hpp>

#include "Rig.h"

namespace logging = boost::log;

enum isoState
{
	PRE_TEST,
	TEST,
	POST_TEST

};

inline std::ostream& operator<<(std::ostream& out, const isoState value )
{
	static std::map<isoState, std::string> strings;
	if (strings.size() == 0){
	#define INSERT_ELEMENT(p) strings[p] = #p
		INSERT_ELEMENT(PRE_TEST);
		INSERT_ELEMENT(TEST);
		INSERT_ELEMENT(POST_TEST);
	#undef INSERT_ELEMENT
	}
	return out << strings[value];
}

class Isolate {
public:
	Isolate(Rig* _rig, int _testDuration, int _flowTolerance);
	virtual ~Isolate();

	int call();

private:
	int preTest();
	int test();
	int postTest();
	bool changeState(isoState newState);

	isoState state = PRE_TEST;
	Rig *rig;
	const int testDuration; //in seconds
	int flowVolume =0;
	const int flowTolerance; //Maximum value for wich no flow will be assumed.


	src::severity_logger_mt<>& lg;
};

#endif /* SRC_ISOLATE_H_ */
