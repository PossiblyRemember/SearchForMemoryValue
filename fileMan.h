#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
namespace PRUtils {
	namespace programs {
		HMODULE GetBaseAddress(const char* windowName);
	}
}