

#ifndef GAMEMAKERMODINJECTOR_LOGGING_H
#define GAMEMAKERMODINJECTOR_LOGGING_H


class Logging {
    public:
    static void WriteLogLine(const char *message);

    static void LogInfo(const char *message);

    static void LogWarning(const char *message);

    static void LogError(const char *message);

    static void LogErrorAndPanic(const char *message);

    static bool BuildLogPath(char *buffer, DWORD buffer_size);
};


#endif //GAMEMAKERMODINJECTOR_LOGGING_H