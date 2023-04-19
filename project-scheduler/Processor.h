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
	virtual void setrun(int TS)=0 ; 
	virtual int Run(Process * & ptr) = 0 ;
	virtual bool  isBusy() {
		return busy;
	}
	
	virtual int  GetTimeLeft(){
		return QueueTimeLeft;
	}
	Process* getRunning(){
		return run;
	}
	void AddToQueue(Process*ToAdd){
		QueueTimeLeft+=ToAdd->getCPUtime();
		ReadyQ->enqueue(ToAdd);
        
	}
	virtual void  PrintReadyQ() = 0;
	virtual int  getRDYCount() =0 ;
	virtual QueueADT<Process*>*  getReadyQ() = 0; 

};