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
#include "Possibly Remember\Memory Management\common_utils.h"

#define DEBUG
#define endl "\n"

using namespace std;


#pragma pack(push, 1)
struct BytedStuff {
	const char* str = "very specific string";
};
#pragma pack(pop)

int main() {
	BytedStuff newStructInit{};
	MemorySearch search 
	cout << "\n" << &newStructInit;

}