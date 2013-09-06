#pragma once
#ifndef _MED_
	#error _MED_
#endif

#define MaxWoundType	(MaxTarget+1+3)

typedef struct _WOUND_DESC {
	unsigned short target;			//wound subtype
	unsigned short severity;		//wound severity
	DAMAGE_EFFECT de;				//damage effect 
	int PainLevel;				//pain from wound
	int BloodLoss;				//instant bloodloss
	int Bleeding;				//bloodloos per tick
	int ToxinsAdd;				//instant intoxication
	int Toxinating;				//intoxication per tick
	int NecroPoints;			//Amount of "DEAD" flesh
	const char* message;
} WOUND_DESC;

extern WOUND_DESC WoundDescs[MaxWoundType][MaxDamageSeverity];
extern const DAMAGE_SEVERITY	RandomSelectPerTenPercent[10];
extern const DAMAGE_EFFECT WoundToDamageEffect[MaxWoundType];

void InitWounds();
DAMAGE_SEVERITY NextCategory(DAMAGE_SEVERITY* curr);
DAMAGE_SEVERITY DecreaseCategory(DAMAGE_SEVERITY* curr);
DAMAGE_SEVERITY IncreaseCategory(DAMAGE_SEVERITY* curr);
void ApplyWound(int wound,int  ds, struct _PART* part);
