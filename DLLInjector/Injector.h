#pragma once

#include <Windows.h>

class Injector
{
public:
	static bool ClassicInject(DWORD procId, char dllPath[]);
};

