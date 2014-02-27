/*
 * Cycle.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#include "Cycle.h"

namespace Machine {

Cycle::Cycle() {
}

Cycle::~Cycle() {

}

Cycle::Cycle(int status, SegmentArray segmentList,
		double measureOfQuality, int numberOfVisits) {
	this->status = status;
	segmentList.Copy(this->segments);
	this->measureOfQuality = measureOfQuality;
	this->numberOfVisits = numberOfVisits;
}

} /* namespace Machine */
