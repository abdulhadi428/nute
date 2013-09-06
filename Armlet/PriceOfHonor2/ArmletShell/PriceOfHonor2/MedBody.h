#pragma once
#ifndef _MED_
	#error _MED_
#endif

//extern const char* TargetNames[MaxWoundType];

typedef struct _PART {
	int wound;
	DAMAGE_SEVERITY CurrSeverity;		//stored by damage effect
	int RemainingTicks;
	//From damage effect
	int PainLevel;
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
//CALCULATED PARAMETERS
	int PainLevel;
	bool Feeling[MaxFeeling];
	bool Symptom[MaxSymptom];

	bool HasInsidious;
	bool HasSerious;
	bool HasCritical;
//PARTS
	PART Part[MaxTarget][MaxDamageEffect];
} BODY, *PBODY;

extern BODY Body;

void InitBody();
void BodyCycle();

void IncreaseBloodCapacity(int val, bool bReduceToxinsCapacity);
void DecreaseBloodCapacity(int val, bool bIncreaseToxinsCapacity);
void DecreaseToxinsCapacity(int val);
void IncreaseToxinsCapacity(int val);

void IncreaseThermal();
void DecreaseThermal();
void DecreaseRadiation();

void DecreaseNecropoints(CURE_ID cure_id);