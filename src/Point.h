/*
 * Segment.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#ifndef SEGMENT_H_
#define SEGMENT_H_
#include <algorithm>
#include <math.h>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>


namespace Machine {


class Utils
{
public:
	static std::string ToString (int);
	static std::string ToString (unsigned long);
	static std::string ToString (double);
};

class Point   {
private:

public:
	Point();
	virtual ~Point();
	std::string ToString();
	int x;
	int y;
};

} /* namespace Machine */
#endif /* SEGMENT_H_ */
