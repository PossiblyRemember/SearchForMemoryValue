#include "memoryMan.h"

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
			return modules;
		}
		void* SearchMemory(HANDLE hProcess,MODULEENTRY32 module) {
			{
				void* buffer;
				ReadProcessMemory(hProcess,module.modBaseAddr,&buffer,module.modBaseSize,NULL);
				return buffer;
			}
		}
	}
}