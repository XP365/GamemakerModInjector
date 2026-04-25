#include <windows.h>
#include "proxy_exports.h"
#include "Logging.h"

HMODULE g_version_module = nullptr;
INIT_ONCE g_init_once = INIT_ONCE_STATIC_INIT;
INIT_ONCE g_startup_once = INIT_ONCE_STATIC_INIT;

#pragma region DLL Passthrough
BOOL CALLBACK LoadVersionModule(PINIT_ONCE, PVOID, PVOID *) {
    char system_path[MAX_PATH];
    const UINT length = GetSystemDirectoryA(system_path, MAX_PATH);
    if (length == 0 || length >= MAX_PATH - 12) {
        return FALSE;
    }

    lstrcatA(system_path, "\\version.dll");
    g_version_module = LoadLibraryA(system_path);
    return g_version_module != nullptr;
}

FARPROC LoadExport(const char *name) {
    if (!InitOnceExecuteOnce(&g_init_once, LoadVersionModule, nullptr, nullptr)) {
        return nullptr;
    }

    InitOnceExecuteOnce(&g_startup_once, StartInjectedRuntime, nullptr, nullptr);
    return GetProcAddress(g_version_module, name);
}
#pragma endregion

//Uses a thread because windows gets pissy if I do this too soon, also keeps messages from blocking the main thread so that's a plus
DWORD WINAPI InitThread(LPVOID) {
    Sleep(100);

    if (AllocConsole()) {
        SetConsoleTitleA("GMI Logger");
    }

    Logging::WriteLogLine("ModLoader injected.");
    return 0;
}

BOOL CALLBACK StartInjectedRuntime(PINIT_ONCE, PVOID, PVOID *) {
    const HANDLE thread = CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
    if (thread == nullptr) {
        Logging::WriteLogLine("Failed to start init thread.");
        return FALSE;
    }

    CloseHandle(thread);
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
    }
    return TRUE;
}
