#pragma once
#ifndef _MED_
	#error _MED_
#endif

//extern const char* TargetNames[MaxWoundType];

/*
	//From organ initializer
	int DefOxygenUse;					//O-=X, per tick		//метаболизм
	int DefDetox;						//T-=X, per tick		//детоксикация
	int DefBloodRegen;					//B+=X, per tick		//регенерация крови
	int DefRestore;						//P-=X, per tick		//восстановление

	//Local toxin capacity
	int PoisonCapacity;					//NECRO POINTS

	//Traumas[]
	int TraumaPainLevel;
*/

typedef struct _PART {
	DAMAGE_SEVERITY CurrSeverity;//TODO FIX [MaxDamageEffect];	//stored by damage effect
	//From damage effect
	int PainLevel;
	int RemainingTicks;
	int Bleeding;						//B-=X, per tick		//кровотечение
	int Toxinating;						//T+=X, per tick		//интоксикация
	int NecroPoints;
} PART, *PPART;

typedef struct _BODY {
//DIAG PARAMETERS
	REGENERATION_LEVEL RegenerationLevel;	//R, hidden 1..3;0 if NanoExoFrame
	int HighPressure;	//diag	//120-105-90-75-60	//200-150-100-50-0
	int LowPressure;	//diag	// 80- 70-60-50-40	//200-150-100-50-0
	int Temperature;	//diag	// 30-45, showed as 30+T/10	T=0..150
	int Pulse;			//diag	//40-59 замедленный, 60-80 норма, 81-150 повышенный	
								//  norm is (50+10*R)
								//	чем выше метаболизм (например жар) тем ниже
								//	чем меньше кислорода (напримр интоксикация) тем выше
	int VisibleBleeding;//diag	//0..3 - no,low,med,high
//MAIN PARAMETERS
	int BloodCapacity;						//B
	int ToxinsCapacity;						//T
	//int ToxinsDelivered;					//TT = min(B/2,T)
	//int OxygenDelivered;					//OO = min(B-TT,O)
	//int CO2_Delivered;						//CC
	int PainReduction;
	//bool BodySymptoms[MaxSymptom];
	//Factors
	int BleedingFactor;
	//int BloodRegenFactor;
	//int ToxinatingFactor;
	PART parts[MaxTarget];
} BODY, *PBODY; //TODO MOVE

//Target by severity/damage
//	int BloodLoss;						//B-=X, once			//кровопотеря
//	int Toxins;							//T-=X, once			//токины	

extern BODY Body;

void InitBody();
void BodyCycle(char* buf, int len);
void BodyDecreaseCategory();
void GatherDescs(char* buf, int len);

void IncreaseBloodCapacity(int val, bool bReduceToxinsCapacity);