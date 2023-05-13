#include"Scheduler.h"
#include "FCFS.h"
#include"RR.h"
#include"SJF.h"

void  Scheduler::loadInputFile() {
	if (inputFile.is_open()) { // if input file is open, scheduler will read all data
		inputFile >> NF >> NS >> NR;
		processorsCount = NF + NS + NR;


		inputFile >> RR_timeSlice >> RTF >> maxW >> STL >> forkProbability >> processessCount;


		STL = 2;

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
	loadInputFile();
	ui.setTimeStep(timeSteps);

	int currentProcessor = 0;
	while (trmList.getCount() != processessCount ) {
		timeSteps++;
		Process* tempProcess;

		// checking the new list for processess arriving at current timestamp and moving them to RDY list of processors
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
		//for (int i = 0; i < processorsCount; i++){
		//	Process* currentRunningProcess = nullptr;
		//	int prob = processorsArray[i]->Run(currentRunningProcess);
		//	if (prob == 1)
		//		blkList.enqueue(currentRunningProcess);
		//	else if (prob == 2) {
 	//			processorsArray[currentProcessor]->AddToQueue(currentRunningProcess);
		//		currentProcessor = (currentProcessor + 1) % processorsCount;
		//	}
		//	else if (prob == 3) {
		//		currentRunningProcess->setTerminateTime(timeSteps);
		//		trmList.enqueue(currentRunningProcess);
		//	}
		//}

		// generate a random number (1-100) and if this number is < 10, move the process from the BLK to RDY
		//int randomNumber = 5 ;//(rand() % 100) + 1;
		//if (randomNumber < 10) {
		//	Process* processInBlk = 0;
		//	if (!blkList.isEmpty()) {
		//		blkList.dequeue(processInBlk);
		//		processorsArray[currentProcessor]->AddToQueue(processInBlk);
		//		currentProcessor = (currentProcessor + 1) % processorsCount;
		//	}
		//}

		// generate a random ID (1-100) and if this number is in any FCFS RDY list, it will be killed
		//int randomID = (rand() % 100) + 1;
		//Process* toKill = nullptr;
		//for (int i = 0; i < NF && !toKill; i++) {
		//	if (processorsArray[i]->exist(randomID, toKill)) {
		//		toKill->setTerminateTime(timeSteps);
		//		trmList.enqueue(toKill);
		//	}
		//}

		ui.setTimeStep(timeSteps);
		//if(timeSteps %5 == 0) ui.Print(processorsArray, processessCount, NF, NS, NR, &blkList, &trmList);
		
		for(int i = 0; i < NF; i++){
			processorsArray[i]->checkFork(forkProbability, this);
		}

		pair<int, int> toKillProcess;
		if(sigKillTimes.peek(toKillProcess)){
			if(toKillProcess.first == timeSteps){
				sigKillTimes.dequeue(toKillProcess);
				killProcess(toKillProcess.second);
			}
		}

		if(timeSteps%STL == 0){
			doWorkStealing();
		}

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
	if(~Lindex && ~Sindex && LQF+0ll-SQF > 40ll*LQF/100){
		cout<<"HERE WE GO "<<Lindex<<" "<<Sindex;
		Process * Temp = nullptr ; 
		processorsArray[Lindex]->getReadyQ()->dequeue(Temp);
		if(Temp) workStealCount++;
		if(Temp) processorsArray[Sindex]->AddToQueue(Temp);
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
			if(!toKill) toKill = processorsArray[i]->getrun();
		}
		if(found && !toKill->getIsKilled()){
			Process *firstChild = toKill->getFirstChild();
			Process *secondChild = toKill->getSecondChild();
			cout<<"will kill "<<id<<" "; 
			if(firstChild) cout<<"child "<<firstChild->getID()<<" ";
			if(secondChild) cout<<"child "<<secondChild->getID()<<" ";
			cout<<"\n";

			if(firstChild) killProcess(firstChild->getID()); 
			if(secondChild) killProcess(secondChild->getID());
			ToTRM(toKill);
			killedCount++;
			cout<<"\n";
			toKill = nullptr;
		}
	}
}

void Scheduler :: ToTRM(Process * ptr ){
	if(ptr){
		ptr->setTerminateTime(timeSteps);

		ptr->setTurnAroundTime(ptr->getTerminationTime()-ptr->getArrivalTime());
		addToTotalTRT(ptr->getTurnAroundTime());
		ptr->setWaitingTime(ptr->getTurnAroundTime()-ptr->getCPUtime());
		addToTotalWT(ptr->getWaitingTime());
		ptr->setIsKilled(true);
		trmList.enqueue(ptr);
	}
}

void Scheduler :: BLKToRDY(){
	if(!currentIo){
		blkList.dequeue(currentIo);
		if(currentIo&&currentIo->getNextIO())
		currentIo->getNextIO()->setArrival(timeSteps);
	}
	if(currentIo){
	currentIo->getNextIO()->setTimeLeft(currentIo->getNextIO()->getTimeLeft()-1);
	if(currentIo->getNextIO()->getTimeLeft()==0){
		currentIo->getNextIO()->setDone(1);
		int Processorindex = 0 ; 
			for (int i = 0; i < processessCount; i++)
			{
				if(processorsArray[i]->GetTimeLeft()<processorsArray[Processorindex]->GetTimeLeft() )
					Processorindex = i ;
			}
			processorsArray[Processorindex]->AddToQueue(currentIo);
			currentIo->popIO();
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
	ptr=nullptr;
	}
	outputFile<<"Processes: "<<processessCount<<endl;
	outputFile<<"Avg WT = "<<double(totalWT)/processessCount<<",	 Avg RT = "<<double(totalRT)/processessCount<<",		Avg TRT = "<<double(totalTRT)/processessCount<<endl;
	outputFile<<"Migration %:	RTF = "<<RTFCount/processessCount<<"%,	MaxW = "<<MaxwCount/processessCount<<"%"<<endl;
	outputFile<<"Work Steal%: "<<workStealCount/processessCount<<"%"<<endl;
	outputFile<<"Forked Process: "<<forkCount/processessCount<<"%"<<endl;
	outputFile<<"Killed Process: "<<killedCount/processessCount<<"%"<<endl<<endl;
	outputFile<<"Processors: "<<processorsCount<<" ["<<NF<<" FCFS, "<<NS<<" SJF, "<<NR<<" RR]"<<endl;
	outputFile<<"Processors Load"<<endl;
	for (int i = 0; i < processorsCount; i++)
	{
		outputFile<<"P"<<i+1<<"="<<processorsArray[i]->getBusyTime()/totalTRT<<"%,	" ;
	}
	outputFile<<endl<<endl;
	outputFile<<"Processors Utiliz"<<endl;
    for (int i = 0; i < processorsCount; i++)
	{
		outputFile<<"P"<<i+1<<"="<<processorsArray[i]->getBusyTime()/(processorsArray[i]->getBusyTime()+processorsArray[i]->getIdelTime())<<"%,	" ;
	}
	outputFile<<endl;
	calcTotalUtiliz();
	outputFile<<"Avg Utilization = "<<double(TotalUtiliz)/processorsCount<<"%"<<endl;


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