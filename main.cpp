#include <cstdlib>
#include <string>
#include <iostream>
#include "include/memoryMan.h"
using namespace std;
using namespace PRUtils::memory;

int main(int argc, char** argv) {
	const char* target;
	int range, loops;
	if (argc == 2) {
		target = argv[1];
	}
	else {
		target = "this is a test message";
	}
	if (argc == 3) {
		range = atoi(argv[2]);
	}
	else {
		range = 500;
	}
	if (argc == 4) {
		loops = atoi(argv[3]);
	}
	else {
		loops = 2;
	}

	cout << searchMemory<const char*>(target, range, loops) << endl;
	cout << *searchMemory<const char*>(target, range, loops) << endl;
	*searchMemory<const char*>(target, range, loops) = "test messagehrthjrjtyjfgthj";
	cout << searchMemory<const char*>("test messagehrthjrjtyjfgthj", range, loops) << endl;
	cout << *searchMemory<const char*>("test messagehrthjrjtyjfgthj", range, loops) << endl;
}