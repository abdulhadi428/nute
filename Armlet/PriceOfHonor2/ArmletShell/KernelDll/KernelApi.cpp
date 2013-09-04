#include "ArmletKernel.h"

namespace KernelApi {
	__LOWVAR unsigned short VideoMemory[SCREENX*SCREENY];

	__LOWVAR VIBRO_CALLBACK* Vibro = 0;
	__LOWVAR LOG_CALLBACK* Log = 0;
	__LOWVAR SET_CURE_NAME_CALLBACK* SetCureName = 0;
	__LOWVAR UPDATE_CURRENT_CURE_CALLBACK* UpdateCurrentCure = 0;
	__LOWVAR UPDATE_CURRENT_TORTURE_CALLBACK* UpdateCurrentTorture = 0;


	int __LOWCALL GetPillCharges(int cure_id)
	{
		if ((cure_id < 0)||(cure_id > 14)) {
			if ((cure_id < 20)||(cure_id > 26))
				return 20;
			return ArmletKernel::TortureCharges[cure_id-20];
		}
		return ArmletKernel::CureCharges[cure_id];
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