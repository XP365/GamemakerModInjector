#include <windows.h>
#include <dbghelp.h>

#include <cstdlib>

#include "Logging.h"

namespace {
bool BuildModuleDirectoryPath(char *buffer, const DWORD buffer_size) {
    if (buffer == nullptr || buffer_size == 0) {
        return false;
    }

    const DWORD length = GetModuleFileNameA(nullptr, buffer, buffer_size);
    if (length == 0 || length >= buffer_size) {
        return false;
    }

    for (LONG i = static_cast<LONG>(length) - 1; i >= 0; --i) {
        if (buffer[i] == '\\' || buffer[i] == '/') {
            buffer[i + 1] = '\0';
            return true;
        }
    }

    return false;
}

bool BuildTimestampedPath(char *buffer, const DWORD buffer_size, const char *subdir, const char *extension) {
    if (buffer == nullptr || buffer_size == 0 || subdir == nullptr || extension == nullptr) {
        return false;
    }

    char module_dir[MAX_PATH];
    if (!BuildModuleDirectoryPath(module_dir, MAX_PATH)) {
        return false;
    }

    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    char file_name[64];
    wsprintfA(
        file_name,
        "GMI_%02u_%02u_%02u_%02u%s",
        local_time.wDay,
        local_time.wHour,
        local_time.wMinute,
        local_time.wSecond,
        extension
    );

    lstrcpynA(buffer, module_dir, static_cast<int>(buffer_size));
    const int current_length = lstrlenA(buffer);
    if (current_length <= 0 || current_length >= static_cast<int>(buffer_size)) {
        return false;
    }

    lstrcpynA(buffer + current_length, subdir, static_cast<int>(buffer_size - current_length));
    const int with_subdir_length = lstrlenA(buffer);
    if (with_subdir_length <= 0 || with_subdir_length >= static_cast<int>(buffer_size)) {
        return false;
    }

    lstrcpynA(buffer + with_subdir_length, "\\", static_cast<int>(buffer_size - with_subdir_length));
    const int with_slash_length = lstrlenA(buffer);
    if (with_slash_length <= 0 || with_slash_length >= static_cast<int>(buffer_size)) {
        return false;
    }

    lstrcpynA(buffer + with_slash_length, file_name, static_cast<int>(buffer_size - with_slash_length));
    return lstrlenA(buffer) > 0;
}

bool EnsureNamedSubdirectoryExists(const char *subdir) {
    if (subdir == nullptr) {
        return false;
    }

    char module_dir[MAX_PATH];
    if (!BuildModuleDirectoryPath(module_dir, MAX_PATH)) {
        return false;
    }

    const int current_length = lstrlenA(module_dir);
    if (current_length <= 0 || current_length >= MAX_PATH) {
        return false;
    }

    lstrcpynA(module_dir + current_length, subdir, static_cast<int>(MAX_PATH - current_length));
    if (!CreateDirectoryA(module_dir, nullptr)) {
        const DWORD error = GetLastError();
        if (error != ERROR_ALREADY_EXISTS) {
            return false;
        }
    }

    return true;
}
}


bool Logging::BuildLogPath(char *buffer, const DWORD buffer_size) {
    return BuildTimestampedPath(buffer, buffer_size, "logs", ".log");
}

void Logging::WriteLogLine(const char *message) {
    char line[1024];
    line[0] = '\0';

    if (message != nullptr) {
        lstrcpynA(line, message, static_cast<int>(sizeof(line)));
    }

    if (const int line_length = lstrlenA(line); line_length == 0 || line[line_length - 1] != '\n') {
        lstrcpynA(line + line_length, "\r\n", static_cast<int>(sizeof(line) - line_length));
    }

    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE && console != nullptr) {
        DWORD written = 0;
        WriteConsoleA(console, line, static_cast<DWORD>(lstrlenA(line)), &written, nullptr);
    }

    char log_path[MAX_PATH];
    if (!BuildLogPath(log_path, MAX_PATH)) {
        return;
    }

    if (!EnsureNamedSubdirectoryExists("logs")) {
        return;
    }

    const HANDLE file = CreateFileA(
        log_path,
        FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        WriteFile(file, line, static_cast<DWORD>(lstrlenA(line)), &written, nullptr);
        CloseHandle(file);
    }
}

void Logging::LogInfo(const char *message) {
    char line[1024];
    lstrcpynA(line, "[INFO]: ", static_cast<int>(sizeof(line)));
    lstrcpynA(line + lstrlenA(line), message != nullptr ? message : "", static_cast<int>(sizeof(line) - lstrlenA(line)));
    WriteLogLine(line);
}

void Logging::LogWarning(const char *message) {
    char line[1024];
    lstrcpynA(line, "[WARNING]: ", static_cast<int>(sizeof(line)));
    lstrcpynA(line + lstrlenA(line), message != nullptr ? message : "", static_cast<int>(sizeof(line) - lstrlenA(line)));
    WriteLogLine(line);
}

void Logging::LogError(const char *message) {
    char line[1024];
    lstrcpynA(line, "[ERROR]: ", static_cast<int>(sizeof(line)));
    lstrcpynA(line + lstrlenA(line), message != nullptr ? message : "", static_cast<int>(sizeof(line) - lstrlenA(line)));
    WriteLogLine(line);
}

void Logging::LogErrorAndPanic(const char *message) {
    //Log the error and quit the game
    char line[1024];
    lstrcpynA(line, "[CRITICAL ERROR]: ", static_cast<int>(sizeof(line)));
    lstrcpynA(line + lstrlenA(line), message != nullptr ? message : "", static_cast<int>(sizeof(line) - lstrlenA(line)));
    WriteLogLine(line);
    CreateFullMemoryDump();
    exit(-1);
}

bool Logging::CreateFullMemoryDump() {
    if (!EnsureNamedSubdirectoryExists("dumps")) {
        return false;
    }

    char dump_path[MAX_PATH];
    if (!BuildTimestampedPath(dump_path, MAX_PATH, "dumps", ".dmp")) {
        return false;
    }

    const HANDLE dump_file = CreateFileA(
        dump_path,
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (dump_file == INVALID_HANDLE_VALUE) {
        return false;
    }

    const MINIDUMP_TYPE dump_type = static_cast<MINIDUMP_TYPE>(
        MiniDumpWithPrivateReadWriteMemory |
        MiniDumpWithDataSegs |
        MiniDumpWithThreadInfo |
        MiniDumpWithHandleData |
        MiniDumpWithIndirectlyReferencedMemory |
        MiniDumpWithUnloadedModules |
        MiniDumpWithFullMemoryInfo
    );

    const BOOL success = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        dump_file,
        dump_type,
        nullptr,
        nullptr,
        nullptr
    );

    const DWORD error = success == TRUE ? ERROR_SUCCESS : GetLastError();
    CloseHandle(dump_file);

    if (success != TRUE) {
        DeleteFileA(dump_path);

        char message[128];
        wsprintfA(message, "MiniDumpWriteDump failed with error %lu.", error);
        WriteLogLine(message);
        return false;
    }

    return true;
}
