#include "ArmletApi.h"
#include "ArmletShell.h"
#include "AppMain.h"
//#include "UserInterface.h"
#include "Med.h"

//extern UserInterface UI;

namespace medicine {

ArmletApi::FILE MedFile;
BODY Body;

void _medSetRegenerationRate(sword_t regenRate)
{
	if ((regenRate<1)||(regenRate>3)) return;
	Body.RegenerationLevel = (REGENERATION_LEVEL)regenRate;
	return;
}

void _medInit()
{
	int sz = 0;
	bool ret = ArmletApi::OpenFile(&MedFile, "MedState.bin",false);
	if (ret) {
		sz = ArmletApi::ReadFile(&MedFile,(char*)&Body,sizeof(BODY));
	} else {
		ArmletApi::OpenFile(&MedFile, "MedState.bin",true);
	}
	if (sz!=sizeof(BODY)) {
		InitBody();
	}
	InitCureAndTortureDescs();
	InitWounds();
}

char buf1[6*1024];
void _medOnMedTick()
{	
	//save
	ArmletApi::WriteFile(&MedFile,(char*)&Body,sizeof(BODY));

	//To server
	ubyte_t state[5];
	state[0] = (Body.BloodCapacity);	//0..200
	state[1] = (Body.ToxinsCapacity);	//0..200
	state[2] = (Body.Pulse);			//40..150
	state[3] = (Body.Temperature);		//0..150 i.e. 30-45
	state[4] = 0;
	ArmletApi::SendAppState(state);

	BodyCycle(buf1,sizeof(buf1)-1);
	//*oSymptoms=buf1;
}

void _medAfterWoundUpdate(char** MedLog, char** Diagnostics, char** Symptoms)
{
	GatherDescs(buf1, sizeof(buf1)-1);
	*MedLog = buf1;
	*Diagnostics = buf1;
	*Symptoms = buf1;
}

//simple healing
char* _medOnPillConnected(ubyte_t cureId)
{
	if ((cureId>=20)&&(cureId<=26))
	{
		TORTURE_ID torture_id = (TORTURE_ID)(cureId-20);
		switch  (torture_id) {
		default:
			break;
		}
		if (Body.PainReduction==2) {
			return (char*)TortureDesc[torture_id].EffectNoPain;
		} else {
			return (char*)TortureDesc[torture_id].Effect;
		}
	}

	CURE_ID cure_id = (CURE_ID)cureId;
	switch (cureId) {
		case Analgetic:
			Body.PainReduction = 1;
			break;
		case Antispasmodic:
			//block spasms
			//foreach organ Muscle disfuntion +
			break;
		case Pyretic:
			//perception+, temperature-
			//bleeding+
			break;
		case Aspirator:
			//can breath even if NoBreaath
			//chest disfunction
			break;
		case CoagulationFactor:
			//bleeding-
			//toxication+
			//TT=B/4
			Body.BleedingFactor = 3;
			break;
		case SyntheticBlood:
			Body.BloodCapacity += Body.RegenerationLevel * 10; //depends on regen
			break;
		case Leatherette:
		case Myorelaxant:
		case Antibiotic:
		case NanoExoFrame:
			//FEATURE CUT
			break;
		case VisceraNanoPack:
			BodyDecreaseCategory();
			break;
		case Anesthetics:
			Body.PainReduction = 2;
			//Def
			break;
		case Absorber:
			//ToxCapacity-
			break;
		case PlasterNanoPack:
			//smth- smth+
			break;
		case MagicCure:
			InitBody();
			break;
		default:
			break;
	}
	return (char*)CureDesc[cureId].Effect;
}

#pragma region forUI
//temp
//char _buf[201];

char* _medOnExplosion(ubyte_t probability, ubyte_t explosionType)
{
	int Target = ArmletApi::GetRandom(MaxTarget);
	int DamageSeverity = IncreaseCategory(&Body.parts[Target].CurrSeverity);	//TODO FIX
	int ExplosionType = ArmletApi::GetRandom(2);
	if (ExplosionType == 1)
		ExplosionType = 2; //skip scorch
	ApplyWound(MaxTarget+1+ExplosionType,DamageSeverity,&Body.parts[Target]);
	const char* msg =  WoundDescs[MaxTarget+1+ExplosionType][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[MaxTarget+1+ExplosionType],
//		DamageSeverity,
//		msg);
//	return _buf;
	return (char*)msg;
}

char* _medOnKnockout ()
{
	int Target = ArmletApi::GetRandom(MaxTarget);
	int DamageSeverity = IncreaseCategory(&Body.parts[Target].CurrSeverity);	//TODO FIX
	ApplyWound(MaxTarget,DamageSeverity,&Body.parts[Target]);
	const char* msg =  WoundDescs[MaxTarget][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[MaxTarget],
//		DamageSeverity,
//		msg);
//	return _buf;
	return (char*)msg;
}

char* _medNewWound(ubyte_t place)
{
	int DamageSeverity = IncreaseCategory(&Body.parts[place].CurrSeverity);	//TODO FIX
	ApplyWound(place,DamageSeverity,&Body.parts[place]);
	const char* msg =  WoundDescs[place][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[place],
//		DamageSeverity,
//		msg);
//	return _buf;
	return (char*)msg;
}
#pragma endregion

} //namespace

//a) BLOOD_CAPACITY_SYMPTOM
//d) process megatick (20 ticks)
//f) generate string for all symptoms