#include "fileMan.h"

using namespace std;

namespace PRUtils {
	namespace programs {
		HMODULE GetBaseAddress(const char* windowName) {
			HMODULE hMods[1024];
			PVOID hProcess = FindWindowA(nullptr, windowName);
			char read[256];
			GetWindowTextA((HWND)hProcess, read, sizeof(read));
			cout << read << endl;
			DWORD cbNeeded;
			if (K32EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
				for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
					cout << i << endl;
					char szModName[MAX_PATH];
					if (K32GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(char)))
					{
						string strModName = szModName;
						string strModContain = windowName;
						if (strModName.find(strModContain) != string::npos)
						{
							return hMods[i];
						}
					}
				}
			}
			return NULL;
		}
	}
}