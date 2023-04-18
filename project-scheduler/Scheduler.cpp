#include"Scheduler.h"

void  Scheduler::loadInputFile() {
	if (inputFile.is_open()) { // if input file is open, scheduler will read all data
		inputFile >> NF >> NS >> NR;
		processorsCount = NF + NS + NR;

		// create processors of each type: they are categorized according to their indices
		for (int i = 0; i < processorsCount; i++) {
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
			newList.enqueue(currentProcess);
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


	while (trmList.getCount() != processessCount) {
		timeSteps++;
		int currentProcessor = 0;
		Process currentProcess;

		// checking the new list for processess arriving at current timestamp and moving them to RDY list of processors
		// TO BE MODIFIED: sharing copies of objects, instead of making new objects (using pointers)
		while (newList.peek(currentProcess) && currentProcess.getArrivalTime() == timeSteps) {
			newList.dequeue(currentProcess);
			Process* pointerToCurrentProcess = &currentProcess;
			processorsArray[currentProcessor]->AddToQueue(pointerToCurrentProcess); // why adding to processors is done through pointers??
			currentProcessor = (currentProcessor + 1) % processorsCount;
		}

		for (int i = 0; i < processessCount; i++){
			// if the processor is idle, it will run the first process on its queue; else, depending on probability, the running process of the processor
			// will be sent to the BLK, RDY, or TRM lists of the scheduler
			int prob = processorsArray[i]->Run();
			if (processorsArray[i]->isBusy()) {
				Process* runningProcess = processorsArray[i]->getRunning();
				trmList.enqueue(*runningProcess);
			}
		}

	}

}