#include "ArmletKernel.h"

#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"	//_vsnprintf
#include "stdarg.h"	//va_...
#include "string.h"	//strlen
#include "malloc.h" //malloc

namespace ArmletKernel {	
	int CureCharges[15] = {
		20,20,20,20,20,
		20,20,20,20,20,
		20,20,20,20,20
	};
	int TortureCharges[7] = {
		1,2,3,4,5,6,7
	};
	bool bLustraInRange = false;
	unsigned char LastLustraId = UNKNOWN_ID;
	unsigned char CurrentLockId = UNKNOWN_ID;

	void __cdecl LowLog(char* fmt,...)
	{
		char buf[4096];
		va_list args;
		va_start(args, fmt);
		_vsnprintf( buf, sizeof(buf), fmt, args);
		KernelApi::Log(buf);
	}
} //namespace
