#include "Injector.h"
#include <string>

// char DLL_PATH[] = "C:\\Dev\\AssaultCubeHack\\Release\\AssaultCubeHackDLL.dll";
// std::string DLL_PATH = "C:\\Dev\\AssaultCubeHack\\Release\\AssaultCubeHackDLL.dll";
// unsigned int DLL_PATH_SIZE = sizeof(DLL_PATH) + 1;

bool Injector::ClassicInject(DWORD procId, char dllPath[])
{
	unsigned int pathSize = sizeof(char) * strlen(dllPath) + 1; // +1 for \0 null terminating string character for c-styled strings
	HANDLE threadHandle;
	LPVOID remoteBuffer;
	
	// handle to kernel32 and pass it to GetProcAddress
	HMODULE hKernel32 = GetModuleHandleA("Kernel32");
	VOID* lb = GetProcAddress(hKernel32, "LoadLibraryA");

	// Open process handle
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	if (!processHandle)
		return false;

	// Create remove buffer in target process
	remoteBuffer = VirtualAllocEx(
		processHandle, 
		NULL, 
		pathSize,
		(MEM_RESERVE | MEM_COMMIT), 
		PAGE_EXECUTE_READWRITE);

	if (!remoteBuffer) {
		return false;
	}

	// Write DLL path into remove buffer in target process
	if (!WriteProcessMemory(processHandle, remoteBuffer, dllPath, pathSize, NULL)) {
		CloseHandle(processHandle);
		return false;
	}
	// Create remove thread in target process set to call LoadLibraryA on startup to load our library
	threadHandle = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)lb, remoteBuffer, 0, NULL);
	// Close our process handle
	CloseHandle(processHandle);
	if (!threadHandle) {
		return false;
	}
	// CloseHandle(threadHandle);
	return true;
}
