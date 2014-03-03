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
#include "PointArray.h"
#include "Cycle.h"

namespace Machine {

class MachineInstance {
public:
	long qBits;
	Cycle *cycles;
	MachineInstance(long);
	long Start();
	long Stop();
	long Tick();
	std::string GetStatus();
	PointArray GenerateRandomPath();
	PointArray GenerateNeighborPath(PointArray points);
	void ProcessActiveCycle(int i);
	void ProcessInvestigatingCycle(int i);
	void ProcessInactiveCycle(int i);
	void InfluenceInactiveCycles(int i);

	virtual ~MachineInstance();
	PointArray pointData; // this is the problem-specific data we want to optimize
	int totalNumberCycles;   // mostly for readability in the object constructor call
	int numberInactive;      // number of inactive cycles
	int numberActive;        // number of active cycles
	int numberInvestigating; // number of investigating cycles
	double probInfluenced; // probability inactive cycle is influenced by another cycle
	double probMistake; // probability an active cycle will reject a better node OR accept a worse node
	PointArray bestPointData;
	int     *indexesOfInactiveCycles; // contains indexes into the cycles array
};

} /* namespace Machine */
#endif /* MACHINE_H_ */







