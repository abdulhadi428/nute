#pragma once

namespace Helper {

	extern bool bLustraInRange;
	extern unsigned char LastLustraId;
	extern unsigned char CurrentLockId;

	#define UNREFERENCED_PARAMETER(P)	(P)
	#define COMPILE_TIME_CHECK(e) extern char _COMPILE_TIME_CHECK_[(e)?1:-1]

	void __cdecl LowLog(char* fmt,...);
	int strlen(char* str);
	unsigned int GetRandom(unsigned int max);
	long GetUptime();

} //namespace

using namespace Helper;