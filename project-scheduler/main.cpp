#include <iostream>
#include"Scheduler.h"
#include <cstdlib>
#include <time.h>

int main() {
	srand(time(0));
	Scheduler S("input-50.txt");
	S.simulator();
	return 0;
}

