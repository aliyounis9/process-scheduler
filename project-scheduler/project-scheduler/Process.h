#pragma once
#include "LinkedQueue.h"
#include "IO.h"
#include<iostream>
using namespace std;
class Process
{
	int ID;
	int AT;
	int RT;
	int CT;
	int TimeDone;
	int TT;
	int TurnAroundTime;
	int WaitingTime;
	int IO_n;
	int timeLeft;
	LinkedQueue <IO> IOs;
	IO* nextIO;
public:
Process(int at=0, int id=0, int ct=0, int IOn=0) {
		AT=at;
		ID=id;
		CT=ct;
	IO_n=IOn;
	timeLeft=ct;
		TimeDone = 0;
		nextIO = nullptr;
	}

void setID(int x){
ID=x;
}
void setArrivalTime(int x){
AT=x;
}
void setResponseTime(int x){
RT=x;
}
void setCPUtime(int x){
CT=x;
}
void setTimeDone(int x){
	TimeDone=x;
}
void setTerminateTime(int x){
TT=x;
}
void setTurnAroundTime(int x){
	TurnAroundTime=x;
}
void setWaitingTime(int x){
	WaitingTime=x;
}
void setIONumber(int x){
IO_n=x;
}
void setTimeLeft(int x){
	timeLeft=x;
}
void SetNextIO(IO* io){
	nextIO=io;
}
int getID(){
return ID;
}
int getArrivalTime(){
return AT;
}
int getResponseTime(){
return RT;
}
int getCPUtime(){
return CT;
}
int getTimeDone(){
	return TimeDone;
}
int getTerminationTime(){
return TT;
}
int getTurnAroundTime(){
	return TurnAroundTime;
}
int getWaitingTime(){
	return WaitingTime;
}
int getIO_n(){
	return IO_n;
}
int getTimeLeft(){
	return timeLeft;
}
IO* getNextIO(){
	return nextIO;
}
bool operator >(Process x){
	return CT>x.CT;
}
void AddIO(IO &io){
	IOs.enqueue(io);
}
void popIO()
	{
		IO ret;
		if (IOs.isEmpty())
			return;
		IOs.dequeue(ret);
		if (ret.isDone())
			return;
		IO back = ret;
		nextIO = new IO(ret);
	
		back.setDone(true);
		IOs.enqueue(back);
		return;
}
void Print() {
		cout << "Arival: " << AT << ", PID: " << ID << ", CPU Time: " << CT << ", IOs: " << IO_n<<endl;
	}
	friend ostream& operator << (ostream& out, Process* ptr)
	{
		
		out << ptr->ID;
		return out;
		
	}
};