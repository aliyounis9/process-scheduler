#pragma once
#include"Processor.h"
#include"LinkedQueue.h"
#include"Scheduler.h"
#include <cstdlib>
#include <time.h>


class FCFS:public Processor
{
public:
	FCFS(){
		QueueTimeLeft =0;
		busy=false;
		run=NULL;
		ReadyQ=new LinkedQueue<Process*>;
	}
	virtual bool setRun(int TS){
		if(!busy&&!ReadyQ->isEmpty()){
	     	ReadyQ->dequeue(run);
			busy = true;
			run->setResponseTime(TS);                                    
			run->setWaitingTime(TS-run->getArrivalTime());
			return true;
		}
		return false;
	}
	virtual int Run(Process * & ptr){
		if( run ){ // if there is a process already executing
		  int prob = rand()%100 +1;
		  if( prob <= 15 && prob >= 1){// Sent to BLK
		    busy = false;
		    QueueTimeLeft-=run->getTimeLeft();
			ptr=run;
			run = NULL;
		    return 1;
		  }
		  else if( prob <= 30 && prob >= 20){// Sent to Ready
			  busy = false;
			  QueueTimeLeft-=run->getTimeLeft();
			  ptr=run ; 
			  run = NULL;
			  return 2;
		   }
		  else if( prob <= 60 && prob >= 50 ){// Sent to Terminate
		      busy = false;
			  QueueTimeLeft-=run->getTimeLeft();
			  ptr=run;
			  run = NULL;
			  return 3;
		    }
		  else 
			  return 0;
		}else{
			return -1 ; 
		}
	}
	virtual int exist(int id, Process*& ptr){
		bool found=false;
		LinkedQueue<Process*>tempQ;
		while (!ReadyQ->isEmpty()) {
			Process* temp;
			ReadyQ->dequeue(temp);
			if (temp->getID() == id) {
				found = true;
				ptr = temp;
			}
			else {
				tempQ.enqueue(temp);
			}
		}
		while(!tempQ.isEmpty()){
			Process* temp;
			tempQ.dequeue(temp);
			ReadyQ->enqueue(temp);
		}
		return found;
	}

	int getRDYCount(){
			return ReadyQ->getCount() ;  
	}
	virtual QueueADT<Process*>*getReadyQ(){
		return ReadyQ;
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

	///////////////start coding for phase 2 //////////////////////////////
	virtual void SchedAlgo(Scheduler * sch){
		if(busy){
			run->setTimeLeft(run->getTimeLeft()-1);
			run->getNextIO()->setTimeLeft(run->getNextIO()->getTimeLeft()-1);
			if(run->getNextIO()->getTimeLeft()==0){
			busy = 0 ;
			QueueTimeLeft-=run->getTimeLeft();
			sch->ToBLK(run);
			run = 0 ; 
			}
			if (run){
			if( run->getTimeLeft()==0){
			 busy = false;
			 QueueTimeLeft-=run->getTimeLeft();
			 sch->ToTRM(run);
			 run = 0;
			}}
		}
		else{
			setRun(sch->gettimestep());
		}
	}

	void checkFork(int forkProbability, Scheduler *Sh){
		if(!run) return;
		int prob = rand()%100 + 1;
		if(prob > forkProbability) return;
		Sh->doFork(run);
	}
};