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
			bool caught = false;
			for (unsigned long i = 0; !caught; i++) {
				char buffer[1024];
				char string[1024];
				ReadProcessMemory(hProcess, modules[0].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);
				string[i] = buffer[i];
				std::printf("ALIGNMENT: %zi\nSIZE: %zi\nBUFFER: %s\n", alignment, size, string);
				if (string == target) {
					ptr = (unsigned char*)(modules[0].modBaseAddr + i);
					caught = true;
				}
			}
			return ptr;
		}
	}
}
