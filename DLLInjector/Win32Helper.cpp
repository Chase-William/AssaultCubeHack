#include "Win32Helper.h"

std::string toLower(const char* str) {
	char newStr[128]{};
	for (int i = 0; i < strlen(str); i++) {
		newStr[i] = std::tolower(*(&(*str) + i * sizeof(char)));		
	}
	return std::string(newStr);
}

std::vector<std::string> Win32Helper::getProcs(const char* name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	// Contains process matches by name
	std::vector<std::string> matches;
	
	// Capture snapshot of all processes on machine
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	_bstr_t otherProcName;

	std::string nameAsString;

	if (name && name[0] != '\0') {
		nameAsString = toLower(name);
	}

	// Inspect first process if valid (copy didnt fail earlier)
	if (Process32First(snapshot, &entry)) {
		while (Process32Next(snapshot, &entry)) {
			// Compare strings and also convert the BSTR (binary string to const char* for comparison)
			/*if (!_stricmp(_bstr_t(entry.szExeFile), name.c_str())) {
				CloseHandle(snapshot);
				return entry.th32ProcessID;				
			}*/
			// If the process name given is less than the other, add it to the list of possible matches
			otherProcName = _bstr_t(entry.szExeFile);
			if (name && name[0] != '\0') {	
				auto test = toLower(std::string(otherProcName).c_str());
				if (toLower(otherProcName).contains(nameAsString)) {
					matches.push_back(std::string(otherProcName));
				}
			}
			else {
				matches.push_back(std::string(otherProcName));
			}
		}
	}

	CloseHandle(snapshot);
	return matches;
}

PROCESSENTRY32 Win32Helper::getProcAddr(const std::string const name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	// Contains process matches by name
	std::vector<std::string> matches;

	// Capture snapshot of all processes on machine
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	_bstr_t otherProcName;

	// Inspect first process if valid (copy didnt fail earlier)
	if (Process32First(snapshot, &entry)) {
		while (Process32Next(snapshot, &entry)) {
			// Compare strings and also convert the BSTR (binary string to const char* for comparison)
			if (!_stricmp(_bstr_t(entry.szExeFile), name.c_str())) {
				CloseHandle(snapshot);
				return entry;
			}
		}
	}

	CloseHandle(snapshot);
	return PROCESSENTRY32{};
}
