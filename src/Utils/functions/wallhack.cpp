#include "Utils/memory/memory_func.h"
#include "Utils/offsets.h"
#include "Utils/config.h"

void WallHack() {
    static uintptr_t cachedAddress = 0;

    if (cachedAddress == 0) {
        uintptr_t baseAddress = GetWallhackAddress();
        if (baseAddress == 0) {
            return;
        }
        cachedAddress = baseAddress + 0x39;
    }

    DWORD oldProtect;
    VirtualProtectEx(hProcess, (LPVOID)cachedAddress, 1, PAGE_EXECUTE_READWRITE, &oldProtect);

    if (config.wallhack) {
        WPM<BYTE>(cachedAddress + 2, 0xE9);
    } else {
        WPM<BYTE>(cachedAddress + 2, 0xE1);
    }

    VirtualProtectEx(hProcess, (LPVOID)cachedAddress, 1, oldProtect, &oldProtect);
}
