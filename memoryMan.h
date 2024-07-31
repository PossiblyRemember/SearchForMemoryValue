#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <iostream>
namespace PRUtils {
	namespace memory {
		void* GetBaseAddress(DWORD dwFlags, DWORD PID);
	}
}
