#include "memoryMan.h"

using namespace std;

namespace PRUtils {
	namespace memory {
		void* GetBaseAddress(DWORD dwFlags, DWORD PID) {
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
			if (snapshot == INVALID_HANDLE_VALUE) {
				cerr << "Invalid, exiting.";
				return(0);
			}
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(PROCESSENTRY32W);
			if (!Module32First(snapshot, &modEntry))
			{
				cerr << "P32F"; // show cause of failure
				CloseHandle(snapshot);          // clean the snapshot object
				return(0);
			}
			do {
				printf("Base address of Module %s is at address %p\n",modEntry.szModule,modEntry.modBaseAddr);

			} while (Module32Next(snapshot, &modEntry));
		}
	}
}