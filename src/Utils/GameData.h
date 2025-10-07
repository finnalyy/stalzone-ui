#pragma once
#include "pch.h"
#include "Utils/Memory.h"

struct Game
{
    static DWORD getProcessId(const char* processName) {
        return pid::get_process_id(processName);
    }

    static bool isRunning(const char* processName) {
        return getProcessId(processName) != 0;
    }
};

namespace Globals {
    inline std::uintptr_t baseAddress = 0;

    inline bool g_wallhack = false;
    inline bool g_wallhack_C = false;
    inline int g_wallhackBind = 0;
    // chams
    inline bool g_chams = false;
    inline bool g_chams_C = false;
    inline int g_chamsBind = 0;
}

struct Wallhack {
private:
    inline static std::uintptr_t address = 0;
    inline static BYTE orig[]  = { 0x48, 0xC1, 0xE1, 0x04 };
    inline static BYTE patch[] = { 0xC1, 0xE0, 0x04, 0x90 };
    inline static const SIZE_T size = sizeof(orig);

public:
    static void Init() { address = Globals::baseAddress + 0x800F7B; }
    static void toggle(bool& enabled, HANDLE processHandle);
    static SIZE_T getSize() { return size; }
};

struct Chams {
private:
    inline static std::uintptr_t address = 0;
    inline static BYTE orig[]  = { 0x48, 0xC1, 0xE1, 0x04 };
    inline static BYTE patch[] = { 0x48, 0xC1, 0xE1, 0x05 };
    inline static const SIZE_T size = sizeof(orig);

public:
    static void Init() { address = Globals::baseAddress + 0x80087B; }
    static void toggle(bool& enabled, HANDLE processHandle);
    static SIZE_T getSize() { return size; }
};


// Wallhack: 0x80043B, 0x800F7B 


// Chams: 0x7FFD3B, 0x80087B