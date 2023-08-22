#pragma once

#include <Windows.h>

class HackClient
{
	DWORD moduleAddr;
	DWORD healthAddr;

public:
	HackClient();
};

// I should use IMGUI for this...
// Therefore I can allow for lazy loading of cheats..
// For instance instead of a massive bulk load
// Initial loads for cheats are done lazily when they're first activated
// Also the console is meh :P
