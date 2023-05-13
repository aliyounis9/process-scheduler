#include <iostream>
#include"Scheduler.h"
#include <cstdlib>
#include <time.h>

using namespace std;

int main() {
	srand(time(0));

	cout << "Program ran successfuly";
	Scheduler S("input-10.txt", "output.txt");
	S.simulator();
	return 0;
}

