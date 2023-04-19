#include <iostream>
#include"Scheduler.h"
#include <cstdlib>
#include <time.h>

int main() {
	srand(time(0));

	cout << "Program ran successfuly";
	Scheduler S("input.txt");
	S.simulator();
	return 0;
}

