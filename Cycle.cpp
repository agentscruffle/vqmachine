/*
 * Cycle.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: VQTEAM
 */

#include "Cycle.h"
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <set>


using namespace std;
namespace Machine {

Cycle::Cycle() {

}


Cycle::~Cycle() {

}

Cycle::Cycle(int status, std::vector<Segment> segmentList, double measureOfQuality, int numberOfVisits)
{
this->status = status;
this->segments = new Segment[segments.size()];
VectorCopy(this->segments, segmentList, segments.size());
this->measureOfQuality = measureOfQuality;
this->numberOfVisits = numberOfVisits;
}

string Cycle::ToString()
{
  string s = "";
  s += "Status = " + this->status + "\n";
  s += " Memory = " + "\n";
  for (int i = 0; i < this->segments.size()-1; ++i)
    s += this->segments[i] + "->";
  s += this->segments[this->segments.size()-1] + "\n";
  s += " Quality = " + this->measureOfQuality;
  s += " Number visits = " + this->numberOfVisits;
  return s;
}






} /* namespace Machine */
