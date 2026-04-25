#include "proxy_exports.h"

namespace {
using GetFileVersionInfoA_t = BOOL(WINAPI *)(LPCSTR, DWORD, DWORD, LPVOID);
using GetFileVersionInfoByHandle_t = BOOL(WINAPI *)(DWORD, DWORD, LPVOID);
using GetFileVersionInfoExA_t = BOOL(WINAPI *)(DWORD, LPCSTR, DWORD, DWORD, LPVOID);
using GetFileVersionInfoExW_t = BOOL(WINAPI *)(DWORD, LPCWSTR, DWORD, DWORD, LPVOID);
using GetFileVersionInfoSizeA_t = DWORD(WINAPI *)(LPCSTR, LPDWORD);
using GetFileVersionInfoSizeExA_t = DWORD(WINAPI *)(DWORD, LPCSTR, LPDWORD);
using GetFileVersionInfoSizeExW_t = DWORD(WINAPI *)(DWORD, LPCWSTR, LPDWORD);
using GetFileVersionInfoSizeW_t = DWORD(WINAPI *)(LPCWSTR, LPDWORD);
using GetFileVersionInfoW_t = BOOL(WINAPI *)(LPCWSTR, DWORD, DWORD, LPVOID);
using VerFindFileA_t = DWORD(WINAPI *)(DWORD, LPSTR, LPSTR, LPSTR, LPSTR, PUINT, LPSTR, PUINT);
using VerFindFileW_t = DWORD(WINAPI *)(DWORD, LPWSTR, LPWSTR, LPWSTR, LPWSTR, PUINT, LPWSTR, PUINT);
using VerInstallFileA_t = DWORD(WINAPI *)(DWORD, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, PUINT);
using VerInstallFileW_t = DWORD(WINAPI *)(DWORD, LPWSTR, LPWSTR, LPWSTR, LPWSTR, LPWSTR, LPWSTR, PUINT);
using VerLanguageNameA_t = DWORD(WINAPI *)(DWORD, LPSTR, DWORD);
using VerLanguageNameW_t = DWORD(WINAPI *)(DWORD, LPWSTR, DWORD);
using VerQueryValueA_t = BOOL(WINAPI *)(LPCVOID, LPCSTR, LPVOID *, PUINT);
using VerQueryValueIndexA_t = BOOL(WINAPI *)(LPCVOID, DWORD, LPVOID *, PUINT);
using VerQueryValueIndexW_t = BOOL(WINAPI *)(LPCVOID, DWORD, LPVOID *, PUINT);
using VerQueryValueW_t = BOOL(WINAPI *)(LPCVOID, LPCWSTR, LPVOID *, PUINT);
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoA(
    LPCSTR filename, DWORD handle, DWORD len, LPVOID data) {
    const auto fn = reinterpret_cast<GetFileVersionInfoA_t>(LoadExport("GetFileVersionInfoA"));
    return fn ? fn(filename, handle, len, data) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoByHandle(
    DWORD handle, DWORD len, LPVOID data) {
    const auto fn = reinterpret_cast<GetFileVersionInfoByHandle_t>(LoadExport("GetFileVersionInfoByHandle"));
    return fn ? fn(handle, len, data) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoExA(
    DWORD flags, LPCSTR filename, DWORD handle, DWORD len, LPVOID data) {
    const auto fn = reinterpret_cast<GetFileVersionInfoExA_t>(LoadExport("GetFileVersionInfoExA"));
    return fn ? fn(flags, filename, handle, len, data) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoExW(
    DWORD flags, LPCWSTR filename, DWORD handle, DWORD len, LPVOID data) {
    const auto fn = reinterpret_cast<GetFileVersionInfoExW_t>(LoadExport("GetFileVersionInfoExW"));
    return fn ? fn(flags, filename, handle, len, data) : FALSE;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeA(
    LPCSTR filename, LPDWORD handle) {
    const auto fn = reinterpret_cast<GetFileVersionInfoSizeA_t>(LoadExport("GetFileVersionInfoSizeA"));
    return fn ? fn(filename, handle) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExA(
    DWORD flags, LPCSTR filename, LPDWORD handle) {
    const auto fn = reinterpret_cast<GetFileVersionInfoSizeExA_t>(LoadExport("GetFileVersionInfoSizeExA"));
    return fn ? fn(flags, filename, handle) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExW(
    DWORD flags, LPCWSTR filename, LPDWORD handle) {
    const auto fn = reinterpret_cast<GetFileVersionInfoSizeExW_t>(LoadExport("GetFileVersionInfoSizeExW"));
    return fn ? fn(flags, filename, handle) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeW(
    LPCWSTR filename, LPDWORD handle) {
    const auto fn = reinterpret_cast<GetFileVersionInfoSizeW_t>(LoadExport("GetFileVersionInfoSizeW"));
    return fn ? fn(filename, handle) : 0;
}

extern "C" __declspec(dllexport) BOOL WINAPI GetFileVersionInfoW(
    LPCWSTR filename, DWORD handle, DWORD len, LPVOID data) {
    const auto fn = reinterpret_cast<GetFileVersionInfoW_t>(LoadExport("GetFileVersionInfoW"));
    return fn ? fn(filename, handle, len, data) : FALSE;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerFindFileA(
    DWORD flags, LPSTR filename, LPSTR win_dir, LPSTR app_dir,
    LPSTR cur_dir, PUINT cur_dir_len, LPSTR dest_dir, PUINT dest_dir_len) {
    const auto fn = reinterpret_cast<VerFindFileA_t>(LoadExport("VerFindFileA"));
    return fn ? fn(flags, filename, win_dir, app_dir, cur_dir, cur_dir_len, dest_dir, dest_dir_len) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerFindFileW(
    DWORD flags, LPWSTR filename, LPWSTR win_dir, LPWSTR app_dir,
    LPWSTR cur_dir, PUINT cur_dir_len, LPWSTR dest_dir, PUINT dest_dir_len) {
    const auto fn = reinterpret_cast<VerFindFileW_t>(LoadExport("VerFindFileW"));
    return fn ? fn(flags, filename, win_dir, app_dir, cur_dir, cur_dir_len, dest_dir, dest_dir_len) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerInstallFileA(
    DWORD flags, LPSTR src_filename, LPSTR dest_filename, LPSTR src_dir,
    LPSTR dest_dir, LPSTR cur_dir, LPSTR tmp_file, PUINT tmp_file_len) {
    const auto fn = reinterpret_cast<VerInstallFileA_t>(LoadExport("VerInstallFileA"));
    return fn ? fn(flags, src_filename, dest_filename, src_dir, dest_dir, cur_dir, tmp_file, tmp_file_len) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerInstallFileW(
    DWORD flags, LPWSTR src_filename, LPWSTR dest_filename, LPWSTR src_dir,
    LPWSTR dest_dir, LPWSTR cur_dir, LPWSTR tmp_file, PUINT tmp_file_len) {
    const auto fn = reinterpret_cast<VerInstallFileW_t>(LoadExport("VerInstallFileW"));
    return fn ? fn(flags, src_filename, dest_filename, src_dir, dest_dir, cur_dir, tmp_file, tmp_file_len) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerLanguageNameA(
    DWORD lang, LPSTR buffer, DWORD size) {
    const auto fn = reinterpret_cast<VerLanguageNameA_t>(LoadExport("VerLanguageNameA"));
    return fn ? fn(lang, buffer, size) : 0;
}

extern "C" __declspec(dllexport) DWORD WINAPI VerLanguageNameW(
    DWORD lang, LPWSTR buffer, DWORD size) {
    const auto fn = reinterpret_cast<VerLanguageNameW_t>(LoadExport("VerLanguageNameW"));
    return fn ? fn(lang, buffer, size) : 0;
}

extern "C" __declspec(dllexport) BOOL WINAPI VerQueryValueA(
    LPCVOID block, LPCSTR sub_block, LPVOID *buffer, PUINT len) {
    const auto fn = reinterpret_cast<VerQueryValueA_t>(LoadExport("VerQueryValueA"));
    return fn ? fn(block, sub_block, buffer, len) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI VerQueryValueIndexA(
    LPCVOID block, DWORD index, LPVOID *buffer, PUINT len) {
    const auto fn = reinterpret_cast<VerQueryValueIndexA_t>(LoadExport("VerQueryValueIndexA"));
    return fn ? fn(block, index, buffer, len) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI VerQueryValueIndexW(
    LPCVOID block, DWORD index, LPVOID *buffer, PUINT len) {
    const auto fn = reinterpret_cast<VerQueryValueIndexW_t>(LoadExport("VerQueryValueIndexW"));
    return fn ? fn(block, index, buffer, len) : FALSE;
}

extern "C" __declspec(dllexport) BOOL WINAPI VerQueryValueW(
    LPCVOID block, LPCWSTR sub_block, LPVOID *buffer, PUINT len) {
    const auto fn = reinterpret_cast<VerQueryValueW_t>(LoadExport("VerQueryValueW"));
    return fn ? fn(block, sub_block, buffer, len) : FALSE;
}
