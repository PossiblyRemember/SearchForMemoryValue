#pragma once
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <windows.h>
#include <psapi.h>
using namespace std;
namespace PRUtils {
	namespace memory {
		namespace searchMemory {
			void* search(void* target, const unsigned int range, const unsigned char loops, const size_t alignment) {
				const size_t target_size = sizeof(target);
				void* root = malloc(target_size + alignment - 1);
				bool caught = false;

			}
		}
		string searchMemory(string target, unsigned int range, unsigned char loops) {
			constexpr size_t alignment = alignment_of_v<string>;
		}
		const char* searchMemory(const char* target, unsigned int range, unsigned char loops) {
			constexpr size_t alignment = alignment_of_v<const char*>;
		}

	}
}
	
	
	
	
	
//	namespace memory {
//		char binaryToChar(const char* binary_c_str) {
//			char character = 0;
//			for (unsigned int i = 0; i < 8; i++) {
//				if (binary_c_str[i] == '1') {
//					character |= 1 << (7 - i);
//				}
//			}
//			return character;
//		}
//
//		DWORD_PTR GetProcessBaseAddress(HANDLE hProcess) {
//			DWORD_PTR baseAddress = 0;
//			if (hProcess) {
//				HMODULE hMod;
//				DWORD cbNeeded;
//				if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, NULL)) {
//					baseAddress = (DWORD_PTR)hMod;
//				}
//				else {
//					std::cerr << "PROCESS MODULE FAILED" << std::endl;
//				}
//			}
//			else {
//				std::cerr << "OPEN PROCESS FAILED" << std::endl;
//			}
//			return baseAddress;
//		}
//
//		template <typename T>
//		T searchMemory(T target, int range, int loops) {
//			size_t size = sizeof(target);
//			size_t alignment = alignment_of_v<T>;
//			void* memoryBlock = malloc(size + alignment - 1);
//			size_t runs;
//			bool caught_target = false;
//			uintptr_t alignedAddress = ((uintptr_t)&memoryBlock + alignment - 1) & ~(alignment - 1);
//			for (int i = 0; i < loops; i++) {
//#ifdef DEBUG
//				cout << "Looping\n" << i % 2 << endl;
//#endif
//				if (i % 2 == 0) {
//					runs = 0;
//					for (size_t offset = 0; !caught_target && runs < range; offset += alignment, runs++) {
//						uintptr_t offsetAddress = alignedAddress + offset;
//						T* dataAddress = reinterpret_cast<T*>(offsetAddress);
//						//__try {
//						if (*dataAddress == target) {
//							caught_target = true;
//#ifdef DEBUG
//							cout << "CAUGHT: " << *dataAddress << endl;
//#endif
//							return *dataAddress;
//						}
//#ifdef DEBUG
//						else {
//							cout << "Rerun a " << endl << (void*)offsetAddress << endl << dataAddress << endl;
//						}
//#endif
//						//}
//						//__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
//						//	cerr << "RUNTIME ERROR";
//						//}
//					}
//				}
//				else {
//					runs = 0;
//					for (size_t offset = 0; !caught_target && runs < range; offset -= alignment, runs++) {
//						uintptr_t offsetAddress = alignedAddress + offset;
//						T* dataAddress = reinterpret_cast<T*>(offsetAddress);
//						//__try {
//						if (*dataAddress == target) {
//							caught_target = true;
//#ifdef DEBUG
//							cout << "CAUGHT: " << *dataAddress << endl;
//#endif
//							return *dataAddress;
//						}
//#ifdef DEBUG
//						else {
//							cout << "Rerun b " << endl << (void*)offsetAddress << endl << dataAddress << endl;
//						}
//#endif
//						//}
//						//__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
//						//	cerr << "RUNTIME ERROR";
//						//}
//					}
//				}
//				if (!caught_target) {
//					throw("SEARCH FAILED");
//				}
//			}
//		}
//
//
//		template <typename T>
//		T searchApplicationMemory(size_t alignment, T target, int range, int loops, DWORD PID) {
//			HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, PID);
//			DWORD_PTR baseAddress = GetProcessBaseAddress(hProcess);
//			T buffer;
//			ReadProcessMemory(hProcess, (LPCVOID)(baseAddress), &buffer, sizeof(target), NULL);
//			return 0;
//		}
//	}
//}