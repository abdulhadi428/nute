#include "stdafx.h"
#pragma unmanaged

#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"	//_vsnprintf
#include "string.h"	//strlen
#include "stdarg.h"	//va_...
#include "stdlib.h" //srand,rand
#include "time.h"	//time

#include "DownInterface.h"

namespace Helper {

	bool bLustraInRange = false;
	unsigned char LastLustraId = UNKNOWN_ID;
	unsigned char CurrentLockId = UNKNOWN_ID;

	void __cdecl LowLog(char* fmt,...)
	{
		char buf[4096];
		va_list args;
		va_start(args, fmt);
		_vsnprintf( buf, sizeof(buf), fmt, args);
		LowLevel::InvokeLog(0,buf);
	}

	int strlen(char* str) {
		return ::strlen(str);
	}

	bool bsrand = false;
	unsigned int GetRandom(unsigned int max)
	{
		if (!bsrand) {
			srand((unsigned int)time(NULL));
			bsrand = true;
		}
		int mod = (max+1);
		if (mod < 0) return 0;
		int rnd = rand() % mod;
		return rnd;
	}

	time_t startTime = 0; 
	long GetUptime()
	{
		if (startTime == 0) {
			startTime = time(NULL);
			return 0;
		}
		long uptime = (long)( time(NULL) - startTime );
		return uptime;
	}

} //namespace
