#include <windows.h>
#include <string>

#ifndef GAMEMAKERMODINJECTOR_LOGGING_H
#define GAMEMAKERMODINJECTOR_LOGGING_H


namespace Logging {
    void WriteLogLine(const char *message);

    void LogInfo(const std::string& message);

    void LogWarning(const std::string& message);

    void LogError(const std::string& message);

    void LogErrorAndPanic(const std::string& message);

    void ShowMessage(const std::string& message);

    bool BuildLogPath(char *buffer, DWORD buffer_size);

};


#endif //GAMEMAKERMODINJECTOR_LOGGING_H
