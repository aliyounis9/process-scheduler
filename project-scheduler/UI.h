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
	template<class T>
	void Print(Processor ** pro ,int count ,int NF ,int NS ,int NR , LinkedQueue<Process>* BLK ,LinkedQueue<Process>*TRM) const  // it should take apointer to Processors to print rdy lists and any way to print BLK and TRM and RUN 
	{    
		cout<<"Current Timestep:"<<TimeStep<<endl  ;
		cout<<"----------- RDY processes ------------"<<endl ; 
		// you need to edit here acorrding to the above comment
		for(int i = 0  ;i< NF ; i++ ) 
		{
			cout<<"processor "<<i+1<<" "<<"[FCFS]: "<<pro[i]->getRDYCount()<<" RDY: ";
			pro[i]->PrintReadyQ(); 

		}
		for(int i = NF ;i< NS ; i++ ) 
		{
			cout<<"processor "<<i+1<<" "<<"[SJF ]: "<<pro[i]->getRDYCount()<<" RDY: ";
			pro[i]->PrintReadyQ(); 

		}
		for(int i = NS  ;i< NR ; i++ ) 
		{
			cout<<"processor "<<i+1<<" "<<"[RR  ]: "<<pro[i]->getRDYCount()<<" RDY: ";
			pro[i]->PrintReadyQ(); 

		}

		cout<<"----------- BLK processes ------------"<<endl ;
		cout<<BLK->getCount()<<" BLK:";
		if(!BLK->isEmpty())
		{
			LinkedQueue<T>*TempQ ; 
			while(!BLK->isEmpty()){
			Process * id ;
			BLK->dequeue(id);
			TempQ->enqueue(id);
			cout<<id->getID();
			if(!BLK->isEmpty())
				cout<<", ";
			}
			while(!TempQ->isEmpty())
			{
				Process * t ; 
				TempQ->dequeue(t);
				BLK->enqueue(t);
			}
		}else 
			cout<<endl;
		int c = 0 ; 
		cout<<"----------- RUN processes ------------"<<endl ;
		for (int i = 0; i < count; i++)
		{
			if(pro[i]->isBusy())
				c++;
		}
		cout<<c<<" RUN:";
		int x = 0 ;
		for (int i = 0; i < count; i++)
		{
			if(pro[i]->isBusy())
			{
				x++;
				cout<<" "<<pro[i]->getrun()->getID()<<"P"<<i+1;
				if(x!=c)
				{
					cout<<","
				}
			}
		}
		cout<<endl;
		cout<<"----------- TRM processes ------------"<<endl ;

	    cout<<TRM->getCount()<<" TRM:";
		if(!TRM->isEmpty())
		{
			LinkedQueue<T>*TempQ ; 
			while(!TRM->isEmpty()){
			Process * id ;
			TRM->dequeue(id);
			TempQ->enqueue(id);
			cout<<id->getID();
			if(!TRM->isEmpty())
				cout<<", ";
			}
			while(!TempQ->isEmpty())
			{
				Process * t ; 
				TempQ->dequeue(t);
				TRM->enqueue(t);
			}
		}else 
			cout<<endl;
        


		
	}




	
};
