/*
 * Machine.h
 *
 *  Created on: Feb 13, 2014
 *      Author: VQTEAM
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "SegmentArray.h"
#include "Cycle.h"

namespace Machine {

class MachineInstance {
public:
	long qBits;
	Cycle **cycles;
	MachineInstance(long);
	long Start(std::ostream);
	long Stop(std::ostream);
	long Tick(std::ostream);
	std::string GetStatus();
	SegmentArray GenerateRandomPath();
	SegmentArray GenerateNeighborPath(SegmentArray segments);
	double MeasureOfQuality(SegmentArray segments);
	void ProcessActiveCycle(int i);
	void ProcessInvestigatingCycle(int i);
	void ProcessInactiveCycle(int i);
	void InfluenceInactiveCycles(int i);

	virtual ~MachineInstance();
	SegmentArray segmentData; // this is the problem-specific data we want to optimize
	int totalNumberCycles;   // mostly for readability in the object constructor call
	int numberInactive;      // number of inactive cycles
	int numberActive;        // number of active cycles
	int numberInvestigating; // number of investigating cycles
	int maxNumberVisits; // max number of times cycle will visit a node without finding a better neighbor
	double probInfluenced; // probability inactive cycle is influenced by another cycle
	double probMistake; // probability an active cycle will reject a better node OR accept a worse node
	SegmentArray bestSegmentData;
	double  bestMeasureOfQuality;
	int     *indexesOfInactiveCycles; // contains indexes into the cycles array
};

} /* namespace Machine */
#endif /* MACHINE_H_ */







