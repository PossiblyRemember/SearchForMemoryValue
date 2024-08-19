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
		template<typename T>
		unsigned char* SearchMemory(T target, HANDLE hProcess, std::vector<MODULEENTRY32>& modules) {

			// buffer
			// char buffer[2048];
			// code to read memory
			// ReadProcessMemory(process, modules[0].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);

			const size_t alignment = std::alignment_of_v<T>;
			const size_t size = sizeof(target);
			const size_t length = std::strlen(target)+1;
			unsigned char* ptr = nullptr;
			unsigned int score = 0;
			unsigned int place = 0;
			std::vector<char> buffer;
			std::vector<char> string;
			if (modules[0] != INVALID_HANDLE_VALUE or NULL) {
				cerr << "\nFAILED, BAD MODULE HANDLE";
				abort();
			}
			for (byte* i = modules[0].modBaseAddr; i < modules[modules.size()/sizeof(T)].modBaseAddr && score < length; ++i) {
				++place;
				ReadProcessMemory(hProcess, i, &buffer, sizeof(buffer), nullptr);
				if (buffer[place] == target[place]) {
					string.push_back(buffer[place]);
					++score;
				}
			}
			return ptr;
		}
	}
}
