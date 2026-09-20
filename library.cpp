#include <cstdio>
#include <windows.h>
#include <string>
#include "proxy_exports.h"
#include "Logging.h"
#include "windows.h"
#include "MinHook.h"

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

    return GetProcAddress(g_version_module, name);
}
#pragma endregion

typedef HANDLE(WINAPI* CreateFileW_t)(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
);

CreateFileW_t fpCreateFileW = nullptr;

HANDLE WINAPI DetourCreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
) {
    if (lpFileName != nullptr) {
        const std::wstring filePath(lpFileName);

        if (filePath.find(L"data.win") != std::wstring::npos ||
            filePath.find(L".win") != std::wstring::npos) {

            const size_t lastBackslashIndex = filePath.find_last_of(L'\\');

            std::wstring redirectedPath;
            if (lastBackslashIndex != std::wstring::npos) {
                redirectedPath = filePath.substr(0, lastBackslashIndex + 1) + L"custom_data.win";
            } else {
                redirectedPath = L"custom_data.win";
            }

            const std::string logMsg(redirectedPath.begin(), redirectedPath.end());
            Logging::LogInfo("Found data.win request, redirecting to " + logMsg + ".");

            HANDLE newFile = fpCreateFileW(redirectedPath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

            if (newFile == INVALID_HANDLE_VALUE) {
                Logging::LogWarning("No modified data.win found, falling back to vinilla.");
                return fpCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
            }

            return newFile;
        }
    }

    return fpCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


bool InitializeHooks() {
    if (MH_Initialize() != MH_OK) return false;

    HMODULE hKernelBase = GetModuleHandleW(L"KERNELBASE.dll");
    if (!hKernelBase) {
        hKernelBase = GetModuleHandleW(L"kernel32.dll");
    }

    if (hKernelBase) {
        void* targetAddress = (void*)GetProcAddress(hKernelBase, "CreateFileW");

        if (targetAddress) {
            if (MH_CreateHook(targetAddress, reinterpret_cast<LPVOID>(&DetourCreateFileW), reinterpret_cast<LPVOID*>(&fpCreateFileW)) == MH_OK) {
                return MH_EnableHook(targetAddress) == MH_OK;
            }
        }
    }
    return false;
}

DWORD WINAPI InitThread(LPVOID) {
    // Keep logger on a separate thread so it doesn't cause a DllMain deadlock
    Sleep(50);
    // if (AllocConsole()) {
    //     SetConsoleTitleA("GMI Logger");
    //     freopen("CONOUT$", "w", stdout);
    //     freopen("CONOUT$", "w", stderr);
    //     freopen("CONIN$", "r", stdin);
    // }

    Logging::LogInfo("Logger attached to active runner instance.");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // We don't thread this because it can create a race condition for the hook to init before GM loads the data.win
        InitializeHooks();

        // Console can be threaded since it kept deadlocking for some reason, and it's not high priority.
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitThread, nullptr, 0, nullptr);
    }
    return TRUE;
}
