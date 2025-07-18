#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <fstream>

#define CONCAT(x, y) x##y
#define CONCAT2(x, y) CONCAT(x, y)
#define PAD(size) uint8_t CONCAT2(pad_, __COUNTER__)[size];

namespace mylib
{
	size_t GetFuncSize(uintptr_t func);
	uint32_t GetProcessID(const char* processName);
	uint32_t GetThreadID(uint32_t processID);
	uint8_t* MyReadFile(const char* fileName);

	// For hooking
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
			uintptr_t Hook(size_t index, uintptr_t hookFunc);
			template<typename Result, typename ... Args>
			Result Call(size_t index, Args ... args)
			{
				using Fn = Result(__thiscall*)(void*, decltype(args)...);
				reinterpret_cast<Fn*>(originalVMT)[index](originalVMT, args...);
			}
		};
	}

	// For calling, getting address
	namespace VMT
	{
		template<typename Return, typename ... Params>
		Return VMTCall(void* vmt, size_t index, Params ... params)
		{
			using Fn = Return(__thiscall*)(void*, decltype(params) ...);
			return (*reinterpret_cast<Fn**>(vmt))[index](vmt, params ...);
		}
		uintptr_t VMTGet(uintptr_t vmt, size_t index);
	}

	// cheat engine like pointer magic
	template<typename ... Params>
	uintptr_t PointerMagic(uintptr_t start, Params ... offsets)
	{
		uintptr_t result = start;
		([&]
			{
				result = *reinterpret_cast<uintptr_t*>(result);
				result += offsets;
			} (), ...);
		return result;
	}
}
