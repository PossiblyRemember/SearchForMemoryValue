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
			size_t count = 0;
			std::vector<char> buffer2;
			if (modules[0].hModule == INVALID_HANDLE_VALUE or NULL) {
				std::cerr << "\nFAILED, BAD MODULE HANDLE";
			}
			for (unsigned int g = 0; g < modules.capacity(); ++g) {
				unsigned long long difference = modules[g].modBaseAddr - modules[g + 1].modBaseAddr;
				for (unsigned int i = 0; score < length or i < difference; ++i) {
					char buffer[2048];
					ReadProcessMemory(hProcess, modules[g].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);
					if (buffer != " " or "\n" or "\0") {
						printf("module: %s\nbuffer value: %s", modules[g].szModule, buffer);
					}
				}
			}
			return ptr;
		}
	}
}
