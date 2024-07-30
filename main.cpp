#include <cstdlib>
#include <string>
#include <iostream>
#include "memoryMan.h"
#include "OpenGLMan.h"
#include "image_utils.h"
#include <future>

using namespace std;
using namespace PRUtils::memory;
using namespace PRUtils::programs;


int main() {
	TOKEN_PRIVILEGES privvy{
		privvy.Privileges::
	};
	AdjustTokenPrivileges(GetCurrentProcess(),false,,0,NULL,NULL);
	//thread th00(mcp);
	//Sleep(10000);
	/*const char* in = searchMemory<const char*>("Colored Box", "test01", 5000, 2);
	const char** ptr = &in;*/
	//cout << GetBaseAddress("Terminal");
	
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS,0, FindWindowA(nullptr, "Calculator"));
	GetBaseAddress();


	return 0;
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
