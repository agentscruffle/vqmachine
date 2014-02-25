/*
 * Cycle.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#ifndef CYCLE_H_
#define CYCLE_H_

#include "Segment.h"

namespace Machine {


class Cycle {
public:
    Cycle(int, std::vector<Segment>, double, int);
	Cycle();
	virtual ~Cycle();
	// Each cycle works on it's own group of segments
	std::vector<Segment> segments;
    int status; // 0 = inactive, 1 = active, 2 = investigating
    double measureOfQuality; // smaller values are better. total distance of path.
    int numberOfVisits;
    std::string ToString();

};



} /* namespace Machine */
#endif /* CYCLE_H_ */
