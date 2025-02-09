#include "memoryMan.hpp"

using namespace std;

namespace PRUtils {
	namespace memory {
		namespace debug {
			std::string MemoryProtectionToString(DWORD protect) {
				switch (protect) {
				case PAGE_NOACCESS:          return "PAGE_NOACCESS";
				case PAGE_READONLY:          return "PAGE_READONLY";
				case PAGE_READWRITE:         return "PAGE_READWRITE";
				case PAGE_WRITECOPY:         return "PAGE_WRITECOPY";
				case PAGE_EXECUTE:           return "PAGE_EXECUTE";
				case PAGE_EXECUTE_READ:      return "PAGE_EXECUTE_READ";
				case PAGE_EXECUTE_READWRITE: return "PAGE_EXECUTE_READWRITE";
				case PAGE_EXECUTE_WRITECOPY: return "PAGE_EXECUTE_WRITECOPY";
				case PAGE_GUARD:             return "PAGE_GUARD";
				case PAGE_NOCACHE:           return "PAGE_NOCACHE";
				case PAGE_WRITECOMBINE:      return "PAGE_WRITECOMBINE";
				default:                     return "UNKNOWN";
				}
			}

			// Helper function to translate memory state constants to a readable string
			std::string MemoryStateToString(DWORD state) {
				switch (state) {
				case MEM_COMMIT:  return "MEM_COMMIT";
				case MEM_RESERVE: return "MEM_RESERVE";
				case MEM_FREE:    return "MEM_FREE";
				default:          return "UNKNOWN";
				}
			}

			// Helper function to translate memory type constants to a readable string
			std::string MemoryTypeToString(DWORD type) {
				switch (type) {
				case MEM_PRIVATE:   return "MEM_PRIVATE";
				case MEM_MAPPED:    return "MEM_MAPPED";
				case MEM_IMAGE:     return "MEM_IMAGE";
				default:            return "UNKNOWN";
				}
			}

			void QueryMemory(HANDLE hProcess, void* address) {
				MEMORY_BASIC_INFORMATION mbi;
				SIZE_T result = VirtualQueryEx(hProcess, address, &mbi, sizeof(mbi));
				if (result == 0) {
					std::cerr << "VirtualQueryEx failed. Error: " << GetLastError() << "\n";
					CloseHandle(hProcess);
					return;
				}

				std::cout << "\nMemory Region Information:\n";
				std::cout << "---------------------------------------------------\n";
				std::cout << "Base Address:        " << mbi.BaseAddress << "\n";
				std::cout << "Allocation Base:     " << mbi.AllocationBase << "\n";
				std::cout << "Allocation Protect:  " << MemoryProtectionToString(mbi.AllocationProtect) << "\n";
				std::cout << "Region Size:         " << mbi.RegionSize << " bytes\n";
				std::cout << "State:               " << MemoryStateToString(mbi.State) << "\n";
				std::cout << "Protect:             " << MemoryProtectionToString(mbi.Protect) << "\n";
				std::cout << "Type:                " << MemoryTypeToString(mbi.Type) << "\n";
				std::cout << "---------------------------------------------------\n";
			}
		}
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

		void* SearchMemory(HANDLE hProcess, string target) {

			std::vector<MODULEENTRY32> modules = GetModules(GetProcessId(hProcess));

			const size_t alignment = 1;
			const size_t size = sizeof(target);
			const size_t length = target.length() - 1;
			void* ptr = nullptr;
			unsigned int score = 0;
			unsigned long long probIterator = 0; // initialize iterator to change string array location
			size_t count = 0;
			if (modules[0].hModule == INVALID_HANDLE_VALUE || NULL) {
				std::cerr << "\nFAILED, BAD MODULE HANDLE";
			}
			bool shouldCountThread = true;

			unsigned int moduleCount = 0;
			unsigned int modulePos = 0;
			thread countThread([&shouldCountThread, &moduleCount, &modulePos, &probIterator, length]() {
				while (shouldCountThread) {
					std::string wText = std::format("Module: {}, Iteration: {}, Probability: {}%", moduleCount, modulePos, (float)probIterator / (float)length * 100.0f);
					SetConsoleTitleA(wText.c_str());
					this_thread::sleep_for(chrono::milliseconds(500));
				}
			});

			for (moduleCount = 0; moduleCount < modules.capacity();) {
				unsigned long long difference = modules[moduleCount].modBaseSize;
				for (modulePos = 0; probIterator < length && modulePos < difference; ++modulePos) {
					static char buffer = 0; // init char buffer to read to

					

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
#endif

				}
				++moduleCount;
			}
			if (probIterator != length) {
				printf("%sERROR: Nothing found.%s", ERRORFMT, DEFAULTFMT);
				shouldCountThread = false;
				countThread.join();	
				return nullptr;
			}

			shouldCountThread = false;
			countThread.join();

			return (void*)((long long)ptr - length + 1);
		}

		class ModifyProtection {
		public:
			MEMORY_BASIC_INFORMATION mbi;
			unsigned long NewProtection;

			ModifyProtection(HANDLE ihProcess, void* iaddress)
				: hProcess(ihProcess), address(iaddress), NewProtection(PAGE_READWRITE), oldProtection(0) {

				std::cout << "Starting\n";

				// Get memory information for the address
				if (VirtualQueryEx(hProcess, address, &mbi, sizeof(mbi)) == 0) {
					std::cerr << "VirtualQueryEx failed! " << GetLastError() << std::endl;
					return;
				}

				// Set the new protection to PAGE_READWRITE if it's not set yet
				/*if (NewProtection == 0) {
					NewProtection = mbi.Protect | PAGE_READWRITE;
					cout << mbi.Protect << " now " << NewProtection << '\n';
				}*/

				// Change memory protection
				if (!VirtualProtectEx(hProcess, mbi.BaseAddress, mbi.RegionSize, NewProtection, &oldProtection)) {
					std::cerr << "Failed to change memory protection! " << GetLastError() << std::endl;
				}
			}

			~ModifyProtection() {
				std::cout << "Destructing\n";
				// Restore the original protection on deconstruction
				if (!VirtualProtectEx(hProcess, mbi.BaseAddress, mbi.RegionSize, oldProtection, &oldProtection)) {
					std::cerr << "Failed to restore memory protection! " << GetLastError() << std::endl;
				}
			}
		private:
			HANDLE hProcess;
			void* address;
			unsigned long oldProtection;
		};

		void* ReplaceMemory(HANDLE hProcess, string target, string replacement)
		{
			void* replaceLocation = SearchMemory(hProcess, target);
			if (replaceLocation == nullptr) {
				return nullptr;
			} 

			ModifyProtection mp(hProcess, replaceLocation);

			debug::QueryMemory(hProcess, replaceLocation);


			return replaceLocation;
		}
	}
}