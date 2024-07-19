#include "mylib.h"

mylib::shadowVMT::VMT::VMT(uintptr_t* vmt)
{
	if (!vmt) return;
	this->instanceVMT = vmt;

	this->originalVMT = reinterpret_cast<uintptr_t*>(*vmt);

	uint64_t size = 0;
	for (; this->originalVMT[size++];);

	this->shadowVMT = new uintptr_t[size];
	memcpy(this->shadowVMT, this->originalVMT, size * sizeof(uintptr_t));

	*vmt = reinterpret_cast<uintptr_t>(this->shadowVMT);
}

mylib::shadowVMT::VMT::~VMT()
{
	delete[] shadowVMT;
	*(this->instanceVMT) = reinterpret_cast<uintptr_t>(this->originalVMT);
}

uintptr_t mylib::shadowVMT::VMT::Hook(uint64_t index, uintptr_t hookFunc)
{
	this->shadowVMT[index] = hookFunc;
	return this->originalVMT[index];
}
