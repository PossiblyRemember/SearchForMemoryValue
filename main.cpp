#include <cstdlib>
#include <string>
#include <iostream>
#include "memoryMan.h"
#include "OpenGLMan.h"
#include "image_utils.h"
#include <future>

using namespace std;
using namespace PRUtils::memory;

string *out;
string searchMemMain() {
	*out = searchMemory<string>("test01", 5000, 2);
	*out = "new value";
	return *out;
}

int code() {
	thread th02(searchMemMain);
	th02.join();
	cout << &out << endl;
	system("pause");
	return 1;
}

void main(int argc, char** argv) {
	thread th01(code);
	mcp();
	th01.join();
}





//string code(int argc, char** argv) {
//	string v = searchMemory<string>("test01", 5000, 2);
//	system(((basic_string<char,char_traits<char>,allocator<char>>)"cmd /k echo test").append(v).c_str());
//	return v;
//}
//
//int main(int argc, char** argv) {
//	thread th01(code,argc,argv);
//	mcp();
//
//	//const char* target;
//	//int range, loops;
//	//if (argc == 2) {
//	//	target = argv[1];
//	//}
//	//else {
//	//	target = "this is a test message";
//	//}
//	//if (argc == 3) {
//	//	range = atoi(argv[2]);
//	//}
//	//else {
//	//	range = 500;
//	//}
//	//if (argc == 4) {
//	//	loops = atoi(argv[3]);
//	//}
//	//else {
//	//	loops = 2;
//	//}
//}
//
//
