#pragma once
#include "ThreeKShell.h"
#define MED_MEGA_TICK	20	//20 mins

namespace medicine {
	#define _MED_
	#include "MedTypes.h"
	#include "MedCures.h"
	#include "MedWounds.h"
	#include "MedSymptoms.h" 
	#include "MedBody.h"
	#undef _MED_

	void _medAfterWoundUpdate(char** oSymptoms);
	void _medOnMedTick(char** oSymptoms);
	char* _medOnKnockout();
	char* _medOnPillConnected(ubyte_t cureId);
	char* _medOnExplosion();
	char* _medNewWound(ubyte_t place);
	void _medSetRegenerationRate(sword_t);
	void _medInit();

}

using namespace medicine;
