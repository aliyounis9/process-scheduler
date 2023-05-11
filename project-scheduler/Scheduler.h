#pragma once
#include <fstream>
#include "Processor.h"
//#include"FCFS.h"

#include <cstdlib>
#include <time.h>
#include"UI.h"




class Scheduler {
	enum { maxProcessorsCount = 500 }; // maximum number of processors
	enum processorType {FCFS_processor, SJF_processor, RR_processor};
	UI ui ; 
private:
	int processorsCount, processessCount, timeSteps, NF, NS, NR, RR_timeSlice, RTF, maxW, STL, forkProbability;
	Processor* processorsArray[maxProcessorsCount];
	LinkedQueue<Process*> newList, trmList, blkList;
	int maxID; 
	LinkedQueue<pair<int, int>> sigKillTimes;
	ifstream inputFile;
	Process * currentIo ;
public:
	Scheduler(const char* fileName): inputFile(fileName){
		timeSteps = processorsCount = processessCount = 0;
		maxID = 0;
		currentIo = nullptr ;
	}
	Scheduler(){
		timeSteps = processorsCount = processessCount = 0;
	}
	void loadInputFile();
	void simulator();
	//////////////////////////start coding for phase 2 ///////////////////////////////////////////////////////
	int gettimestep()const{
		return timeSteps ; 
	}
    void NewToRdy();
	void ToBLK(Process * ptr );
	void ToTRM(Process * ptr );
	void BLKToRDY();
	void doFork(Process*& running); 
	void killProcess(int id);
	void doWorkStealing();
};                                                                    