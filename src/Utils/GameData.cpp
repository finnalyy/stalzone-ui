#include "GameData.h"

void Wallhack::toggle(bool& enabled, HANDLE processHandle)
{
    SIZE_T bytesWritten = 0;
    BOOL result;

    if (!enabled) {
        result = WriteProcessMemory(processHandle, (LPVOID)address, patch, sizeof(patch), &bytesWritten);
    }
    else {
        result = WriteProcessMemory(processHandle, (LPVOID)address, orig, sizeof(orig), &bytesWritten);
    }
    if (result && bytesWritten == sizeof(patch)) {
        enabled = !enabled;
    }
    else {
        std::cerr << "Wallhack toggle failed: " << GetLastError() << std::endl;
    }
}

void Chams::toggle(bool& enabled, HANDLE processHandle)
{
    SIZE_T bytesWritten = 0;
    BOOL result;

    if (!enabled)
        result = WriteProcessMemory(processHandle, (LPVOID)address, patch, sizeof(patch), &bytesWritten);
    else
        result = WriteProcessMemory(processHandle, (LPVOID)address, orig, sizeof(orig), &bytesWritten);

    if (result && bytesWritten == sizeof(patch))
        enabled = !enabled;
    else
        std::cerr << "Chams toggle failed: " << GetLastError() << std::endl;
}
