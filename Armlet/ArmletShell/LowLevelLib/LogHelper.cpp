#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "DownInterface.h"

namespace Helper {

	typedef void (__stdcall INT_STR_ROUTINE)(int n, char* str);
	extern INT_STR_ROUTINE* pfnLog;
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

} //namespace
