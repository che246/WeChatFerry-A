// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"
#include <Windows.h>
#include <TlHelp32.h>

BOOL HideDll(HMODULE hModule) {
    MODULEENTRY32 moduleEntry;
    moduleEntry.dwSize = sizeof(MODULEENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    if (snapshot == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    BOOL moduleFound = Module32First(snapshot, &moduleEntry);

    while (moduleFound) {
        if (moduleEntry.hModule == hModule) {
            memset(moduleEntry.szModule, 0, sizeof(moduleEntry.szModule));
            memset(moduleEntry.szExePath, 0, sizeof(moduleEntry.szExePath));
            break;
        }
        moduleFound = Module32Next(snapshot, &moduleEntry);
    }

    CloseHandle(snapshot);
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            HideDll(hModule);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
