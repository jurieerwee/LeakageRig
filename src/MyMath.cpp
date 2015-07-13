/*
 * MyMath.cpp
 *
 *  Created on: 13 Jul 2015
 *      Author: Jurie
 */

#include "MyMath.h"

//#include <boost/numeric/ublas/matrix.hpp>
#include <unistd.h>
#include <vector>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

/*bool myMath::linearRegression(std::vector<double> pressures, std::vector<double> flows, double &m, double& c)
{
	using namespace boost::numeric::ublas;

	if(pressures.size()!= flows.size())
	{
		return false;
	}

	matrix<double> X (pressures.size(),2);
	vector<double> Y (pressures.size());

	for(int i=0; i<pressures.size();i++)
	{
		X(i,0) =1;
		X(i,1) = pressures[i];
		Y(i) = flows[i];
	}

	//vector<double> b (2);
	matrix<double> temp1 = - prod(trans(X),X);
	matrix<double> temp2 = prod(trans(X),Y);

	matrix<double> b = prod(temp1,temp2);

	c = b(0);
	m = b(1);

	return true;

}*/

/*
http://stackoverflow.com/questions/18939869/how-to-get-the-slope-of-a-linear-regression-line-using-c
Qué Padre
13/7/2015
*/
bool myMath::linearRegression(std::vector<double> pressures, std::vector<double> flows, double &m, double& c)
{

	    if(pressures.size() != flows.size()){
	        //throw exception("...");
	        return false;
	    }
	    double n = pressures.size();

	    double avgX = accumulate(pressures.begin(), pressures.end(), 0.0) / n;
	    double avgY = accumulate(flows.begin(), flows.end(), 0.0) / n;

	    double numerator = 0.0;
	    double denominator = 0.0;

	    for(int i=0; i<n; ++i){
	        numerator += (pressures[i] - avgX) * (flows[i] - avgY);
	        denominator += (pressures[i] - avgX) * (pressures[i] - avgX);
	    }

	    if(denominator == 0){
	        //throw exception("...");
	    	return false;
	    }

	    m = numerator / denominator;
	    c = avgY - avgX * m;




}
