/*
 *  SegmentArray.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: root
 */

#include "SegmentArray.h"

namespace Machine {


void SegmentArray::Copy(SegmentArray in) {
}

std::string SegmentArray::ToString() {
	return 0;
}

long SegmentArray::NumberOfPossiblePaths()
{
return 0L;
}

long SegmentArray::ShortestPathLength()
{
return 0L;
}

double SegmentArray::MeasureOfQuality()
{
  double answer = 0.0;
  for (unsigned int i = 0; i < this->size() - 1; ++i)
  {
    Segment s1 =  ((Segment*) this) [i];
    Segment s2 =  ((Segment*) this)[i + 1];
    double d = s1.Length() +  s2.Length();
    answer += d;
  }
  return answer;
} // MeasureOfQuality()


} /* namespace Machine */
