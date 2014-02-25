/*
 * Machine.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQTEAM
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include <stdlib.h>
#include "Segment.h"

namespace Machine {

class MachineInstance {
public:
	long qBits;
	std::vector<Cycle> cycles;
	MachineInstance(long);
	long Start();
	long Stop();
	long Tick();
	std::string GetStatus();
	std::vector<Segment> GenerateRandomPath();
	std::vector<Segment> GenerateNeighborPath(std::vector<Segment> segments);
	double MeasureOfQuality(std::vector<Segment> segments);
	void ProcessActiveCycle(int i);
	void ProcessInvestigatingCycle(int i);
	void InfluenceInactiveCycles(int i);

	virtual ~MachineInstance();
	std::vector<Segment> segmentData; // this is the problem-specific data we want to optimize
	int totalNumberCycles;   // mostly for readability in the object constructor call
	int numberInactive;      // number of inactive cycles
	int numberActive;        // number of active cycles
	int numberInvestigating; // number of investigating cycles
	int maxNumberVisits; // max number of times cycle will visit a node without finding a better neighbor
	double probInfluenced = 0.90; // probability inactive cycle is influenced by another cycle
	double probMistake = 0.01; // probability an active cycle will reject a better node OR accept a worse node
	std::vector<Segment> bestSegmentData;
	double  bestMeasureOfQuality;
	int     indexesOfInactiveCycles[]; // contains indexes into the cycles array
};

} /* namespace Machine */
#endif /* MACHINE_H_ */







