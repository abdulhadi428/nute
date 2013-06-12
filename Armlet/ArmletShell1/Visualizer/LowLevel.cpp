#include "stdafx.h"
#include "LowLevel.h"

namespace LowLevel {

	//DownInterface.h
	unsigned short VideoMemory[SCREENX*SCREENY];

	//LowLevel.h
	bool bLustraInRange = false;
	unsigned char LastLustraId = UNKNOWN_ID;
	unsigned char CurrentLockId = UNKNOWN_ID;
	int CureCharges[15] = {
		20,20,20,20,20,
		20,20,20,20,20,
		20,20,20,20,20
	};
	short NoncheId = 1000;

} //namespace
