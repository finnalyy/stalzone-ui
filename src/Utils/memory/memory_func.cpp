#include "memory_func.h"

#include <TlHelp32.h>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "offsets.h"

HANDLE hProcess = nullptr;
DWORD processId = -1;

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32FirstW(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32NextW(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

bool AttachToProcess() {
	processId = GetProcId(STEAM_EXE); // steam
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	return hProcess != nullptr;
}

BYTE* RBM(SIZE_T address, size_t size) {
	BYTE* buffer = new BYTE[size];
	ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, NULL);
	return buffer;
}

//bool WBM(SIZE_T address, const byte* bytes, size_t size) {
//	return WriteProcessMemory(hProcess, (LPVOID)address, bytes, size, NULL);
//}



bool WBM(SIZE_T address, const char* hexString) {
	std::istringstream hexStream(hexString);
	BYTE bytes[1024];
	int size = 0;
	int byteValue;
	while (hexStream >> std::hex >> byteValue) {
		bytes[size++] = static_cast<BYTE>(byteValue);
	}

	bool result = WriteProcessMemory(hProcess, (LPVOID)address, bytes, size, NULL);
	return result;
}

uintptr_t GetModuleBaseAddress(const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32W modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32FirstW(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32NextW(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

std::vector<uintptr_t> FindAllPatterns(uintptr_t baseAddress, size_t scanSize, const char* pattern, size_t patternSize) {
    std::vector<uintptr_t> addresses;
    const size_t chunkSize = 4096;
    BYTE* buffer = new BYTE[chunkSize];
    
    for (size_t i = 0; i < scanSize; i += chunkSize) {
        size_t readSize = min(chunkSize, scanSize - i);
        
        if (!ReadProcessMemory(hProcess, (LPCVOID)(baseAddress + i), buffer, readSize, NULL)) {
            continue;
        }
        
        for (size_t j = 0; j < readSize - patternSize; j++) {
            bool found = true;
            for (size_t k = 0; k < patternSize; k++) {
                if (buffer[j + k] != (BYTE)pattern[k]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                addresses.push_back(baseAddress + i + j);
            }
        }
    }
    
    delete[] buffer;
    return addresses;
}

uintptr_t GetWallhackAddress() {
    const char pattern[] = "\xA9\xFF\xF7\xFF\xFF";
    size_t patternSize = 5;
    
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;
    
    MODULEENTRY32W modEntry;
    modEntry.dwSize = sizeof(modEntry);
    
    if (Module32FirstW(hSnap, &modEntry)) {
        do {
            if (!_wcsicmp(modEntry.szModule, L"nvoglv64.dll")) {
                CloseHandle(hSnap);
                
                std::vector<uintptr_t> results = FindAllPatterns(
                    (uintptr_t)modEntry.modBaseAddr, 
                    modEntry.modBaseSize, 
                    pattern, 
                    patternSize
                );
                
                if (results.size() > 8) {
                    return results[8];
                }
                return 0;
            }
        } while (Module32NextW(hSnap, &modEntry));
    }
    
    CloseHandle(hSnap);
    return 0;
}

uintptr_t GetChamsAddress() {
    const char pattern[] = "\xA9\xFF\xF7\xFF\xFF";
    size_t patternSize = 5;
    
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;
    
    MODULEENTRY32W modEntry;
    modEntry.dwSize = sizeof(modEntry);
    
    if (Module32FirstW(hSnap, &modEntry)) {
        do {
            if (!_wcsicmp(modEntry.szModule, L"nvoglv64.dll")) {
                CloseHandle(hSnap);
                
                std::vector<uintptr_t> results = FindAllPatterns(
                    (uintptr_t)modEntry.modBaseAddr, 
                    modEntry.modBaseSize, 
                    pattern, 
                    patternSize
                );
                
                if (results.size() > 0) {
                    return results[0];
                }
                return 0;
            }
        } while (Module32NextW(hSnap, &modEntry));
    }
    
    CloseHandle(hSnap);
    return 0;
}