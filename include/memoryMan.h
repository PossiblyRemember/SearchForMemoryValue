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
void searchMemory(size_t size, size_t alignment, T target, int range, int loops) {
	void* memoryBlock = malloc(size + alignment - 1);
	size_t runs;
	bool caught_target = false;
	uintptr_t alignedAddress = ((uintptr_t) & memoryBlock + alignment - 1) & ~(alignment - 1);
	for (int i = 0; i <= loops; i++) {
		cout << "Looping " << loops % 2;
		if (loops % 2 == 0) {
			runs = 0;
			for (size_t offset = 0; !caught_target && runs < range; offset += alignment, runs++) {
				uintptr_t offsetAddress = alignedAddress + offset;
				try {
					if (reinterpret_cast<const char*>(offsetAddress) == target) {
						caught_target = true;
						cout << "CAUGHT";
					}
					else {
						//cout << "Rerun a " << (void*)offsetAddress << endl;
					}
				}
				catch (const std::runtime_error& ex) {
					cerr << "RUNTIME ERROR";
				}
			}
		}
		else {
			runs = 0;
			for (size_t offset = 0; !caught_target && runs < range; offset -= alignment, runs++) {
				uintptr_t offsetAddress = alignedAddress + offset;
				try {
					if (reinterpret_cast<const char*>(offsetAddress) == target) {
						caught_target = true;
						cout << "CAUGHT";
					}
					else {
						//cout << "Rerun b " << (void*)offsetAddress << endl;
					}
				}
				catch (const std::runtime_error& ex) {
					cerr << "RUNTIME ERROR";
				}
			}
		}
	}
}

//template <typename T>
//class MemorySearch {
//private:
//    void* memoryBlock;
//    uintptr_t alignedAddress;
//    uintptr_t memoryBlockAddress; // Add memoryBlockAddress as a member variable
//    size_t alignment;
//
//    void initiateMemRun(size_t range, T TARGET) {
//        bool TARGET_REACHED = false;
//        void* dataAddress = nullptr;
//
//        size_t RUNS = 0;
//        for (size_t offset = 0; !TARGET_REACHED && RUNS < range; offset -= alignment, ++RUNS) {
//            dataAddress = reinterpret_cast<void*>(alignedAddress + offset);
//            std::cout << "Memory Block Address: " << reinterpret_cast<void*>(memoryBlockAddress) << std::endl;
//            std::cout << "Data Address: " << dataAddress << std::endl;
//            std::cout << "Memory Data: " << (void*)&dataAddress << endl;
//
//            try { dataAddress; }
//            catch (const std::exception& ex) { std::cerr << "Caught std::exception: " << ex.what() << std::endl; }
//            if (*reinterpret_cast<T*>(dataAddress) == TARGET) {
//                TARGET_REACHED = true;
//            }
//#ifdef DEBUG
//            else {
//                std::cout << "Rerun" << std::endl;
//            }
//#endif // DEBUG
//        }
//        if (TARGET_REACHED) {
//            std::cout << "Target found at address: " << dataAddress << std::endl;
//        }
//        else {
//            std::cout << "Target not found within the specified range" << std::endl;
//        }
//    }
//
//public:
//    MemorySearch(size_t size, size_t alignment) : alignment(alignment) {
//        memoryBlock = malloc(size + alignment - 1);
//        memoryBlockAddress = reinterpret_cast<uintptr_t>(memoryBlock);
//        alignedAddress = (memoryBlockAddress + alignment - 1) & ~(alignment - 1);
//    }
//
//    ~MemorySearch() {
//        free(memoryBlock);
//    }
//
//    void search(T TARGET, size_t range) {
//        initiateMemRun(range, TARGET);
//    }
//};
