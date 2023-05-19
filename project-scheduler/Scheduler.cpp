#include"Scheduler.h"
#include "FCFS.h"
#include"RR.h"
#include"SJF.h"

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
			maxID = max(maxID, pid);
			Process* currentProcess = new Process(at, pid, ct, io_count);
			for (int j = 0; j < io_count; j++) {
				char c;
				int st, end;
				inputFile >> c >> st >> c >> end >> c;
				if (j + 1 < io_count) inputFile >> c;
				IO * curIO= new IO(st, end);
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
	ui.continueprinting();
	loadInputFile();
	ui.setTimeStep(timeSteps);

	while (trmList.getCount() != processessCount ) {
		timeSteps++;
		ui.setTimeStep(timeSteps);

		// moving processes from the NEW list to the RDY lists of processors 
		NewToRdy();


		// 1 - checking the RDY List of each processor and moving a process from the RDY list to the running state iff the processor is idle
		// 2 - sending instructions to the scheduler. These instructions include:
		//       a - moving processes requesting IO from RUN to BLK (through toBLK function)
		//       b - moving porocesses that finish execution from RUN to TRM list (through toTRM function)
		//       c - do process migration operations (through doMigrationFCFS and doMigrationRR functions)
		//       d - do orphan killing operations (through killProcess function)
		// this is done through the schedAlgo function for each processor
		for (int i = 0; i < processorsCount; i++) {
			processorsArray[i]->SchedAlgo(this);
		}

		// move processes that finish IO from BLK to RDY
		BLKToRDY();


		// do work stealing operations
		if (timeSteps % STL == 0) {
			doWorkStealing();
		}


		// do forking operations (probability-dependent) and place forked processes at appropriate RDY lists
		for(int i = 0; i < NF; i++){
			processorsArray[i]->checkFork(forkProbability, this);
		}


		// do process killing
		pair<int, int> toKillProcess;
		if(sigKillTimes.peek(toKillProcess)){
			if(toKillProcess.first == timeSteps){
				sigKillTimes.dequeue(toKillProcess);
				killProcess(toKillProcess.second);
			}
		}
 

		// calls UI class functions to print details on the output screen depending on the mode of operation
		if(modeOfOperation < 3)ui.Print(processorsArray, processorsCount, NF, NS, NR, &blkList, &trmList, currentIo);
		if (modeOfOperation == 1)  ui.continueprinting();
		else if (modeOfOperation == 2) ui.continueWithDelay();

  

	}
	// generating output file and deleting processes 
	output();

	 //deleting processors
	for (int i = 0; i < processorsCount; i++) {
		delete processorsArray[i];
	}

}

//this func should handle moving processes from new list to the shortest ready queue of the  processors
void Scheduler::NewToRdy(){
	Process * tempProcess = nullptr;
	while (newList.peek(tempProcess) && tempProcess->getArrivalTime() == timeSteps) {
			newList.dequeue(tempProcess);
			int Processorindex = 0 ; 
			for (int i = 0; i < processorsCount; i++)
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


void Scheduler::doFork(Process*& running){
	
	int ct = running->getTimeLeft();
	Process* currentProcess = new Process(timeSteps, ++maxID, ct, 0);
	processessCount++;
	forkCount++;
	running->setChild(currentProcess);
	currentProcess->setParent(running);
	int Processorindex = 0 ; 
	for (int i = 0; i < NF; i++)
	{		
		if(processorsArray[i]->GetTimeLeft()<processorsArray[Processorindex]->GetTimeLeft() )
				Processorindex = i ;
	}
	processorsArray[Processorindex]->AddToQueue(currentProcess);
	currentProcess = nullptr;
}

void Scheduler::doWorkStealing(){
	int LQF = 0 , SQF = INT_MAX; 
	int Lindex =-1 , Sindex=-1 ; 
	for (int i = 0; i < processorsCount; i++){
		int t = processorsArray[i]->GetTimeLeft();
		if(t > LQF) LQF = t, Lindex = i;
		if(t < SQF) SQF = t, Sindex = i;
	}
	
	while(~Lindex && ~Sindex && LQF+0ll-SQF > 40ll*LQF/100 && !(processorsArray[Lindex]->getReadyQ()->isEmpty()) ){
		Process * Temp = nullptr ; 
		processorsArray[Lindex]->getReadyQ()->dequeue(Temp);
		if(Temp){
			workStealCount++;
		    processorsArray[Sindex]->AddToQueue(Temp);
			processorsArray[Lindex]->SetQueueTimeLeft(processorsArray[Lindex]->GetTimeLeft()-Temp->getTimeLeft());
		}
		LQF = processorsArray[Lindex]->GetTimeLeft();
		SQF = processorsArray[Sindex]->GetTimeLeft();
	}
}

void Scheduler::killProcess(int id){
	Process *toKill = nullptr;
	bool found = false;
	for(int i= 0; i < NF && !found; i++){
		if(processorsArray[i]->exist(id, toKill) || (processorsArray[i]->getrun() && processorsArray[i]->getrun()->getID() == id)) {
			found = true;
			if (!toKill) {
				toKill = processorsArray[i]->getrun();
				processorsArray[i]->clearRun();
			}
		}
		if(found && !toKill->getIsKilled()){
			Process *firstChild = toKill->getFirstChild();
			Process *secondChild = toKill->getSecondChild();
			if(firstChild) killProcess(firstChild->getID()); 
			if(secondChild) killProcess(secondChild->getID());
			int predictedWT = timeSteps - toKill->getArrivalTime()-(toKill->getCPUtime()-toKill->getTimeLeft());
			toKill->setWaitingTime(predictedWT);

			ToTRM(toKill);
			killedCount++;
			toKill = nullptr;
		}
	}
}

void Scheduler :: ToTRM(Process * ptr ){
	if(ptr){
		ptr->setTerminateTime(timeSteps);
		ptr->setTurnAroundTime(ptr->getTerminationTime()-ptr->getArrivalTime());
		addToTotalTRT(ptr->getTurnAroundTime());
		if(ptr->getWaitingTime()==0 &&ptr->getTurnAroundTime()-ptr->getCPUtime() > 0 )
		ptr->setWaitingTime(ptr->getTurnAroundTime()-ptr->getCPUtime());
		addToTotalWT(ptr->getWaitingTime());
		ptr->setIsKilled(true);
		trmList.enqueue(ptr);
	}
}

void Scheduler :: BLKToRDY(){
	if(!currentIo){
		blkList.dequeue(currentIo);
	}
	if(currentIo){
	currentIo->getNextIO()->setTimeLeft(currentIo->getNextIO()->getTimeLeft()-1);
	if(currentIo->getNextIO()->getTimeLeft()==0){
		currentIo->getNextIO()->setDone(1);
		int Processorindex = 0 ; 
		for (int i = 0; i < processorsCount; i++)
		{
			if(processorsArray[i]->GetTimeLeft()<processorsArray[Processorindex]->GetTimeLeft() )
				Processorindex = i ;
		}
		processorsArray[Processorindex]->AddToQueue(currentIo);
		currentIo->popIO();
		delete currentIo->getNextIO(); // deleting IOs
		currentIo = nullptr ;
	}
	}

}

int Scheduler:: getAvgWT(){
     return avgWT ;
}

int Scheduler:: getAvgRT(){
     return avgRT ;
}

int Scheduler:: getAvgTRT(){
     return avgTRT ;
}


void Scheduler:: output(){
	outputFile <<"TT  PID  AT  CT  IO_D  WT  RT  TRT"<<endl;
	while(!trmList.isEmpty()){
		Process * ptr = nullptr ; 
		trmList.dequeue(ptr);
		addToTotalRT(ptr->getResponseTime());
		outputFile<<ptr->getTerminationTime()<<"  "<<ptr->getID()<<"  "<<ptr->getArrivalTime()<<"  "<<ptr->getCPUtime()<<"  "<<ptr->getTotalIO_D()<<"  "<<ptr->getWaitingTime()<<"  "<<ptr->getResponseTime()<<"  "<<ptr->getTurnAroundTime()<<endl;
		delete ptr; // deleting process
	}
	outputFile<<"Processes: "<<processessCount<<endl;
	outputFile<<"Avg WT = "<<double(totalWT)/processessCount<<",	 Avg RT = "<<double(totalRT)/processessCount<<",		Avg TRT = "<<double(totalTRT)/processessCount<<endl;
	outputFile<<"Migration %:	RTF = "<<(1.0*RTFCount*100)/(1.0*processessCount)<<"%,	MaxW = "<<(1.0*100*MaxwCount)/(1.0*processessCount)<<"%"<<endl;
	outputFile<<"Work Steal%: "<<(1.0*100*workStealCount)/(processessCount*1.0)<<"%"<<endl;
	outputFile<<"Forked Process: "<<(100.0*forkCount)/(1.0*processessCount)<<"%"<<endl;
	outputFile<<"Killed Process: "<<(100.0*killedCount)/processessCount<<"%"<<endl<<endl;
	outputFile<<"Processors: "<<processorsCount<<" ["<<NF<<" FCFS, "<<NS<<" SJF, "<<NR<<" RR]"<<endl;
	outputFile<<"Processors Load"<<endl;
	for (int i = 0; i < processorsCount; i++)
	{
		outputFile<<"P"<<i+1<<"="<<(100.0*processorsArray[i]->getBusyTime())/totalTRT<<"%,	" ;
	}
	outputFile<<endl<<endl;
	outputFile<<"Processors Utiliz"<<endl;
    for (int i = 0; i < processorsCount; i++)
	{
		outputFile<<"P"<<i+1<<"="<<(100.0*processorsArray[i]->getBusyTime())/(processorsArray[i]->getBusyTime()+processorsArray[i]->getIdelTime())<<"%,	" ;
	}
	outputFile<<endl;
	calcTotalUtiliz();
	outputFile<<"Avg Utilization = "<<(100.0*TotalUtiliz)/processorsCount<<"%"<<endl;


}



void Scheduler::doMigrationRR(Process * ptr){
	RTFCount++;
	int ind = NF ; 
	for (int i = NF; i < NS; i++)
	{
		if(processorsArray[i]->GetTimeLeft()<processorsArray[ind]->GetTimeLeft())
			ind = i ; 
	}
	processorsArray[ind]->AddToQueue(ptr);
}

void Scheduler::doMigrationFCFS(Process * ptr){
	MaxwCount++;
	int ind = NF+NS ; 
	for (int i =ind; i < processorsCount; i++)
	{
		if(processorsArray[i]->GetTimeLeft()<processorsArray[ind]->GetTimeLeft())
			ind = i ; 
	}
	processorsArray[ind]->AddToQueue(ptr);
}