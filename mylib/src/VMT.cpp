#include "mylib.h"

uintptr_t mylib::VMT::VMTGet(uintptr_t vmt, size_t index)
{
	return (*reinterpret_cast<uintptr_t**>(vmt))[index];
}