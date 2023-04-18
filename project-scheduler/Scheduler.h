#pragma once
#include <fstream>
#include "Processor.h"
#include"FCFS.h"
#include"SJF.h"

class Scheduler {
	enum { maxProcessorsCount = 500 }; // maximum number of processors
	enum processorType {FCFS_processor, SJF_processor, RR_processor};
private:
	int processorsCount, processessCount, timeSteps, NF, NS, NR, RR_timeSlice, RTF, maxW, STL, forkProbability;
	Processor* processorsArray[maxProcessorsCount];
	LinkedQueue<Process> newList;
	ifstream inputFile;
public:
	Scheduler(const char* fileName): inputFile(fileName){
		timeSteps = processorsCount = processessCount = 0;
		loadInputFile();
	}
	void loadInputFile() {
		if (inputFile.is_open()) {
			inputFile >> NF >> NS >> NR;
			processorsCount = NF + NS + NR;

			for (int i = 0; i < processessCount; i++) {
				if (i < NF) processorsArray[i] = new FCFS;
				else if (i < NS) processorsArray[i] = new SJF;
				else processorsArray[i] = new SJF;
			}
			
			
			inputFile>> RR_timeSlice >> RTF >> maxW >> STL >> forkProbability >> processessCount;

		}
	}
};                                                                    