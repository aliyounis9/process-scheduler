#pragma once
#include <fstream>
#include "Processor.h"

class Scheduler {
	enum { maxProcessorsCount = 500 }; // maximum number of processors
	enum processorType {FCFS, SJF, RR};
private:
	int processorsCount, processessCount, timeSteps, NF, NS, NR, RTF, maxW, STL, forkProbability;
	Processor* processorsArray[maxProcessorsCount];
	LinkedQueue<Process> newList;
	Scheduler() {
		timeSteps= 0;
		processorsCount = 0;
	}
};