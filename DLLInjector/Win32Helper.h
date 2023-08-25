#pragma once
#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <vector>

class Win32Helper
{
public:
	static std::vector<std::string> getProcs(const char* name = nullptr);
	static PROCESSENTRY32 getProcAddr(const std::string const name);
};

