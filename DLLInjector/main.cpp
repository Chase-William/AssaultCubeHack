#include <comdef.h>
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>

char DLL_PATH[] = "C:\\Dev\\DLLInjector\\Release\\AssaultCubeHackDLL.dll";
unsigned int DLL_PATH_SIZE = sizeof(DLL_PATH) + 1;

DWORD getProc(const char* name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	// Capture snapshot of all processes on machine
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	// Inspect first process if valid (copy didnt fail earlier)
	if (Process32First(snapshot, &entry)) {
		while (Process32Next(snapshot, &entry)) {
			// Compare strings and also convert the BSTR (binary string to const char* for comparison)
			if (!_stricmp(_bstr_t(entry.szExeFile), name)) {
				CloseHandle(snapshot);
				return entry.th32ProcessID;				
			}
		}
	}

	CloseHandle(snapshot);
	return NULL;
}

int main(int argc, char* argv[]) {
	DWORD procId = getProc("ac_client.exe");
	if (!procId) {
		std::cout << "Failed to get process id.\n";
		return EXIT_SUCCESS;
	}
	HANDLE threadHandle;
	LPVOID remoteBuffer;
	
	// handle to kernel32 and pass it to GetProcAddress
	HMODULE hKernel32 = GetModuleHandleA("Kernel32");
	VOID* lb = GetProcAddress(hKernel32, "LoadLibraryA");

	// Open process handle
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	// Create remove buffer in target process
	remoteBuffer = VirtualAllocEx(processHandle, NULL, DLL_PATH_SIZE, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	// Write DLL path into remove buffer in target process
	WriteProcessMemory(processHandle, remoteBuffer, DLL_PATH, DLL_PATH_SIZE, NULL);
	// Create remove thread in target process set to call LoadLibraryA on startup to load our library
	threadHandle = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)lb, remoteBuffer, 0, NULL);
	// Close our process handle
	CloseHandle(processHandle);
	// CloseHandle(threadHandle);
	return EXIT_SUCCESS;
}