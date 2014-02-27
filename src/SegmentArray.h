
#ifndef SEGVECTORS_H_
#define SEGVECTORS_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Segment.h"

using namespace std;

namespace Machine {
class SegmentArray : public vector<Segment> {
private:
public:
	void Copy(SegmentArray);
	std::string ToString();
	long NumberOfPossiblePaths();
	long ShortestPathLength();
	double MeasureOfQuality();


};


} /* namespace Machine */

#endif /* SEGVECTORS_H_ */
