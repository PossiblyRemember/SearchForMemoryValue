#pragma once
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <windows.h>
#include <psapi.h>
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

DWORD_PTR GetProcessBaseAddress(HANDLE hProcess) {
	DWORD_PTR baseAddress = 0;
	if (hProcess) {
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded,NULL)) {
			baseAddress = (DWORD_PTR)hMod;
		}
		else {
			std::cerr << "PROCESS MODULE FAILED" << std::endl;
		}
	}
	else {
		std::cerr << "OPEN PROCESS FAILED" << std::endl;
	}
	return baseAddress;
}

//template <typename T>
//T searchMemory(size_t size, size_t alignment, T target, int range, int loops) {
//	void* memoryBlock = malloc(size + alignment - 1);
//	size_t runs;
//	bool caught_target = false;
//	uintptr_t alignedAddress = ((uintptr_t)&memoryBlock + alignment - 1) & ~(alignment - 1);
//	for (int i = 0; i < loops; i++) {
//		cout << "Looping\n" << i % 2 << endl;
//		if (i % 2 == 0) {
//			runs = 0;
//			for (size_t offset = 0; !caught_target && runs < range; offset += alignment, runs++) {
//				uintptr_t offsetAddress = alignedAddress + offset;
//				T* dataAddress = reinterpret_cast<T*>(offsetAddress);
//				__try {
//					if (*dataAddress == target) {
//						caught_target = true;
//						cout << "CAUGHT\n";
//						return *dataAddress;
//					}
//					else {
//						cout << "Rerun a " << endl << (void*)offsetAddress << endl << dataAddress << endl;
//					}
//				}
//				__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
//					cerr << "RUNTIME ERROR";
//				}
//			}
//		}
//		else {
//			runs = 0;
//			for (size_t offset = 0; !caught_target && runs < range; offset -= alignment, runs++) {
//				uintptr_t offsetAddress = alignedAddress + offset;
//				T* dataAddress = reinterpret_cast<T*>(offsetAddress);
//				__try {
//					if (*dataAddress == target) {
//						caught_target = true;
//						cout << "CAUGHT\n";
//						return *dataAddress;
//					}
//					else {
//						cout << "Rerun b " << endl << (void*)offsetAddress << endl << dataAddress << endl;
//					}
//				}
//				__except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
//					cerr << "RUNTIME ERROR";
//				}
//			}
//		}
//	}
//}

template <typename T>
T searchMemory(HANDLE hProcess, DWORD_PTR baseAddress, size_t alignment, T target, int range, int loops) {
    size_t runs;
    bool caught_target = false;
    uintptr_t alignedAddress = baseAddress + alignment - 1;
    alignedAddress &= ~(alignment - 1);

    for (int i = 0; i < loops; i++) {
        std::cout << "Looping " << i % 2 << std::endl;
        if (i % 2 == 0) {
            runs = 0;
            for (size_t offset = 0; !caught_target && runs < range; offset += alignment, runs++) {
                uintptr_t offsetAddress = alignedAddress + offset;
                T buffer;
                if (ReadProcessMemory(hProcess, (LPCVOID)offsetAddress, &buffer, sizeof(T), NULL)) {
                    if (buffer == target) {
                        caught_target = true;
                        std::cout << "CAUGHT" << std::endl;
                        return buffer;
                    }
                    else {
                        std::cout << "Rerun a " << std::endl;
                    }
                }
                else {
                    std::cerr << "FAILED AT " << (void*)offsetAddress << std::endl;
                }
            }
        }
        else {
            runs = 0;
            for (size_t offset = 0; !caught_target && runs < range; offset -= alignment, runs++) {
                uintptr_t offsetAddress = alignedAddress + offset;
                T buffer;
                if (ReadProcessMemory(hProcess, (LPCVOID)offsetAddress, &buffer, sizeof(T), NULL)) {
                    if (buffer == target) {
                        caught_target = true;
                        std::cout << "CAUGHT" << std::endl;
                        return buffer;
                    }
                    else {
                        std::cout << "Rerun b " << std::endl;
                    }
                }
                else {
                    std::cerr << "FAILED AT " << (void*)offsetAddress << std::endl;
                }
            }
        }
    }

    std::cerr << "Target not found after " << loops << " loops" << std::endl;
    return T(); // Return default value of T if target not found
}

template <typename T>
T searchApplicationMemory(size_t alignment, T target, int range, int loops, DWORD PID) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (!hProcess) {
        std::cerr << "PID ERROR " << PID << std::endl;
        return T(); 
    }

    DWORD_PTR baseAddress = GetProcessBaseAddress(hProcess);
    if (!baseAddress) {
        std::cerr << "BASE ADDRESS ERROR " << PID << std::endl;
        CloseHandle(hProcess);
        return T(); // Return default value of T if base address cannot be obtained
    }

    T result = searchMemory(hProcess, baseAddress, alignment, target, range, loops);

    CloseHandle(hProcess);
    return result;
}

//template <typename T>
//T searchApplicationMemory(size_t alignment, T target, int range, int loops, DWORD PID) {
//	HANDLE hProcess = OpenProcess(PROCESS_VM_READ,FALSE,PID);
//	DWORD_PTR baseAddress = GetProcessBaseAddress(hProcess);
//	T buffer;
//	ReadProcessMemory(hProcess, (LPCVOID)(baseAddress), &buffer, sizeof(target), NULL);
//	return 0;
//}