#include "ArmletKernel.h"

namespace KernelApi {
	__LOWVAR unsigned short VideoMemory[SCREENX*SCREENY];

	__LOWVAR VIBRO_CALLBACK* Vibro = 0;
	__LOWVAR LOG_CALLBACK* Log = 0;
	__LOWVAR SET_PILL_NAME_CALLBACK* SetPillName = 0;
	__LOWVAR UPDATE_CURRENT_CURE_CALLBACK* UpdateCurrentCure = 0;
	__LOWVAR UPDATE_CURRENT_TORTURE_CALLBACK* UpdateCurrentTorture = 0;

	__LOWVAR UPDATE_KERNEL_CALLBACK* UpdateKernel = 0;
	__LOWVAR UPDATE_APP_STATE_CALLBACK* UpdateAppState = 0;

	int __LOWCALL GetPillCharges(int pill_id)
	{
		if ((pill_id < 0)||(pill_id > 14)) {
			if ((pill_id < 20)||(pill_id > 26))
				return 20;
			return ArmletKernel::TortureCharges[pill_id-20];
		}
		return ArmletKernel::CureCharges[pill_id];
	}

	void __LOWCALL SetPillCharges(int pill_id, int charges)
	{
		if ((pill_id < 0)||(pill_id > 14)) {
			if ((pill_id < 20)||(pill_id > 26))
				return;
			ArmletKernel::TortureCharges[pill_id-20] = charges;
		}
		ArmletKernel::CureCharges[pill_id] = charges;
	}

	void __LOWCALL LustraInRange(bool bInRange)
	{
		ArmletKernel::bLustraInRange = bInRange;
	}

	void __LOWCALL SetCurrentLustra(int lustra_id)
	{
		if ((lustra_id < 0) || (lustra_id >= UNKNOWN_ID))
			ArmletKernel::LastLustraId = UNKNOWN_ID;
			ArmletKernel::LastLustraId = (unsigned char)lustra_id;
	}

	void __LOWCALL SetCurrentLock(int lock_id)
	{
		if ((lock_id <= 0) || (lock_id >= UNKNOWN_ID))
			ArmletKernel::CurrentLockId = UNKNOWN_ID;
		ArmletKernel::CurrentLockId = (unsigned char)lock_id;
	}
} //namespace