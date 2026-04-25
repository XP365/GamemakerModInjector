#ifndef UNTITLED_PROXY_EXPORTS_H
#define UNTITLED_PROXY_EXPORTS_H

#include <windows.h>

extern HMODULE g_version_module;
extern INIT_ONCE g_init_once;
extern INIT_ONCE g_startup_once;

BOOL CALLBACK LoadVersionModule(PINIT_ONCE, PVOID, PVOID *);
BOOL CALLBACK StartInjectedRuntime(PINIT_ONCE, PVOID, PVOID *);
DWORD WINAPI InitThread(LPVOID);
FARPROC LoadExport(const char *name);

#endif
