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
        std::wstring filePath(lpFileName);

        // Hardcoded to deltarune rn, rewrite all of this.
        if (filePath.find(L"data.win") != std::wstring::npos ||
            filePath.find(L"DATA.WIN") != std::wstring::npos ||
            filePath.find(L".win") != std::wstring::npos ||
            filePath.find(L".WIN") != std::wstring::npos) {

            wchar_t baseExePath[MAX_PATH];
            if (GetModuleFileNameW(nullptr, baseExePath, MAX_PATH) != 0) {
                std::wstring rootDir(baseExePath);
                size_t exeSlash = rootDir.find_last_of(L"\\/");
                if (exeSlash != std::wstring::npos) {
                    rootDir = rootDir.substr(0, exeSlash + 1);

                    std::wstring relativeSubfolder = L"";

                    size_t reqLastSlash = filePath.find_last_of(L"\\/");
                    if (reqLastSlash != std::wstring::npos) {
                        size_t rootTokenPos = filePath.find(L"DELTARUNE");
                        if (rootTokenPos != std::wstring::npos) {
                            size_t subfolderStart = rootTokenPos + 9;
                            if (subfolderStart < reqLastSlash) {

                                relativeSubfolder = filePath.substr(subfolderStart, (reqLastSlash - subfolderStart) + 1);


                                if (!relativeSubfolder.empty() && (relativeSubfolder[0] == L'\\' || relativeSubfolder[0] == L'/')) {
                                    relativeSubfolder = relativeSubfolder.substr(1);
                                }
                            }
                        }
                    }

                    std::wstring replacementPath = rootDir + relativeSubfolder + L"custom_data.win";

                    std::string customNarrow(replacementPath.begin(), replacementPath.end());
                    Logging::LogInfo(("Redirecting asset handle to target subfolder path: " + customNarrow).c_str());

                    HANDLE hCustomFile = fpCreateFileW(
                        replacementPath.c_str(),
                        dwDesiredAccess,
                        dwShareMode,
                        lpSecurityAttributes,
                        dwCreationDisposition,
                        dwFlagsAndAttributes,
                        hTemplateFile
                    );

                    if (hCustomFile != INVALID_HANDLE_VALUE) {
                        return hCustomFile;
                    }
                }
            }
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
    if (AllocConsole()) {
        SetConsoleTitleA("GMI Logger");
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
    }

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
