#pragma once
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <TlHelp32.h>
#include <windows.h>
using namespace std;

char binaryToChar(const char* binary_c_str) {
	char character = 0;
	for (unsigned int i = 0; i < 8; i++) {
		if (binary_c_str[i] == '1') {
			character |= 1 << (7 - i);
		}
	}
	return character;
}

DWORD_PTR GetProcessBaseAddress(DWORD processId) {
	DWORD_PTR baseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);
		if (Module32First(hSnapshot, &moduleEntry)) {
			baseAddress = (DWORD_PTR)moduleEntry.modBaseAddr;
		}
		CloseHandle(hSnapshot);
	}
	return baseAddress;
}

template <typename T>
T searchMemory(size_t size, size_t alignment, T target, int range, int loops) {
	void* memoryBlock = malloc(size + alignment - 1);
	size_t runs;
	bool caught_target = false;
	uintptr_t alignedAddress = ((uintptr_t)&memoryBlock + alignment - 1) & ~(alignment - 1);
	for (int i = 0; i < loops; i++) {
		cout << "Looping\n" << i % 2 << endl;
		if (i % 2 == 0) {
			runs = 0;
			for (size_t offset = 0; !caught_target && runs < range; offset += alignment, runs++) {
				uintptr_t offsetAddress = alignedAddress + offset;
				T* dataAddress = reinterpret_cast<T*>(offsetAddress);
				__try {
					if (*dataAddress == target) {
						caught_target = true;
						cout << "CAUGHT\n";
						return *dataAddress;
					}
					else {
						cout << "Rerun a " << endl << (void*)offsetAddress << endl << dataAddress << endl;
					}
				}
				__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
					cerr << "RUNTIME ERROR";
				}
			}
		}
		else {
			runs = 0;
			for (size_t offset = 0; !caught_target && runs < range; offset -= alignment, runs++) {
				uintptr_t offsetAddress = alignedAddress + offset;
				T* dataAddress = reinterpret_cast<T*>(offsetAddress);
				__try {
					if (*dataAddress == target) {
						caught_target = true;
						cout << "CAUGHT\n";
						return *dataAddress;
					}
					else {
						cout << "Rerun b " << endl << (void*)offsetAddress << endl << dataAddress << endl;
					}
				}
				__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
					cerr << "RUNTIME ERROR";
				}
			}
		}
	}
}

template <typename T>
T searchApplicationMemory(size_t size, size_t alignment, T target, int range, int loops, DWORD PID) {
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ,FALSE,PID);
	cout << GetProcessBaseAddress(PID);
	T buffer;
	ReadProcessMemory(hProcess,GetProcessBaseAddress(PID), &buffer, sizeof(target), NULL)
	return "return";
	/*for (int i = 0; i < loops; i++) {
		cout << "Looping\n" << i % 2 << endl;
		if (i % 2 == 0) {
			runs = 0;
			for (size_t offset = 0; !caught_target && runs < range; offset += alignment, runs++) {
				uintptr_t offsetAddress = alignedAddress + offset;
				T* dataAddress = reinterpret_cast<T*>(offsetAddress);
				__try {
					if (*dataAddress == target) {
						caught_target = true;
						cout << "CAUGHT\n";
						return *dataAddress;
					}
					else {
						cout << "Rerun a " << endl << (void*)offsetAddress << endl << dataAddress << endl;
					}
				}
				__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
					cerr << "RUNTIME ERROR";
				}
			}
		}
		else {
			runs = 0;
			for (size_t offset = 0; !caught_target && runs < range; offset -= alignment, runs++) {
				uintptr_t offsetAddress = alignedAddress + offset;
				T* dataAddress = reinterpret_cast<T*>(offsetAddress);
				__try {
					if (*dataAddress == target) {
						caught_target = true;
						cout << "CAUGHT\n";
						return *dataAddress;
					}
					else {
						cout << "Rerun b " << endl << (void*)offsetAddress << endl << dataAddress << endl;
					}
				}
				__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
					cerr << "RUNTIME ERROR";
				}
			}
		}
	}*/
}