#pragma once
#include<iostream>
#include"Processor.h"
#include"Process.h"
#include"LinkedQueue.h"
using namespace std ; 
class UI
{
	int TimeStep ;
public:
	UI(int x ){
		TimeStep = x ;
	}
    
	void setTimeStep(int x){
		TimeStep=x; 
	}
	int getTimeStep() const 
	{ return TimeStep ; }

	void continueprinting () {
	cout<<"PRESS ANY KEY TO MOVE TO NEXT STEP!" <<endl ;
	cin.clear() ; 
	cin.ignore(1000000000, '\n'); 
	system("CLS");
	}

	void Print(Processor ** pro ,int NF ,int NS ,int NR , LinkedQueue<Process>* BLK ,LinkedQueue<Process>*RUN ,LinkedQueue<Process>*TRM) const  // it should take apointer to Processors to print rdy lists and any way to print BLK and TRM and RUN 
	{    
		cout<<"Current Timestep:"<<TimeStep<<endl  ;
		cout<<"----------- RDY processes ------------"<<endl ; 
		// you need to edit here acorrding to the above comment
		for(int i = 0  ;i< NF ; i++ ) 
		{
			cout<<"processor "<<i+1<<" "<<"[FCFS]: "<<pro[i]->getRDYCount()<<" RDY: ";
			pro[i]->PrintReadyQ(); 

		}
		for(int i = 0  ;i< NS ; i++ ) 
		{
			cout<<"processor "<<i+1<<" "<<"[SJF ]: "<<pro[i]->getRDYCount()<<" RDY: ";
			pro[i]->PrintReadyQ(); 

		}
		for(int i = 0  ;i< NR ; i++ ) 
		{
			cout<<"processor "<<i+1<<" "<<"[RR  ]: "<<pro[i]->getRDYCount()<<" RDY: ";
			pro[i]->PrintReadyQ(); 

		}

		cout<<"----------- BLK processes ------------"<<endl ;
		cout<<BLK->getCount()<<" BLK: ";


		cout<<"----------- RUN processes ------------"<<endl ;
		// here we should add count that count number of ready processes and then iterate on the processors to print them  " 


		cout<<"----------- TRM processes ------------"<<endl ;
	//	cout<<TRM->getCount()<<" TRM: "<<TRM->Print() ; 


		
	}




	
};
