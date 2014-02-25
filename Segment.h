/*
 * Segment.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQMTEAM
 */

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace Machine {

class Segment  {
public:
	Segment();
	virtual ~Segment();
	double    Length();
	StartingTuple start;
	EndingTuple   end;
};

} /* namespace Machine */
#endif /* SEGMENT_H_ */
