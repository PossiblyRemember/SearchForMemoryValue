#include <cstdlib>
#include <string>
#include <iostream>
#include "memoryMan.hpp"
#include "OpenGLMan.h"
#include "image_utils.h"
#include <future>
#include <TlHelp32.h>
using namespace std;
using namespace PRUtils::memory;
int mainV() {
	// testing
	const char* stringSource = "DOS MODE";
	vector<char> buffer;
	for (unsigned int i = 0; i < strlen(stringSource); ++i) {
		buffer.push_back(stringSource[i]);
	}
	buffer.push_back('\0');
	buffer.shrink_to_fit();
	cout << buffer.capacity() << endl;
	cout << &buffer[0];
	return 0;
}




int main() {
	istream& input = cin;
	HANDLE token;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);
	LUID luid; 
	if (!LookupPrivilegeValueA(
		nullptr,            // lookup privilege on local system
		"SeDebugPrivilege",   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}
	TOKEN_PRIVILEGES new_privileges{
		new_privileges.PrivilegeCount = 1,
		new_privileges.Privileges[0].Luid = luid,
		new_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED
	};
	if (!AdjustTokenPrivileges(token, false, &new_privileges, 0, nullptr, nullptr)) {
		cout << "ERROR: " << GetLastError();
	};
	unsigned long PID;
	input >> PID;
	// check if process id is null
	if (PID == 0) { 
		printf("Invalid Process ID");
		return 0;
	}
	vector<MODULEENTRY32> modules = GetModules(PID);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	if (process == INVALID_HANDLE_VALUE) {
		cerr << "Invalid OpenProcess.\n";
		return 0;
	}/*
	for (MODULEENTRY32 mod : modules) {
		printf("Module %s with an address of %p containing the following: %s\n", mod.szModule, mod.modBaseAddr, (const char*)SearchMemory(process, mod));
	}*/

	// Create input stream. (I'm deciding to document more.)
	char* data = new char[2048];
	WriteProcessMemory(process,SearchMemory("DOS mode", process, modules,&data),"BOSS RUN",8,nullptr);
	cout << data;
	delete[] data;
	return 0;
}



