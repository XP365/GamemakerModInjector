#include <windows.h>

#include <cstdlib>

#include "Logging.h"


bool Logging::BuildLogPath(char *buffer, const DWORD buffer_size) {
    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    char file_name[64];
    wsprintfA(file_name,"logs/GMI_%02u_%02u_%02u.log", local_time.wDay, local_time.wHour, local_time.wMinute);


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
            return lstrcatA(buffer, file_name) != nullptr;
        }
    }

    buffer[0] = '.';
    buffer[1] = '\\';
    buffer[2] = '\0';
    return lstrcatA(buffer, file_name) != nullptr;
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

    //Create the log dir if it doesn't exist
    CreateDirectoryA("logs", nullptr);

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
    exit(-1);
}
