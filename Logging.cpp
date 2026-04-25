#include <windows.h>
#include <string>

#include "Logging.h"

bool Logging::BuildLogPath(char *buffer, const DWORD buffer_size) {
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
            return lstrcatA(buffer, "GMI.log") != nullptr;
        }
    }

    buffer[0] = '.';
    buffer[1] = '\\';
    buffer[2] = '\0';
    return lstrcatA(buffer, "GMI.log") != nullptr;
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
    WriteLogLine((std::string("[INFO]: ") + message).c_str());
}

void Logging::LogWarning(const char *message) {
    WriteLogLine((std::string("[WARNING]: ") + message).c_str());
}

void Logging::LogError(const char *message) {
    WriteLogLine((std::string("[ERROR]: ") + message).c_str());
}

void Logging::LogErrorAndPanic(const char *message) {
    //Log the error and quit the game
    WriteLogLine((std::string("[CRITICAL ERROR]: ") + message).c_str());
    exit(-1);
}