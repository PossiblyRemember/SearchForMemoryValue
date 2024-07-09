#include <cstdlib>
#include <string>
#include <iostream>
#include "include/memoryMan.h"
using namespace std;
using namespace PRUtils::memory;

int main(int argc, char** argv) {
	const char* target = argv[1];
	
	cout << searchMemory<const char*>(target, 5000, 2) << std::endl;
}