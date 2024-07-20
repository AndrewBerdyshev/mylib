#include "mylib.h"

size_t mylib::GetFuncSize(uintptr_t func)
{
	size_t result = 0;
	for (; *reinterpret_cast<uint32_t*>(func + result++) != 0xCCCCCCCC;);
	return result + 3; // 0xCCCCCCCC included.
}

uint32_t mylib::GetProcessID(const char* processName)
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	Process32First(snapshot, &entry);
	do
	{
		if (!strcmp(entry.szExeFile, processName))
		{
			CloseHandle(snapshot);
			return entry.th32ProcessID;
		}
	} while (Process32Next(snapshot, &entry));
	CloseHandle(snapshot);
	return NULL;
}

uint32_t mylib::GetThreadID(uint32_t processID)
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	THREADENTRY32 entry;
	entry.dwSize = sizeof(entry);

	Thread32First(snapshot, &entry);
	do
	{
		if (entry.th32OwnerProcessID == processID)
		{
			CloseHandle(snapshot);
			return entry.th32ThreadID;
		}
	} while (Thread32Next(snapshot, &entry));
	CloseHandle(snapshot);
	return NULL;
}

uint8_t* mylib::MyReadFile(const char* fileName)
{
	std::ifstream fin(fileName, std::ios::binary | std::ios::ate | std::ios::in);
	if (!fin.is_open()) return nullptr;
	const auto fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);
	auto result = new char[fileSize];
	fin.read(result, fileSize);
	return reinterpret_cast<uint8_t*>(result);
}
