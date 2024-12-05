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
#define DEBUG_MBI

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
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	if (process == INVALID_HANDLE_VALUE) {
		cerr << "Invalid OpenProcess.\n";
		return 0;
	}

	void* address = SearchMemory(process, "DOS mode");

	// Create input stream. (I'm deciding to document more.)
	WriteProcessMemory(process,address,"BOSS RUN",8,nullptr);
	if (address != nullptr) {
		printf("Still there.\n");
		std::cout << GetLastError();
	}
	else {
		printf("No? Searching.");
		if (address != nullptr) {
			printf("Boss run!");
		}
		else {
			printf("OOPS! Something went wrong ;(");
		}
	}

#ifdef DEBUG_MBI
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T result = VirtualQueryEx(process, address, &mbi, sizeof(mbi));
	if (result == 0) {
		std::cerr << "VirtualQueryEx failed. Error: " << GetLastError() << "\n";
		CloseHandle(process);
		return 1;
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
#endif

	CloseHandle(process);



	return 0;
}



