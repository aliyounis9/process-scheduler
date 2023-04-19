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
	virtual void setrun(int TS){
		if(!busy&&!ReadyQ->isEmpty()){
	     	ReadyQ->dequeue(run);
			busy = true;
			run->setResponseTime(TS);                                    
			run->setWaitingTime(TS-run->getArrivalTime());
		}
	}
	int Run(Process * & ptr,int T){
		if(!Time){
			busy=false;
			Process*Temp=run;
			ReadyQ->enqueue(Temp);
			run=NULL;
		}
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