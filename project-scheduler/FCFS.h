#pragma once
#include"Processor.h"
#include"LinkedQueue.h"

class FCFS:public Processor
{
public:
	FCFS(){
		QueueTimeLeft =0;
		busy=false;
		run=NULL;
		ReadyQ=new LinkedQueue<Process*>;
	}
	virtual void setrun(int TS){
		if(!busy){
	     	ReadyQ->dequeue(run);
			busy = true;
			run->setResponseTime(TS);                                    
			run->setWaitingTime(TS-run->getArrivalTime());
		}
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
		}else if(!busy&&ReadyQ->isEmpty()){
		return -1 ;
		}

	}
	int exist(){
	int x=rand();
	bool found=false;
	LinkedQueue<Process*>TempQ;
	for (int i = 0; i <ReadyQ->getCount(); i++)
	{
		Process*temp;
		ReadyQ->dequeue(temp);
		TempQ.enqueue(temp);
		if (temp->getID()==x)
		{
			found=true;
			break;
		}
	}
	if(found)
		return x;
	else return 0;
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
        cout<<"\n";
    }

};