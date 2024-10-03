#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <cstdio>
#include <vector>
#include <format>
namespace PRUtils {
	namespace memory {
		std::vector<MODULEENTRY32> GetModules(unsigned long PID);
		template<typename T>
		unsigned char* SearchMemory(T target, HANDLE hProcess, std::vector<MODULEENTRY32>& modules, void* output) {
			// buffer
			// char buffer[2048];
			// code to read memory
			// ReadProcessMemory(process, modules[0].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);

			const size_t alignment = std::alignment_of_v<T>;
			const size_t size = sizeof(target);
			const size_t length = std::strlen(target)+1;
			unsigned char* ptr = nullptr;
			unsigned int score = 0;
			unsigned int probability = 0;
			size_t count = 0;
			unsigned char* tempptr = nullptr;
			std::vector<char> buffer2;
			if (modules[0].hModule == INVALID_HANDLE_VALUE or NULL) {
				std::cerr << "\nFAILED, BAD MODULE HANDLE";
			}
			for (unsigned int g = 0; g < modules.capacity() or score != length; ++g) {
				unsigned long long difference = modules[g+1].modBaseAddr - modules[g].modBaseAddr;
				for (unsigned int i = 0; score < length or i < difference; ++i) {
					unsigned int ii = 0;
					char buffer;
					unsigned int p = 0;
					if (score) {
						p = 1 / (length / score) * 100;
					}
					std::string wText = std::format<>("Module: {}, Iteration: {}, Probability: {}%", g, i, p);
					SetConsoleTitleA(wText.c_str());
					ReadProcessMemory(hProcess, modules[g].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);
#ifdef DEBUG
					if (buffer == '\0') {
						std::cout << '\n';
					}
					std::cout << buffer;
					std::wcout << buffer;
#endif
					if (buffer == target[ii]) {
						tempptr = modules[g].modBaseAddr - score;
						++score;
						++ii;
					}
					else {
						score = 0;
						ii = 0;
					}
				}
			}
			ptr = tempptr;

			return ptr;
		}
	}
}
