#pragma once
#include"QueueADT.h"
#include"process.h"

class Processor{
int QueueTimeLeft;
Process *run;
QueueADT<Process*>*ReadyQ;
bool busy;
public:
	Processor(){
		busy=false;
		QueueTimeLeft=0;
	}
	int virtual Run(Process* & done, int TS) = 0;
	void virtual setBusy(bool b) = 0;
	bool virtual isBusy() = 0;
	void virtual setTimeLeftInQueue(int t) = 0;
	int virtual GetTimeLeft(){
		int time=0;
		if(run)time+=run->getTimeLeft();// Adds the time left for the running process to the total time 
		LinkedQueue<Process*>TempQ;
		if(ReadyQ->isEmpty())return time;// If ready Queue is empty the time left for processor is the time left for the running process
		while(!ReadyQ->isEmpty()){
			Process*temp;
			ReadyQ->dequeue(temp);
			time+=temp->getTimeLeft();
			TempQ.enqueue(temp);// Enquing Processes into temp Queue then Enquing them again into the Ready Queue
		}
		while(!TempQ.isEmpty()){
			Process *x;
			TempQ.dequeue(x);
			ReadyQ->enqueue(x);
		}
	return time;
	}
	Process* getrun(){
		return run;
	}
	void AddToQueue(Process*ToAdd){
		QueueTimeLeft+=ToAdd->getCPUtime();
		ReadyQ->enqueue(ToAdd);
	}
	void virtual PrintReadyQ() = 0;
	int virtual CountReadyQ() = 0;
	virtual QueueADT<Process*>*  getReadyQ() = 0;
};