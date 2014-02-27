/*
 * Machine.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: VQTEAM
 */

#include "Machine.h"

namespace Machine {

MachineInstance::MachineInstance(long numberOfQBits) {
	qBits = numberOfQBits;
	// our algorithm assumes that the number of active > number investigating > number inactive
    numberInactive = qBits * .20;
    numberInvestigating = qBits * .30;
    numberActive = qBits * .50;
    maxNumberVisits = 100;
	probInfluenced = 0.90; // probability inactive cycle is influenced by another cycle
	probMistake = 0.01; // probability an active cycle will reject a better node OR accept a worse node
	*cycles =  new Cycle[qBits];
    srand(0);  // set the seed for the random number generator

}

MachineInstance::~MachineInstance() {
}

long MachineInstance::Start(std::ostream cout) {

    cout << "\nStarting machine\n";
    cout << "Loading segment data for Traveling Salesman Problem analysis";

    cout << segmentData.ToString();

    cout << "Number segments = " + segmentData.size();
    cout << "Number of possible paths = " + segmentData.NumberOfPossiblePaths();
    cout << "Best possible solution (shortest path) length = " + segmentData.ShortestPathLength();

    this->bestSegmentData = GenerateRandomPath(); // alternative initializations are possible
    this->bestMeasureOfQuality = MeasureOfQuality(this->bestSegmentData);
    this->indexesOfInactiveCycles = new int[numberInactive]; // indexes of cycles which are currently inactive

	return 0L;
}

long MachineInstance::Stop(std::ostream cout) {
    cout << "Stop machine";
	return 0L;
}

long MachineInstance::Tick(std::ostream cout) {
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

      SegmentArray randomSegmentPath = GenerateRandomPath();
      double mq = randomSegmentPath.MeasureOfQuality();
      int numberOfVisits = 0;

      cycles[i] = new Cycle(currStatus, randomSegmentPath, mq, numberOfVisits); // instantiate current cycle

      // does this cycle have best solution?
      if (cycles[i]->measureOfQuality < this->bestMeasureOfQuality) // current cycle is better (< because smaller is better)
      {
    	  cycles[i]->segments.reserve(this->bestSegmentData.size());
    	  copy(this->bestSegmentData.begin(),cycles[i]->segments.end(),back_inserter(cycles[i]->segments));
          this->bestMeasureOfQuality = cycles[i]->measureOfQuality;
      }

    cout << "\nInitial random quantum machine state";
    cout << GetStatus();

    for (int i = 0; i < this->qBits; ++i) // each cycle
    {
      if (this->cycles[i]->status == 1) // active cycle
        ProcessActiveCycle(i);
      else if (this->cycles[i]->status == 2) // investigating cycle
    	  ProcessInvestigatingCycle(i);
      else if (this->cycles[i]->status == 0) // inactive cycle
        ProcessInactiveCycle(i);
    } // for each cycle

    cout << "\nChanged quantum machine state";
    cout << GetStatus();
    }
	return 0L;
}


std::string MachineInstance::GetStatus()
{
  std::string s = "";
  s += "Best path found: ";
  for (unsigned int i = 0; i < this->bestSegmentData.size() - 1; ++i)
    s += this->bestSegmentData[i].ToString() + "->";
  s += this->bestSegmentData[this->bestSegmentData.size()-1].ToString() + "\n";

  s += "Path quality:    ";
  if (bestMeasureOfQuality < 10000.0)
    s += bestMeasureOfQuality;
  else
    s += bestMeasureOfQuality;
  s += "\n";
  return s;
}

SegmentArray MachineInstance::GenerateRandomPath()
{
  SegmentArray result;
  result.Copy(segmentData);
  for (unsigned int i = 0; i < result.size(); i++) // Fisher-Yates (Knuth) shuffle
  {
    int r = random.Next(i, result.size());
    Segment temp = result[r]; result[r] = result[i]; result[i] = temp;
  }
  return result;
} // GenerateRandomPath()


SegmentArray MachineInstance::GenerateNeighborPath(SegmentArray segments)
{
  SegmentArray result;
  result.Copy(segments);
  unsigned int ranIndex = rand() % result.size(); // [0, Length-1] inclusive
  int adjIndex;
  if (ranIndex == result.size() - 1)
    adjIndex = 0;
  else
    adjIndex = ranIndex + 1;

  Segment tmp = result[ranIndex];
  result[ranIndex] = result[adjIndex];
  result[adjIndex] = tmp;

  return result;
} // GenerateNeighborPath()



void MachineInstance::ProcessActiveCycle(int i)
{
  SegmentArray neighbor = GenerateNeighborPath(cycles[i]->segments); // find a neighbor solution
  double neighborQuality = neighbor.MeasureOfQuality(); // get its quality
  double prob =  ((double) rand() / (RAND_MAX)); // used to determine if cycle makes a mistake; compare against probMistake which has some small value (~0.01)
  bool memoryWasUpdated = false; // used to determine if cycle should influence other cycles
  bool numberOfVisitsOverLimit = false; // used to determine if cycle will convert to inactive status

  if (neighborQuality < cycles[i]->measureOfQuality) // active cycle found better neighbor (< because smaller values are better)
  {
    if (prob < probMistake) // cycle makes mistake: rejects a better neighbor food source
    {
      ++cycles[i]->numberOfVisits; // no change to memory but update number of visits
      if (cycles[i]->numberOfVisits > maxNumberVisits) numberOfVisitsOverLimit = true;
    }
    else // cycle does not make a mistake: accepts a better neighbor
    {
      cycles[i]->segments.Copy(neighbor);
      cycles[i]->measureOfQuality = neighborQuality; // update the quality
      cycles[i]->numberOfVisits = 0; // reset counter
      memoryWasUpdated = true; // so that this cycle will influence other cycles
    }
  }
  else // active cycle did not find a better neighbor
  {
    if (prob < probMistake) // cycle makes mistake: accepts a worse neighbor segment path
    {
      cycles[i]->segments.Copy(neighbor);
      cycles[i]->measureOfQuality = neighborQuality; // update the quality
      cycles[i]->numberOfVisits = 0; // reset
      memoryWasUpdated = true; // so that this cycle will influence other cycles
    }
    else // no mistake: cycle rejects worse segment path
    {
      ++cycles[i]->numberOfVisits;
      if (cycles[i]->numberOfVisits > maxNumberVisits) numberOfVisitsOverLimit = true;
    }
  }

  if (numberOfVisitsOverLimit == true)
  {
    cycles[i]->status = 0; // current active cycle transitions to inactive
    cycles[i]->numberOfVisits = 0; // reset visits (and no change to this cycles memory)
    int x = rand() % numberInactive; // pick a random inactive cycle. x is an index into a list, not a cycle ID
    cycles[indexesOfInactiveCycles[x]]->status = 1; // make it active
    indexesOfInactiveCycles[x] = i; // record now-inactive cycle 'i' in the inactive list
  }
  else if (memoryWasUpdated == true) // current cycle returns and performs inter cycle communication
  {
    // first, determine if the new memory is a global best. note that if cycle has accepted a worse food source this can't be true
    if (cycles[i]->measureOfQuality < this->bestMeasureOfQuality) // the modified cycle's memory is a new global best (< because smaller is better)
    {
      this->bestSegmentData.Copy(cycles[i]->segments);
      this->bestMeasureOfQuality = cycles[i]->measureOfQuality; // update global best quality
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
  SegmentArray randomSegmentPath = GenerateRandomPath(); // investigating cycle finds a random segment path
  double randomSegmentPathQuality = MeasureOfQuality(randomSegmentPath); // and examines its quality
  if (randomSegmentPathQuality < cycles[i]->measureOfQuality) // investigating cycle has found a better solution than its current one (< because smaller measure is better)
  {
    cycles[i]->segments.Copy(randomSegmentPath);
    cycles[i]->measureOfQuality = randomSegmentPathQuality;
    // no change to cycles numberOfVisits or status

    // did this cycle find a better overall/global solution?
    if (cycles[i]->measureOfQuality < bestMeasureOfQuality) // yes, better overall solution (< because smaller is better)
    {
      this->bestSegmentData.Copy(cycles[i]->segments);
      this->bestMeasureOfQuality = cycles[i]->measureOfQuality;
    } // better overall solution

    InfluenceInactiveCycles(i); // investigating cycle returns to machine availability and attempts to influence other cycles

  } // if cycle found better solution
} // ProcessInvestigatingCycle()

void MachineInstance::ProcessInactiveCycle(int i)
{}

void MachineInstance::InfluenceInactiveCycles(int i)
{
  for (int ii = 0; ii < numberInactive; ++ii) // each inactive cycle
  {
    int b = indexesOfInactiveCycles[ii]; // index of an inactive cycle
    if (cycles[b]->status != 0) break;
    if (cycles[b]->numberOfVisits != 0) break;
    if (cycles[i]->measureOfQuality < cycles[b]->measureOfQuality) // investigating cycle has a better solution than current inactive cycle (< because smaller is better)
    {

      double p = random.NextDouble(); // will current inactive cycle be influenced by the cycle?
      if (this->probInfluenced > p) // this inactive cycle is persuaded by the investigation
      {
        cycles[b]->segments.Copy(cycles[i]->segments);
        cycles[b]->measureOfQuality = cycles[i]->measureOfQuality;
      } // inactive cycle has been persuaded
    } // investigating cycle has better solution than inactive cycle
  } // each inactive cycle
} // InfluenceInactiveCycles


} /* namespace Machine */
