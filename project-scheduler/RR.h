#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
#include"Scheduler.h"
class RR:public Processor
{
int TimeSlice;
int Time;
public:
	RR(int T){
		QueueTimeLeft=0;
	    busy=false;
		ReadyQ=new LinkedQueue<Process*>;
		run=NULL;
		TimeSlice=T;
		Time=TimeSlice;
	}
	void SetTimeSlice(int T){
		TimeSlice=T;
	}
	int GetTimeSlice(){
		return TimeSlice;
	}
	virtual bool setRun(int TS){
		if(!busy&&!ReadyQ->isEmpty()){
	     	ReadyQ->dequeue(run);
			QueueTimeLeft-=run->getTimeLeft();
			busy = true;
			if(run->getResponseTime()==0)
			run->setResponseTime(TS);                                    
			run->setWaitingTime(TS-run->getArrivalTime());
			Time = TimeSlice ; 
			return 1 ; 
		}else 
		{
		busy = 0 ; 
		run =0;
		return false;
		} 
	}
	int Run(Process * & ptr){
		if(run&&Time){
				int probability = rand() % 100 + 1;
				if (probability <= 15 && probability >= 1)
				{
					// Sent to BLK
					QueueTimeLeft-=run->getTimeLeft();
					busy = false;
					ptr =run ; 
					run = nullptr;
					return 1;
				}
			    else if (probability <= 30 && probability >= 20)
				{
					// Sent to RDY
					QueueTimeLeft-=run->getTimeLeft();
					busy = false;
					ptr =run ; 
					run = nullptr;
					return 2;
				}
				else if (probability <= 60 && probability >= 50)
				{
					// Sent to TRM
					QueueTimeLeft-=run->getTimeLeft();
					busy = false;
					ptr =run ; 
					run = nullptr;
					return 3;
				}
				else {
					run->setTimeLeft(run->getTimeLeft()-1);
					Time--;
					if(Time == 0)
					{
						busy=false;
		             	Process*Temp=run;
		             	ptr=run;
			            run=NULL;
						return 1 ; 
					}else
					return 0;
					}
			}
	}
	int getRDYCount(){
			return ReadyQ->getCount() ;  
	}
	bool isBusy(){
	   return busy;
	}
	virtual QueueADT<Process*>*getReadyQ(){
		return ReadyQ;
	}

	virtual int exist(int id, Process*& ptr) {
		return 0;
	}
	virtual void PrintReadyQ() {
        if(ReadyQ->isEmpty())
    return;
        LinkedQueue<Process*>TempQ;
            Process* temp;
            ReadyQ->dequeue(temp);
            TempQ.enqueue(temp);
            cout<<temp;
        while(!ReadyQ->isEmpty()){
            Process* temp;
            ReadyQ->dequeue(temp);
            TempQ.enqueue(temp);
            cout<<", "<<temp;
        }
        while(!TempQ.isEmpty()){
            Process* temp;
            TempQ.dequeue(temp);
            ReadyQ->enqueue(temp);
        }
    }
	/////////////////////strart coding for phase 2 /////////////////////////////////////////////
	 virtual void SchedAlgo(Scheduler * sch){
		if(busy){
			if(run){
			run->setTimeLeft(run->getTimeLeft()-1);
			Time--;
			if(run->getTimeLeft()==0){
			 busy = false;
			
			 sch->ToTRM(run);
			 run = 0;
			}
			if(Time==0&&run){
			 busy = false;
			
			 AddToQueue(run);
			 run = 0;
			}}
			if(run&&run->getNextIO()){
			if(run->getNextIO()->getArrival()==run->getCPUtime()-run->getTimeLeft()){
			busy = 0 ;
		
			sch->ToBLK(run);
			run = 0 ; 
			}}
			if(run)
				busyTime++;
			else
				idleTime++;
			
		}
		else{
			if(setRun(sch->gettimestep())){
			while(run&&run->getTimeLeft()<sch->getRTF())
			{
				sch->doMigrationRR(run);
                setRun(sch->gettimestep());
			}}
			if(busy)
				busyTime++;
			else
				idleTime++;
		}
	}
	 virtual ~RR() {

	 }
};