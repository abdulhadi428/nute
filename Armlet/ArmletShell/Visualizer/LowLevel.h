#pragma unmanaged
#include "DownInterface.h"
#include "ArmletApi.h"

#define UNREFERENCED_PARAMETER(P)	(P)
#define COMPILE_TIME_CHECK(e) extern char _COMPILE_TIME_CHECK_[(e)?1:-1]

namespace LowLevel {

	//define functions that are used between DownInterface.cpp and ArmletApi.cpp
	//but are not required for FormHelper.h / MainForm.h
	void __cdecl LowLog(char* fmt,...);
	
	//define variables that are used between DownInterface.cpp and ArmletApi.cpp
	//
	//variables that are required by managed code should be declared in DownInterface.h
	//but functions are more preferrable (BUT define them in LowLevel.cpp)
	//
	//DownInterface.h variables:
	//	VideoMemory 

	//ARMLET emulation GetLustraId
	//DownInterface LustraInRange
	extern bool bLustraInRange;
	extern unsigned char LastLustraId;
	//DownInterface SetCurrentLock
	extern unsigned char CurrentLockId;
	//ARMLET emulation WritePill
	//DownInterface GetPillCharges
	extern int CureCharges[15];
	extern int TortureCharges[7];
	extern short NoncheId;

}