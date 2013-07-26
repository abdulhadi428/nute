#pragma once
#include "PlatformDependent.h"

namespace KernelApi {
	extern __LOWVAR unsigned short VideoMemory[SCREENX*SCREENY];

	typedef void (__stdcall VIBRO_CALLBACK)();
	extern __LOWVAR VIBRO_CALLBACK* Vibro;
	typedef void (__stdcall LOG_CALLBACK)(char* msg);
	extern __LOWVAR LOG_CALLBACK* Log;
	typedef void (__stdcall SET_CURE_NAME_CALLBACK)(int cure_id, char* cure_name);
	extern __LOWVAR SET_CURE_NAME_CALLBACK* SetCureName;
	typedef void (__stdcall UPDATE_CURRENT_CURE_CALLBACK)();
	extern __LOWVAR UPDATE_CURRENT_CURE_CALLBACK* UpdateCurrentCure;

	int __LOWCALL GetPillCharges(int cure_id);
	void __LOWCALL LustraInRange(bool bInRange);
	void __LOWCALL SetCurrentLustra(int lustra_id);
	void __LOWCALL SetCurrentLock(int lock_id);

	void __LOWCALL CheckTimers();
}