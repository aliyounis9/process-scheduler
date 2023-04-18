#include"Scheduler.h"

void  Scheduler::loadInputFile() {
	if (inputFile.is_open()) { // if input file is open, scheduler will read all data
		inputFile >> NF >> NS >> NR;
		processorsCount = NF + NS + NR;

		// create processors of each type: they are categorized according to their indices
		for (int i = 0; i < processessCount; i++) {
			if (i < NF) processorsArray[i] = new FCFS;
			else if (i < NS) processorsArray[i] = new SJF;
			else processorsArray[i] = new SJF;
		}

		inputFile >> RR_timeSlice >> RTF >> maxW >> STL >> forkProbability >> processessCount;

		// read data of each individual process then creating it
		for (int i = 0; i < processessCount; i++) {
			int at, pid, ct, io_count;
			inputFile >> at >> pid >> ct >> io_count;
			Process currentProcess(at, pid, ct, io_count);
			for (int j = 0; j < io_count; j++) {
				char c;
				int st, end;
				inputFile >> c >> st >> c >> end >> c;
				if (j + 1 < io_count) inputFile >> c;
				IO curIO(st, end);
				currentProcess.AddIO(curIO);
			}
		}

		// read the kill time and process id until it reaches the end of the input file
		while (!inputFile.eof()) {
			int killTime, processID;
			inputFile >> killTime >> processID;
			pair<int, int> processToKill;
			processToKill.first = killTime, processToKill.second = processID;
			sigKillTimes.enqueue(processToKill);
		}

		inputFile.close();
	}
}

void Scheduler::simulator() {
	loadInputFile();

}