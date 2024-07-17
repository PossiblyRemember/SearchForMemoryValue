#include <cstdlib>
#include <string>
#include <iostream>
#include "memoryMan.h"
#include "OpenGLMan.h"
#include "image_utils.h"

using namespace std;
using namespace PRUtils::memory;

string code(int argc, char** argv) {
	string *v = searchMemory<string>("test01", 5000, 2);
	system(((string)"cmd /k echo ").append(searchMemory<string>("test01", 5000, 2)->c_str()).c_str());
	return *v;
}

int main(int argc, char** argv) {
	thread th01(code,argc,argv);
	mcp();

	//const char* target;
	//int range, loops;
	//if (argc == 2) {
	//	target = argv[1];
	//}
	//else {
	//	target = "this is a test message";
	//}
	//if (argc == 3) {
	//	range = atoi(argv[2]);
	//}
	//else {
	//	range = 500;
	//}
	//if (argc == 4) {
	//	loops = atoi(argv[3]);
	//}
	//else {
	//	loops = 2;
	//}
}


