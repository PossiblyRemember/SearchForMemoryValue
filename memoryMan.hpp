#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <cstdio>
#include <vector>
#include <format>
#define DEBUG
#define CHARDEBUG

namespace PRUtils {
	namespace memory {
		std::vector<MODULEENTRY32> GetModules(unsigned long PID);

		unsigned char* SearchMemory(const char* target, HANDLE hProcess, std::vector<MODULEENTRY32>& modules, void* output);
	}
}
