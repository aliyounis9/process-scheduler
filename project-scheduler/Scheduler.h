#pragma once
#include <fstream>
#include "Processor.h"

class Scheduler {
	enum {MaxProcessorsCount = 500}; // maximum number of processors
private:
	int processorsCount;
	Processors* processorsArray[MaxProcessorsCount];

};