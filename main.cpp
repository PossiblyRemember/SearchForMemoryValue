#include <cstdlib>
#include <string>
#include <iostream>
#include "memoryMan.hpp"
//#include "OpenGLMan.h"
#include "image_utils.h"
#include <future>
#include <TlHelp32.h>
#include "common.hpp"

using namespace std;
using namespace PRUtils::memory;



int main(int *argc, char **argv) {
#pragma region InitializeTokenAndHandle
	printf("Program PID: ");
	istream& input = cin;

	HANDLE token; // initialize token for the current process

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);
	LUID luid; 
	if (!LookupPrivilegeValueA(
		nullptr,            // lookup privilege on local system
		"SeDebugPrivilege",   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return 0;
	}
	TOKEN_PRIVILEGES new_privileges{
		new_privileges.PrivilegeCount = 1,
		new_privileges.Privileges[0].Luid = luid,
		new_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED
	};
	if (!AdjustTokenPrivileges(token, false, &new_privileges, 0, nullptr, nullptr)) { // change current token to be able to debug other applications. output error if fail
		printf("%sERROR: %ld%s\n", ERRORFMT, GetLastError(), DEFAULTFMT);
	};
	string iPID;
	getline(cin,iPID); // save user input as PID
	unsigned long PID = stoi(iPID);
	cout << '\n';

	// check if process id is null
	if (PID == 0) { 
		printf("%sInvalid Process ID.%s\n", ERRORFMT, DEFAULTFMT);
		return 0;
	}


	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, PID); //open process with debug perms
	if (process == INVALID_HANDLE_VALUE) {
		printf("%sInvalid OpenProcess.%s\n", ERRORFMT, DEFAULTFMT);
		return 0;
	}
#pragma endregion

	printf("Enter string to search for: ");
	string stringToSearchFor;
	getline(cin, stringToSearchFor);
	cout << stringToSearchFor.length() << '\n';

	printf("Enter string to replace it with: ");
	string stringToReplaceWith;
	getline(cin, stringToReplaceWith);
	cout << stringToReplaceWith.length() << '\n';

	if (stringToReplaceWith.length() > stringToSearchFor.length()) {
		printf("%sWARNING: Replacement string is longer than source. Potential overflow. Continue?%s (y/n): ",WARNINGFMT,DEFAULTFMT);
		input.clear();
		string answer;
		input >> answer;
		switch (answer[0]) {
		case 'y':
			break;
		case 'n':
			printf("Enter new string to replace it with: ");
			input.clear();
			input >> stringToReplaceWith;
			cout << '\n';
			break;
		default: break;
		}
	}


	
	ReplaceMemory(process, stringToSearchFor, stringToReplaceWith);

	CloseHandle(process);



	return 0;
}



