#include <cstdlib>
#include <string>
#include <iostream>
#include "memoryMan.h"
#include "OpenGLMan.h"
#include "image_utils.h"
#include <future>
#include <TlHelp32.h>

using namespace std;
using namespace PRUtils::memory;


int main() {
	HANDLE token;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);
	LUID luid; 
	if (!LookupPrivilegeValueA(
		NULL,            // lookup privilege on local system
		"SeDebugPrivilege",   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}
	TOKEN_PRIVILEGES privvy{
		privvy.PrivilegeCount = 1,
		privvy.Privileges[0].Luid = luid,
		privvy.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED
	};
	if (!AdjustTokenPrivileges(token, false, &privvy, 0, NULL, NULL)) {
		cout << "ERROR: " << GetLastError();
	};
	HWND calculatorWindow = FindWindowA(nullptr, "Calculator");
	unsigned long ProcessID = GetProcessId(calculatorWindow);
	PRUtils::memory::GetBaseAddress(NULL,ProcessID);
	return 0;
}



