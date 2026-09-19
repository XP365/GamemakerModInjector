#include <windows.h>

#ifndef GAMEMAKERMODINJECTOR_LOGGING_H
#define GAMEMAKERMODINJECTOR_LOGGING_H


namespace Logging {
    void WriteLogLine(const char *message);

    void LogInfo(const char *message);

    void LogWarning(const char *message);

    void LogError(const char *message);

    void LogErrorAndPanic(const char *message);

    bool BuildLogPath(char *buffer, DWORD buffer_size);
    bool CreateFullMemoryDump();
};


#endif //GAMEMAKERMODINJECTOR_LOGGING_H
