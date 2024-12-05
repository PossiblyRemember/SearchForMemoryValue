#include "memoryMan.hpp"

using namespace std;

namespace PRUtils {
	namespace memory {
		std::vector<MODULEENTRY32> GetModules(unsigned long PID) {
			unsigned long cPID = GetCurrentProcessId();
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
			if (snapshot == INVALID_HANDLE_VALUE) {
				cerr << "Invalid, exiting.";
				return vector<MODULEENTRY32>();
			} else if(PID == cPID){
				cerr << "Invalid; is current process.";
				return vector<MODULEENTRY32>();
			}
			printf("%i\n%i\n",cPID,PID);
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(PROCESSENTRY32W);
			if (!Module32First(snapshot, &modEntry))
			{
				cerr << "P32F"; // show cause of failure
				CloseHandle(snapshot);          // clean the snapshot object
				return vector<MODULEENTRY32>();
			}
			vector<MODULEENTRY32> modules;
			do {
				printf("Base address of Module %s is at address %p\n",modEntry.szModule,modEntry.modBaseAddr);
				unsigned int i = 1;
				++i;
				modules.push_back(modEntry);

			} while (Module32Next(snapshot, &modEntry));
			modules.shrink_to_fit();
			return modules;
		}

		unsigned char* SearchMemory(const char* target, HANDLE hProcess, std::vector<MODULEENTRY32>& modules) {
			// buffer
			// char buffer[2048];
			// code to read memory
			// ReadProcessMemory(process, modules[0].modBaseAddr + i, &buffer, sizeof(buffer), nullptr);

			const size_t alignment = 1;
			const size_t size = sizeof(target);
			const size_t length = strlen(target) - 1;
			unsigned char* ptr = nullptr;
			unsigned int score = 0;
			unsigned long long probIterator = 0; // initialize iterator to change string array location
			size_t count = 0;
			unsigned int modulePos = 0;
			if (modules[0].hModule == INVALID_HANDLE_VALUE || NULL) {
				std::cerr << "\nFAILED, BAD MODULE HANDLE";
			}
			for (unsigned int moduleCount = 0; moduleCount < modules.capacity();) {
				unsigned long long difference = modules[moduleCount].modBaseSize;
				for (modulePos = 0; probIterator < length && modulePos < difference; ++modulePos) {
					static char buffer = 0; // init char buffer to read to

					std::string wText = std::format("Module: {}, Iteration: {}, Probability: {}%", moduleCount, modulePos, (float)probIterator / (float)length * 100.0f);

#ifdef DEBUG
					SetConsoleTitleA(wText.c_str());
#endif

					ReadProcessMemory(hProcess, modules[moduleCount].modBaseAddr + modulePos, &buffer, sizeof(buffer), nullptr);

					if (buffer == target[probIterator]) {
						++probIterator;
					}
					else {
						probIterator = 0;
					}
					ptr = modules[moduleCount].modBaseAddr + modulePos;

#ifdef CHARDEBUG
					if (buffer == '\0') {
						std::cout << '\n';
					}
					std::cout << buffer;
					std::wcout << buffer;
#endif

				}
				++moduleCount;
			}
			printf("FOUND SOMETHING!");

			return ptr-length+1;
		}
	}
}