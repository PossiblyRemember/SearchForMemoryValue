#pragma once
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <windows.h>
#include <psapi.h>
#include "fileman.h"
using namespace std;
//namespace PRUtils {
//	namespace memory {
//		namespace searchMemory {
//			void* search(void* target, const unsigned int range, const unsigned char loops, const size_t alignment) {
//				const size_t target_size = sizeof(target);
//				void* root = malloc(target_size + alignment - 1);
//				bool caught = false;
//				 
//
//			}
//		}
//		string searchMemory(string target, unsigned int range, unsigned char loops) {
//			constexpr size_t alignment = alignment_of_v<string>;
//		}
//		const char* searchMemory(const char* target, unsigned int range, unsigned char loops) {
//			constexpr size_t alignment = alignment_of_v<const char*>;
//		}
//
//	}
//}
	
	
	
	
namespace PRUtils {
	namespace memory {
		/*char binaryToChar(const char* binary_c_str) {
			char character = 0;
			for (unsigned int i = 0; i < 8; i++) {
				if (binary_c_str[i] == '1') {
					character |= 1 << (7 - i);
				}
			}
			return character;
		}*/

		template <typename T>
		T& searchMemory(const char* name, T target, int range, int loops) {
			HWND hwnd = FindWindowA(NULL, name);
			if (!hwnd) {
				throw runtime_error("Window not found!");
			}
			DWORD pid;
			GetWindowThreadProcessId(hwnd, &pid);
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
			if (!hProcess) {
				throw runtime_error("Thread not found!");
			}
			PRUtils::programs::GetBaseAddress(hwnd);

		}
	}
}