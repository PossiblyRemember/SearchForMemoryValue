#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <cstdio>
#include <vector>
#include <format>
//#define DEBUG
//#define CHARDEBUG

namespace PRUtils {
	namespace memory {
		std::vector<MODULEENTRY32> GetModules(unsigned long PID);

		void* SearchMemory(HANDLE hProcess, const char* target);

		bool ReplaceMemory(HANDLE hProcess, const char* target, const char* replacement);
	}
}
