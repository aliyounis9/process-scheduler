#pragma once
#include"QueueADT.h"
#include"Process.h"

class Processor{
protected:
int QueueTimeLeft;
Process * run ;
QueueADT<Process*>*ReadyQ;
bool busy;
int RDYCount;
public:
	Processor(){
	busy=false;
	QueueTimeLeft=0;
	RDYCount=0;
	}
	int virtual Run() = 0 ;
	bool virtual isBusy() = 0;
	
	int virtual GetTimeLeft(){
		return QueueTimeLeft;
	}
	Process* getrun(){
		return run;
	}
	void AddToQueue(Process*ToAdd){
		QueueTimeLeft+=ToAdd->getCPUtime();
		ReadyQ->enqueue(ToAdd);
        
	}
	void virtual PrintReadyQ() = 0;
	int virtual getRDYCount() =0 ; 
	virtual QueueADT<Process*>*  getReadyQ() = 0; 

};