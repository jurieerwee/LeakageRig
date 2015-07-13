/*
 * MyMath.h
 *
 *  Created on: 13 Jul 2015
 *      Author: Jurie
 */

#ifndef SRC_MYMATH_H_
#define SRC_MYMATH_H_

#include <vector>
#include <unistd.h>

namespace myMath
{

	bool linearRegression(std::vector<double> pressures, std::vector<double> flows, double &m, double& c);


} /* namespace myMath */

#endif /* SRC_MYMATH_H_ */
