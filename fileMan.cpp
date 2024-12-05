#include "fileMan.h"


using namespace std;

namespace PRUtils {
	namespace programs {
		void* GetBaseAddress(DWORD dwFlags, DWORD PID) {
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,PID);
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
				cout << "Current module is " << modEntry.szModule << " with a base address of " << modEntry.modBaseAddr << "\n";

			} while (Module32Next(snapshot, &modEntry));
		}
	}
}