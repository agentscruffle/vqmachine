/*
 * Machine.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: VQTEAM
 */


#include "Machine.h"

namespace Machine {

MachineInstance::MachineInstance(long numberOfQBits) {

	// number of qbits in the vqm
	qBits = numberOfQBits;

	// our algorithm assumes that the number of active > number investigating > number inactive
    numberInactive = qBits * .20;
    numberInvestigating = qBits * .30;
    numberActive = qBits * .50;

	probInfluenced = 0.90; // probability inactive cycle is influenced by another cycle
	probMistake = 0.01; // probability an active cycle will reject a better node OR accept a worse node
	cycles =  new Cycle[qBits];
    srand(0);  // set the seed for the random number generator

    for (int i = 0; i <  qBits; i++)
    {
    Point s;
    s.x = (rand() % 100) + 1 ;
    s.y = (rand() % 100) + 1;
    pointData.push_back(s);
    }

}

MachineInstance::~MachineInstance() {
}

long MachineInstance::Start() {

    long returnedstatus = 0;

    cout << "\nStarting machine\n";
    cout << "Loading point data for Traveling Salesman Problem analysis\n";

    cout << pointData.GetStatus() + "\n";

    cout << "Number points = " + Utils::ToString(pointData.Length()) + "\n";
    cout << "Number of possible paths = " + Utils::ToString(pointData.NumberOfPossiblePaths()) + "\n";
    cout << "Best possible solution (shortest path) length = " + Utils::ToString(pointData.ShortestPathLength()) + "\n";

    bestPointData = GenerateRandomPath(); // alternative initializations are possible
    indexesOfInactiveCycles = new int[numberInactive]; // indexes of cycles which are currently inactive

    while (returnedstatus == 0)
    	returnedstatus = Tick();

	return returnedstatus;
}

long MachineInstance::Stop() {
    cout << "Stop machine";
	return 0L;
}

long MachineInstance::Tick() {

    for (int i = 0; i < qBits; ++i) // initialize each cycle, and best solution
    {
      int currStatus;

      // depends on i. need status before we can initialize cycle
  	  // our algorithm assumes that the number of active > number investigating > number inactive

      if (i < numberInactive)
      {
        currStatus = 0; // inactive
        indexesOfInactiveCycles[i] = i; // current cycle is inactive
      }
      else if (i < numberInactive + numberInvestigating)
      {
        currStatus = 2; // investigating
      }
      else
      {
        currStatus = 1; // active
      }

      PointArray randomPointPath = GenerateRandomPath();
      double mq = randomPointPath.MeasureOfQuality();
      int numberOfVisits = 0;

      cycles[i].status = currStatus;
      cycles[i].points = randomPointPath;
      cycles[i].measureOfQuality = mq;
      cycles[i].numberOfVisits = numberOfVisits;

      // does this cycle have best solution?
      if (cycles[i].measureOfQuality < bestPointData.MeasureOfQuality()) // current cycle is better (< because smaller is better)
      {
    	  bestPointData.Copy(cycles[i].points);
      }

    cout << "\nInitial random quantum machine state\n";
    cout << GetStatus();

      if (cycles[i].status == 1) // active cycle
        ProcessActiveCycle(i);
      else if (cycles[i].status == 2) // investigating cycle
    	  ProcessInvestigatingCycle(i);
      else if (cycles[i].status == 0) // inactive cycle
        ProcessInactiveCycle(i);

    cout << "\nChanged quantum machine state\n";
    cout << GetStatus();
    }

	return 0L;
}


std::string MachineInstance::GetStatus()
{
  std::string s = "";
  s += "\nBest path found: ";
  for (unsigned int i = 0; i < bestPointData.size() - 1; ++i)
    s += bestPointData[i].ToString() + "->";
  s += bestPointData[bestPointData.size()-1].ToString() + "\n";

  s += "\nPath quality:    ";
    s += Utils::ToString(bestPointData.MeasureOfQuality());
  s += "\n";
  return s;
}

PointArray MachineInstance::GenerateRandomPath()
{
  PointArray result;
  result.Copy(pointData);
  for (unsigned int i = 0; i < result.size(); i++) // Fisher-Yates (Knuth) shuffle
  {
	unsigned int r = rand() % result.size(); // [0, Length-1] inclusive
    Point temp = result[r]; result[r] = result[i]; result[i] = temp;
  }
  return result;
} // GenerateRandomPath()


PointArray MachineInstance::GenerateNeighborPath(PointArray points)
{
  PointArray result;
  result.Copy(points);

  unsigned int ranIndex = rand() % result.size(); // [0, Length-1] inclusive
  int adjIndex;
  if (ranIndex == result.size() - 1)
    adjIndex = 0;
  else
    adjIndex = ranIndex + 1;

  Point tmp = result[ranIndex];
  result[ranIndex] = result[adjIndex];
  result[adjIndex] = tmp;

  return result;
} // GenerateNeighborPath()



void MachineInstance::ProcessActiveCycle(int i)
{
  PointArray neighbor = GenerateNeighborPath(cycles[i].points); // find a neighbor solution
  double neighborQuality = neighbor.MeasureOfQuality(); // get its quality
  double prob =  ((double) rand() / (RAND_MAX)); // used to determine if cycle makes a mistake; compare against probMistake which has some small value (~0.01)
  bool memoryWasUpdated = false; // used to determine if cycle should influence other cycles
  bool numberOfVisitsOverLimit = false; // used to determine if cycle will convert to inactive status

  if (neighborQuality < cycles[i].measureOfQuality) // active cycle found better neighbor (< because smaller values are better)
  {
    if (prob < probMistake) // cycle makes mistake: rejects a better neighbor food source
    {
      ++cycles[i].numberOfVisits; // no change to memory but update number of visits
      // we don't allow more visits per node than number of qBits
      if (cycles[i].numberOfVisits > qBits) numberOfVisitsOverLimit = true;
    }
    else // cycle does not make a mistake: accepts a better neighbor
    {
      cycles[i].points.Copy(neighbor);
      cycles[i].measureOfQuality = neighborQuality; // update the quality
      cycles[i].numberOfVisits = 0; // reset counter
      memoryWasUpdated = true; // so that this cycle will influence other cycles
    }
  }
  else // active cycle did not find a better neighbor
  {
    if (prob < probMistake) // cycle makes mistake: accepts a worse neighbor point path
    {
      cycles[i].points.Copy(neighbor);
      cycles[i].measureOfQuality = neighborQuality; // update the quality
      cycles[i].numberOfVisits = 0; // reset
      memoryWasUpdated = true; // so that this cycle will influence other cycles
    }
    else // no mistake: cycle rejects worse point path
    {
      ++cycles[i].numberOfVisits;
      // we don't allow more visits per node than we have the number of qBits
      if (cycles[i].numberOfVisits > qBits) numberOfVisitsOverLimit = true;
    }
  }

  if (numberOfVisitsOverLimit == true)
  {
    cycles[i].status = 0; // current active cycle transitions to inactive
    cycles[i].numberOfVisits = 0; // reset visits (and no change to this cycles memory)
    int x = rand() % numberInactive; // pick a random inactive cycle. x is an index into a list, not a cycle ID
    cycles[indexesOfInactiveCycles[x]].status = 1; // make it active
    indexesOfInactiveCycles[x] = i; // record now-inactive cycle 'i' in the inactive list
  }
  else if (memoryWasUpdated == true) // current cycle returns and performs inter cycle communication
  {
    // first, determine if the new memory is a global best. note that if cycle has accepted a worse food source this can't be true
    if (cycles[i].measureOfQuality < bestPointData.MeasureOfQuality()) // the modified cycle's memory is a new global best (< because smaller is better)
    {
      bestPointData.Copy(cycles[i].points);
    }
    InfluenceInactiveCycles(i); // cycle returns to machine availability and does communicates state to other cycles
  }
  else // number visits is not over limit and memory was not updated so do nothing (return to machine but do not influence)
  {
    return;
  }
} // ProcessActiveCycle()

void MachineInstance::ProcessInvestigatingCycle(int i)
{
  PointArray randomPointPath = GenerateRandomPath(); // investigating cycle finds a random point path
  double randomPointPathQuality = randomPointPath.MeasureOfQuality(); // and examines its quality
  if (randomPointPathQuality < cycles[i].measureOfQuality) // investigating cycle has found a better solution than its current one (< because smaller measure is better)
  {
    cycles[i].points.Copy(randomPointPath);
    cycles[i].measureOfQuality = randomPointPathQuality;
    // no change to cycles numberOfVisits or status

    // did this cycle find a better overall/global solution?
    if (cycles[i].measureOfQuality < bestPointData.MeasureOfQuality()) // yes, better overall solution (< because smaller is better)
    {
      bestPointData.Copy(cycles[i].points);
    } // better overall solution

    InfluenceInactiveCycles(i); // investigating cycle returns to machine availability and attempts to influence other cycles

  } // if cycle found better solution
} // ProcessInvestigatingCycle()

void MachineInstance::ProcessInactiveCycle(int i) {}

void MachineInstance::InfluenceInactiveCycles(int i)
{
  for (int ii = 0; ii < numberInactive; ++ii) // each inactive cycle
  {
    int b = indexesOfInactiveCycles[ii]; // index of an inactive cycle
    if (cycles[b].status != 0) break;
    if (cycles[b].numberOfVisits != 0) break;
    if (cycles[i].measureOfQuality < cycles[b].measureOfQuality) // investigating cycle has a better solution than current inactive cycle (< because smaller is better)
    {
      double p =  ((double) rand() / (RAND_MAX)); // will current inactive cycle be influenced by the cycle?
      if (probInfluenced > p) // this inactive cycle is persuaded by the investigation
      {
        cycles[b].points.Copy(cycles[i].points);
        cycles[b].measureOfQuality = cycles[i].measureOfQuality;
      } // inactive cycle has been persuaded
    } // investigating cycle has better solution than inactive cycle
  } // each inactive cycle
} // InfluenceInactiveCycles


} /* namespace Machine */
