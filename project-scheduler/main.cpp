#include <iostream>
#include"Scheduler.h"

int main() {
	cout << "Program ran successfuly";
	Scheduler S("input.txt");
	S.simulator();
	return 0;
}

