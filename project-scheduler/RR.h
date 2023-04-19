#include "Processor.h"
#include "PriorityQueue.h"
class RR:public Processor
{int TimeSlice;
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
	virtual bool setrun(int TS){
		if(!busy&&!ReadyQ->isEmpty()){
	     	ReadyQ->dequeue(run);
			busy = true;
			run->setResponseTime(TS);                                    
			run->setWaitingTime(TS-run->getArrivalTime());
			Time = TimeSlice ; 
			return 1 ; 
		}else 
			return 0 ; 
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
		             	ReadyQ->enqueue(Temp);
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

};