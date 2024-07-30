#include "fileMan.h"

using namespace std;

namespace PRUtils {
	namespace programs {
		void* GetBaseAddress(HANDLE hProcess) {
			LPSTR processName;
			K32GetProcessImageFileNameA(hProcess,processName,sizeof(processName));
			HMODULE moduleList[256];
			LPDWORD cbneeded;
			K32EnumProcessModules(hProcess,moduleList,sizeof(moduleList),cbneeded);
			LPSTR filename;
			int i;
			for (i = 0; i < sizeof(moduleList) - *cbneeded; i++) {
				K32GetModuleFileNameExA(hProcess,moduleList[i],filename,sizeof(filename));
				if (processName == filename) {
					cout << "HIT!!";
					break;
				}
			}
			if (moduleList[i] != NULL) {
				MODULEINFO moduleInfo;
				K32GetModuleInformation(hProcess,moduleList[i],&moduleInfo,sizeof(moduleInfo));
				return moduleInfo.EntryPoint;
			}
			else {
				return NULL;
			}
		}
	}
}