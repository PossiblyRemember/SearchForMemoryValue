#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <cstdio>
#include <vector>
namespace PRUtils {
	namespace memory {
		std::vector<MODULEENTRY32> GetModules(unsigned long PID);
		char* SearchMemory(HANDLE hProcess, MODULEENTRY32 module);
	}
}
