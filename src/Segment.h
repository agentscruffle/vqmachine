/*
 * Segment.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "StartingTuple.h"
#include "EndingTuple.h"

namespace Machine {

class Segment   {
public:
	Segment();
	virtual ~Segment();
	double    Length();
	std::string ToString();
	StartingTuple start;
	EndingTuple   end;
};

} /* namespace Machine */
#endif /* SEGMENT_H_ */
