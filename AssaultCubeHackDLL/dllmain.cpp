#include <Windows.h>
#include "pch.h"
#include <iostream>

void CreateConsole()
{
    if (!AllocConsole()) {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    // std::wcout, std::wclog, std::wcerr, std::wcin
    HANDLE hConOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();
}

static HANDLE procAddr;
static unsigned long long moduleAddr;
static unsigned long long healthAddr;

DWORD WINAPI HookThread(LPVOID param) {
    while (true) {
        if (GetAsyncKeyState(VK_F5)) {
            // MessageBoxA(NULL, "Hotkey F5 Clicked!", "Hotkey Clicked", MB_OK);
            int newHealth = 1000;
            WriteProcessMemory(procAddr, (void*)healthAddr, &newHealth, sizeof(newHealth), NULL);
            std::cout << "Health set to 1000.\n";
        }
        Sleep(100);
    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        // MessageBoxA(NULL, "Hello From", "Within Side!", MB_OK);
        
        CreateThread(NULL, NULL, HookThread, hModule, NULL, NULL);

        /*if (!AllocConsole()) {
            MessageBoxA(NULL, "Error creating console.", "Error!", MB_OK);
        }*/
        
        CreateConsole();

        std::cout << "Testing 123!\n";

        // Finish setting up the console so we can few addresses
        // I am unsure if we need to take the base + whatever and dereference that and then add the offset EC to it the address

        // std::cout << "Hello!\n";

        procAddr = GetCurrentProcess();
        moduleAddr = (unsigned long long)GetModuleHandleA("ac_client.exe");
        // unsigned long long healthPtrBase = moduleAddr + 0x17E0A8;
        unsigned long long healthPtr1;
        unsigned long long healthPtr2;
        unsigned int health;

        // std::cout << "Module Address: " << moduleAddr << "\n";

        if (!ReadProcessMemory(procAddr, (void*)(moduleAddr + 0x18B0B8), &healthPtr1, sizeof(healthPtr1), NULL)) {
            MessageBoxA(NULL, "Failed to read dereference from static address to pointer 1.", "Error!", MB_OK);
            CloseHandle(procAddr);
            return EXIT_SUCCESS;
        }

        // std::cout << "healthPtr1 Address: " << healthPtr1 << "\n";

        if (!ReadProcessMemory(procAddr, (void*)healthPtr1, &healthPtr2, sizeof(healthPtr2), NULL)) {
            MessageBoxA(NULL, "Failed to read dereferenced value from ptr 1 to ptr 2.", "Error!", MB_OK);
            CloseHandle(procAddr);
            return EXIT_SUCCESS;
        }

        // std::cout << "healthPtr2 Address: " << healthPtr2 << "\n";

        healthAddr = healthPtr2 + 0x408;

        if (!ReadProcessMemory(procAddr, (void*)healthAddr, &health, sizeof(health), NULL)) {
            MessageBoxA(NULL, "Failed to read dereferenced value from ptr + offset for health.", "Error!", MB_OK);
            CloseHandle(procAddr);
            return EXIT_SUCCESS;
        }

        std::cout << "Current Player Health: " << health << "\n";
        std::cout << "-- Press F5 to set health to 1000. --\n";
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        CloseHandle(procAddr);
        break;
    }
    return TRUE;
}

