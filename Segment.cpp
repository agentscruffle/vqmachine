/*
 * Segments.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#include "Segment.h"

namespace Machine {

Segment::Segment() {

}

Segment::~Segment() {

}

double Segment::Length() {
	return sqrt((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y));
}

} /* namespace Machine */
