#pragma once
#include "common.hpp"
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <TlHelp32.h>
#include <cstdio>
#include <vector>
#include <format>
#include <string>
#include <iostream>
#include <thread>
#include <chrono> 

/*
#define DEBUG
#define CHARDEBUG
*/
#define export __declspec(dllexport)
using std::string;

namespace PRUtils {
	namespace memory {
		namespace debug {
			std::string MemoryProtectionToString(DWORD protect);

			// Helper function to translate memory state constants to a readable string
			std::string MemoryStateToString(DWORD state);

			// Helper function to translate memory type constants to a readable string
			std::string MemoryTypeToString(DWORD type);

			void QueryMemory(HANDLE hProcess, void* address);
		}
		std::vector<MODULEENTRY32> GetModules(unsigned long PID);

		std::vector<void*> SearchMemory(HANDLE hProcess, string target);

		std::vector<void*> ReplaceMemory(HANDLE hProcess, string target, string replacement);
	}
}
