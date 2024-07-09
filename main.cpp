#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <memory.h>
#include <windows.h>
#include <thread>
#include <fstream>
#include <vector>
//#include <GLFW/glfw3.h>
#include <bitset>
//#include <GL/freeglut.h>
#pragma comment(lib, "")

//#include "image_utils.h"
#include "./include/memoryMan.h"
//#define DEBUG
#define endl "\n"

using namespace std;

int test = 2;

#pragma pack(push, 1)
struct BytedStuff {
	string str = "very specific string";
};
#pragma pack(pop)

int working() {
	BytedStuff newStructInit;
	int* intPtr = &test;
	
	//DWORD PID = 30884;
	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);
	//MEMORY_BASIC_INFORMATION memInfo;
	//cout << (void*)VirtualQueryEx(hProcess, NULL, &memInfo, sizeof(memInfo)) << endl;
	//cout << memInfo.BaseAddress << endl;
	//cout << (void*)hProcess << endl;
	//system("pause");
	return -1;
}

int main() {
	int n = 2;
	asm(""); 
}