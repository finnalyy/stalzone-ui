#pragma once
#include <Windows.h>
#include <vector>
#include <string>

extern HANDLE hProcess;

bool AttachToProcess();

template<typename T> T RPM(SIZE_T address) {
	T buffer;
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template<typename T> void WPM(SIZE_T address, T value) {
	WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);
}

template<typename T>
T ReadOffsetValue(uintptr_t baseAddress, std::vector<DWORD> offsets) {
	uintptr_t currentPtr = baseAddress + offsets[0];
	currentPtr = RPM<uintptr_t>(currentPtr);
	for (size_t i = 1; i < offsets.size() - 1; i++) {
		currentPtr += offsets[i];
		currentPtr = RPM<uintptr_t>(currentPtr);
	}
	currentPtr += offsets.back();
	return RPM<T>(currentPtr);
}

BYTE* RBM(SIZE_T address, size_t size);
//bool WBM(SIZE_T address, const BYTE* bytes, size_t size);
bool WBM(SIZE_T address, const char* hexString);

uintptr_t GetModuleBaseAddress(const wchar_t* modName);
std::vector<uintptr_t> FindAllPatterns(uintptr_t baseAddress, size_t scanSize, const char* pattern, size_t patternSize);
uintptr_t GetWallhackAddress();
uintptr_t GetChamsAddress();