#pragma once
#include "Processor.h"
#include"PriorityQueue.h"
#include <cstdlib>
#include <time.h>
class SJF :	public Processor
{

public:
	SJF(){
		ReadyQ = new PriorityQueue<Process *> ;
		run = 0 ; 
	}
	virtual int  Run(){
		if(!busy && !ReadyQ->isEmpty()){
			busy=1;
			ReadyQ->dequeue(run);	
			srand(time(0));
			int x = rand()%100 ;
			if(x>=1 && x<=15){
			busy = 0 ; 
			run = 0 ; 
			QueueTimeLeft-=run->getTimeLeft();
			return 1 ; 
			}else if(x>=20 && x<=30){
            busy = 0 ; 
			run = 0 ;
			QueueTimeLeft-=run->getTimeLeft();
			return 2 ; 
			}else if(x>=50 && x<=60 ) {
			busy = 0 ; 
			run = 0 ; 
			QueueTimeLeft-=run->getTimeLeft();
			return 3 ; 
			}
			return 0 ; 
		}
	}
	bool isBusy(){
		return busy ; 
	}
	virtual void PrintReady(){
		if(ReadyQ->isEmpty()) return;
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
		QueueADT<Process*>*  getReadyQ() {
			return ReadyQ ; 
		}
	

	~SJF(){
	
	}
};

