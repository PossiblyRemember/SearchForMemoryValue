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

//#include "image_utils.h"
#include "./include/memoryMan.h"
#define DEBUG
#define endl "\n"

using namespace std;


#pragma pack(push, 1)
struct BytedStuff {
	const char* str = "very specific string";
};
#pragma pack(pop)

int main() {
	BytedStuff newStructInit;
	std::cout << searchApplicationMemory<const char*>(1, "",5000,2,13520);

}