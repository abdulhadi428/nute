#pragma once
#include "ArmletApi.h"
#include "KernelApi.h"

namespace ArmletKernel {
	extern int CureCharges[15];
	extern bool bLustraInRange;
	extern unsigned char LastLustraId;
	extern unsigned char CurrentLockId;

	void __cdecl LowLog(char* fmt,...);
} //namespace
