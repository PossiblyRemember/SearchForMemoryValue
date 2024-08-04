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
	unsigned long PID;
	cin >> PID;
	// check if process id is null
	if (PID == 0) { 
		printf("Invalid Process ID");
		return 0;
	}
	vector<MODULEENTRY32> modules = GetModules(PID);
	HANDLE process = OpenProcess(PROCESS_VM_READ, false, PID);
	if (process == INVALID_HANDLE_VALUE) {
		cerr << "Invalid OpenProcess.\n";
		return 0;
	}/*
	for (MODULEENTRY32 mod : modules) {
		printf("Module %s with an address of %p containing the following: %s\n", mod.szModule, mod.modBaseAddr, (const char*)SearchMemory(process, mod));
	}*/
	char buffer[2048];
	ReadProcessMemory(process, modules[1].modBaseAddr,&buffer,sizeof(buffer),nullptr);
	cout << buffer;

	return 0;
}



