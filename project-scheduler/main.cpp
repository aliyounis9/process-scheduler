#include <iostream>
#include"Scheduler.h"
#include <cstdlib>
#include <time.h>
#include <Windows.h>

using namespace std;

int main() {
	srand(time(0));
	cout << "Please enter your mode of operation: " << "\n" << "Press 1 for Interactive Mode, 2 for Step-By-Step Mode, or 3 for Silent Mode.\n";
	int mode;

	cin >> mode;
	cout << "Program will run in " << (mode == 1 ? "Interactive" : mode == 2 ? "Step-By-Step" : "Silent") << " mode" << "\n";
	Sleep(1000);

	Scheduler S("input-200.txt", "output.txt", mode);
	S.simulator();
	return 0;
}

