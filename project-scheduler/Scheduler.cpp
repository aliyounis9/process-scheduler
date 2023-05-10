#include"Scheduler.h"

void  Scheduler::loadInputFile() {
	if (inputFile.is_open()) { // if input file is open, scheduler will read all data
		inputFile >> NF >> NS >> NR;
		processorsCount = NF + NS + NR;


		inputFile >> RR_timeSlice >> RTF >> maxW >> STL >> forkProbability >> processessCount;

		// create processors of each type: they are categorized according to their indices
		for (int i = 0; i < processorsCount; i++) {
			if (i < NF) processorsArray[i] = new FCFS;
			else if (i < NS+NF) processorsArray[i] = new SJF;
			else processorsArray[i] = new RR(RR_timeSlice) ;
		}

		// read data of each individual process then creating it
		for (int i = 0; i < processessCount; i++) {
			int at, pid, ct, io_count;
			inputFile >> at >> pid >> ct >> io_count;
			Process* currentProcess = new Process(at, pid, ct, io_count);
			for (int j = 0; j < io_count; j++) {
				char c;
				int st, end;
				inputFile >> c >> st >> c >> end >> c;
				if (j + 1 < io_count) inputFile >> c;
				IO curIO(st, end);
				currentProcess->AddIO(curIO);
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
	ui.setTimeStep(timeSteps);

	int currentProcessor = 0;
	while (trmList.getCount() != processessCount ) {
		timeSteps++;
		Process* tempProcess;

		// checking the new list for processess arriving at current timestamp and moving them to RDY list of processors
		// TO BE MODIFIED: sharing copies of objects, instead of making new objects (using pointers)
		while (newList.peek(tempProcess) && tempProcess->getArrivalTime() == timeSteps) {
			newList.dequeue(tempProcess);
			Process* cur = tempProcess;
			processorsArray[currentProcessor]->AddToQueue(cur); // why adding to processors is done through pointers??
			currentProcessor = (currentProcessor + 1) % processorsCount;
			tempProcess = nullptr;
		}


		// checking the RDY List of each processor and moving a process from the RDY list to the running state iff the processor is idle
		for (int i = 0; i < processorsCount; i++) {
			if (!processorsArray[i]->isBusy()) {
				processorsArray[i]->setRun(timeSteps);
			}
		}

		// for each process in the run state, it will be either sent to the BLK, RDY, or TRM lists of the scheduler or remain as it (depending on probability)
		for (int i = 0; i < processorsCount; i++){
			Process* currentRunningProcess = nullptr;
			int prob = processorsArray[i]->Run(currentRunningProcess);
			if (prob == 1)
				blkList.enqueue(currentRunningProcess);
			else if (prob == 2) {
 				processorsArray[currentProcessor]->AddToQueue(currentRunningProcess);
				currentProcessor = (currentProcessor + 1) % processorsCount;
			}
			else if (prob == 3) {
				currentRunningProcess->setTerminateTime(timeSteps);
				trmList.enqueue(currentRunningProcess);
			}
		}

		// generate a random number (1-100) and if this number is < 10, move the process from the BLK to RDY
		int randomNumber = 5 ;//(rand() % 100) + 1;
		if (randomNumber < 10) {
			Process* processInBlk = 0;
			if (!blkList.isEmpty()) {
				blkList.dequeue(processInBlk);
				processorsArray[currentProcessor]->AddToQueue(processInBlk);
				currentProcessor = (currentProcessor + 1) % processorsCount;
			}
		}

		// generate a random ID (1-100) and if this number is in any FCFS RDY list, it will be killed
		int randomID = (rand() % 100) + 1;
		Process* toKill = nullptr;
		for (int i = 0; i < NF && !toKill; i++) {
			if (processorsArray[i]->exist(randomID, toKill)) {
				toKill->setTerminateTime(timeSteps);
				trmList.enqueue(toKill);
			}
		}

		ui.setTimeStep(timeSteps);
		//if(timeSteps %5 == 0) ui.Print(processorsArray, processessCount, NF, NS, NR, &blkList, &trmList);
		

		ui.Print(processorsArray, processorsCount, NF, NS, NR, &blkList, &trmList);
		ui.continueprinting();


	}
	cout << timeSteps << endl;
}
//this func should handle moving processes from new list to the shortest ready queue of the  processoers BY BAHR 
void Scheduler::NewToRdy(){
	Process * tempProcess = 0;
	while (newList.peek(tempProcess) && tempProcess->getArrivalTime() == timeSteps) {
			newList.dequeue(tempProcess);
			int Processorindex = 0 ; 
			for (int i = 0; i < processessCount; i++)
			{
				if(processorsArray[i]->GetTimeLeft()<processorsArray[Processorindex]->GetTimeLeft() )
					Processorindex = i ;
			}
			processorsArray[Processorindex]->AddToQueue(tempProcess);
			tempProcess = nullptr;
		}
}


void Scheduler :: ToBLK(Process * ptr ){
	if(ptr){
		blkList.enqueue(ptr);
	}
}

void Scheduler :: ToTRM(Process * ptr ){
	if(ptr){
		trmList.enqueue(ptr);
	}
}