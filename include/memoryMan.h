#pragma once
#include <cstdlib>
#include <iostream>
#include <errno.h>
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

//template <typename T>
//void searchMemory(size_t size, size_t alignment, T target, int range, int loops) {
//	void* memoryBlock = malloc(size + alignment - 1);
//	bool caught_target = false;
//
//	uintptr_t rawAddress = &memoryBlock;
//	uintptr_t alignedAddress = (rawAddress + alignment - 1) & ~(alignment - 1);
//
//    for (int i = 0; i < loops; ++i) {
//        std::cout << "Looping " << i << std::endl;
//
//        size_t runs = 0;
//        caught_target = false;
//
//        if (i % 2 == 0) {
//            // Forward direction
//            for (size_t offset = 0; !caught_target && runs < range; offset += alignment, ++runs) {
//                uintptr_t offsetAddress = alignedAddress + offset;
//                __try {
//                    T* dataAddress = reinterpret_cast<T*>(offsetAddress);
//                    if (*dataAddress == target) {
//                        caught_target = true;
//                        std::cout << "Caught target at offset " << offset << std::endl;
//                    }
//                    else {
//                        // Uncomment for debugging
//                        std::cout << "Rerun a " << (void*)offsetAddress << std::endl;
//                    }
//                }
//                __except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
//                    std::cerr << "Caught exception "<< std::endl;
//                }
//            }
//        }
//        else {
//            // Reverse direction
//            for (size_t offset = (range - 1) * alignment; !caught_target && runs < range; offset -= alignment, ++runs) {
//                uintptr_t offsetAddress = alignedAddress + offset;
//                __try {
//                    T* dataAddress = reinterpret_cast<T*>(offsetAddress);
//                    if (*dataAddress == target) {
//                        caught_target = true;
//                        std::cout << "Caught target at offset " << offset << std::endl;
//                    }
//                    else {
//                        // Uncomment for debugging
//                        std::cout << "Rerun b " << (void*)offsetAddress << std::endl;
//                    }
//                }
//                __except (EXCEPTION_ACCESS_VIOLATION or EXCEPTION_EXECUTE_HANDLER) {
//                    std::cerr << "Caught exception " << std::endl;
//                }
//            }
//        }
//
//        if (caught_target) {
//            break; // Exit the loop if target is caught
//        }
//    }
//
//    free(memoryBlock);
//    if (!caught_target) {
//        std::cout << "Failed to find target within the specified range" << std::endl;
//    }
//}
