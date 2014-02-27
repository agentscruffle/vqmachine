/*
 * Cycle.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#ifndef CYCLE_H_
#define CYCLE_H_

#include "SegmentArray.h"

namespace Machine {

class Cycle  {
public:
    Cycle(int, SegmentArray, double, int);
	Cycle();
	virtual ~Cycle();
	// Each cycle works on it's own group of segments
	SegmentArray segments;
    int status; // 0 = inactive, 1 = active, 2 = investigating
    double measureOfQuality; // smaller values are better. total distance of path.
    int numberOfVisits;
};



} /* namespace Machine */
#endif /* CYCLE_H_ */
