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

	void _medInit();
	void _medSetRegenerationRate(sword_t);

	//return 1 strings - into msg box (or into wound form)
	char* _medNewWound(ubyte_t place);
	char* _medOnKnockout();
	char* _medOnExplosion(ubyte_t probability, ubyte_t explosionType);//(ubyte_t power);
	char* _medOnPillConnected(ubyte_t cureId);

	void _medOnMedTick();
	
	//called after previous 4 events and on med tick
	//1100 char max
	void _medUpdateStrings(char** Symptoms, char** Diagnostics, char** MedLog);
}

using namespace medicine;
