#pragma once
#include "PlatformDependent.h"

namespace KernelApi {
	extern __LOWVAR unsigned short VideoMemory[SCREENX*SCREENY];

	typedef void (__stdcall VIBRO_CALLBACK)();
	extern __LOWVAR VIBRO_CALLBACK* Vibro;
	typedef void (__stdcall LOG_CALLBACK)(char* msg);
	extern __LOWVAR LOG_CALLBACK* Log;
	typedef void (__stdcall SET_PILL_NAME_CALLBACK)(int pill_id, char* pill_name);
	extern __LOWVAR SET_PILL_NAME_CALLBACK* SetPillName;
	typedef void (__stdcall UPDATE_CURRENT_CURE_CALLBACK)();
	extern __LOWVAR UPDATE_CURRENT_CURE_CALLBACK* UpdateCurrentCure;
	typedef void (__stdcall UPDATE_CURRENT_TORTURE_CALLBACK)();
	extern __LOWVAR UPDATE_CURRENT_TORTURE_CALLBACK* UpdateCurrentTorture;	//for medicine emulation

	typedef void (__stdcall UPDATE_KERNEL_CALLBACK)(int armlet_id, int lustra_id, int battery_level,
			int gate_id, int signal_level, int hours, int minutes, int uptime);
	extern __LOWVAR UPDATE_KERNEL_CALLBACK* UpdateKernel;
	typedef void (__stdcall UPDATE_APP_STATE_CALLBACK)(int blood, int toxins, int pulse, int temperature);
	extern __LOWVAR UPDATE_APP_STATE_CALLBACK* UpdateAppState;

	int __LOWCALL GetPillCharges(int pill_id);
	void __LOWCALL LustraInRange(bool bInRange);
	void __LOWCALL SetCurrentLustra(int lustra_id);
	void __LOWCALL SetCurrentLock(int lock_id);

	void __LOWCALL CheckTimers();
}