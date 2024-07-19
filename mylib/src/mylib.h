#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <fstream>

namespace mylib
{
	size_t GetFuncSize(void* func);
	uint32_t GetProcessID(const char* processName);
	uint32_t GetThreadID(uint32_t processID);
	uint8_t* MyReadFile(const char* fileName);

	namespace shadowVMT
	{
		class VMT
		{
		private:
			uintptr_t* originalVMT;
			uintptr_t* shadowVMT;
			uintptr_t* instanceVMT;
		public:
			VMT(uintptr_t* classInstance);
			~VMT();
			uintptr_t Hook(uint64_t index, uintptr_t hookFunc);
			template<typename Result, typename ... Args>
			Result Call(uint64_t index, Args ... args)
			{
				using Fn = Result(__thiscall*)(void*, decltype(Args)...);
				reinterpret_cast<Fn*>(originalVMT)[index](originalVMT, args...);
			}
		};
	}
}