#pragma once
class IO{
private:
	int arrival,duration,timeleft;
	bool done;
public:
	IO(){
		arrival = duration = timeleft = 0;
		done =false;
	}
	IO(int arrive,int du){
		arrival = arrive;
		duration = du;
		timeleft = du;
		done = false;
	}
	int getDuration(){
		return duration;
	}
	int getArrival(){
		return arrival;
	}
	int getTimeLeft(){
		return timeleft;
	}
	bool isDone(){
		return done;
	}
	void setDuration(int x){
		if(x>0) duration=x;
	}
	void setArrival(int x){
		if(x>0) arrival=x;
	}
	void setTimeLeft(int x){
		timeleft=x;
	}
	void setDone(bool x){
		done=x;
	}
};

