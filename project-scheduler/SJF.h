#pragma once
#include "Processor.h"
#include"PriorityQueue.h"
#include"Scheduler.h"
#include <cstdlib>
#include <time.h>

class SJF : public Processor
{

public:
	SJF() {
		ReadyQ = new PriorityQueue<Process*>;
		run = 0;
	}
	virtual bool setRun(int TS) {
		if (!busy && !ReadyQ->isEmpty()) {
			ReadyQ->dequeue(run);
			busy = true;
			run->setResponseTime(TS);
			run->setWaitingTime(TS - run->getArrivalTime());
			return 1;
		}
		else
			return 0;
	}
	virtual int  Run(Process*& ptr) {
		if (run) {
			int x = rand() % 100 + 1;
			if (x >= 1 && x <= 15) {
				busy = 0;
				QueueTimeLeft -= run->getTimeLeft();
				ptr = run;
				run = 0;
				return 1;

			}
			else if (x >= 20 && x <= 30) {
				busy = 0;
				ptr = run;
				QueueTimeLeft -= run->getTimeLeft();
				run = 0;
				return 2;
			}
			else if (x >= 50 && x <= 60) {
				busy = 0;
				ptr = run;
				QueueTimeLeft -= run->getTimeLeft();
				run = 0;
				return 3;
			}
			return 0;
		}
		else {
			return -1;
		}
	}
	virtual void PrintReadyQ() {
		if (ReadyQ->isEmpty()) return;
		LinkedQueue<Process*>TempQ;
		Process* temp;
		ReadyQ->dequeue(temp);
		TempQ.enqueue(temp);
		cout << temp;
		while (!ReadyQ->isEmpty()) {
			Process* temp;
			ReadyQ->dequeue(temp);
			TempQ.enqueue(temp);
			cout << ", " << temp;
		}
		while (!TempQ.isEmpty()) {
			Process* temp;
			TempQ.dequeue(temp);
			ReadyQ->enqueue(temp);
		}
	}
	int getRDYCount() {
		return ReadyQ->getCount();
	}
	QueueADT<Process*>* getReadyQ() {
		return ReadyQ;
	}

	virtual int exist(int id, Process*& ptr) {
		return 0;
	}

	~SJF(){
	
	}
	/////////////////////strart coding for phase 2 /////////////////////////////////////////////

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
			if(run){
			if(run->getTimeLeft()==0){
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
};

