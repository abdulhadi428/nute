#pragma once

namespace Helper {

	#define UNREFERENCED_PARAMETER(P)	(P)

	void __cdecl LowLog(char* fmt,...);	//LogHelper.cpp
	long GetRunningTime();				//CppHelper.cpp
	int strlen(char* str);

} //namespace

using namespace Helper;