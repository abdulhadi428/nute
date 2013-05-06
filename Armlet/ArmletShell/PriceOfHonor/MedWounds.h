#pragma once
#ifndef _MED_
	#error _MED_
#endif

#define MaxWoundType	(MaxTarget+1+MaxExplosionEffect)

typedef struct _WOUND_DESC {
	unsigned short target;			//wound subtype
	unsigned short severity;		//wound severity
	PAIN_LEVEL PainLevel;
	DISFUNCTION_LEVEL DisfnLevel;
	int BloodLoss;
	int Bleeding;
	int ToxinsAdd;
	int Toxinating;
	const char* message;
} WOUND_DESC;

extern WOUND_DESC WoundDescs[MaxWoundType][MaxDamageSeverity];
extern const DAMAGE_SEVERITY	RandomSelectPerTenPercent[10];
//extern const DAMAGE_SEVERITY	

void InitWounds();
DAMAGE_SEVERITY NextCategory(DAMAGE_SEVERITY* curr);
DAMAGE_SEVERITY DecreaseCategory(DAMAGE_SEVERITY* curr);
DAMAGE_SEVERITY IncreaseCategory(DAMAGE_SEVERITY* curr);
void ApplyWound(int wound,int  ds, struct _PART* part);
