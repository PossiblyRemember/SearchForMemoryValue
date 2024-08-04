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
		T SearchMemory(T target, HANDLE hProcess, std::vector<MODULEENTRY32>& modules) {

			// buffer
			// char buffer[2048];
			// code to read memory
			// ReadProcessMemory(process, modules[0].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);

			const size_t alignment = std::alignment_of_v<T>;
			const size_t size = sizeof(T);
			bool caught = false;
			for (unsigned long i = 0; !caught; i++) {
				char buffer[2048];
				ReadProcessMemory(hProcess, modules[0].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);
				std::printf("ALIGNMENT: %zi\nSIZE: %zi\n", alignment, size);
				if (buffer == target) {
					caught = true;
				}
			}
			return "this is test";


		}
	}
}
