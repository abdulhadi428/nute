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
	InitCureActions();
	InitWounds();
}

char buf1[2*1024];
char buf2[2*1024];
void _medOnMedTick(bool bBreathOnly)
{	
	if (!bBreathOnly) {
		//process cures
		for (int i=0; i<MaxCureId; i++) {
			if ((CureAction[i].IsUsing)&&(CureAction[i].RemainingTicks>0)) {
				if (0==--CureAction[i].RemainingTicks)
					ProcessCureUsage((CURE_ID)i, false);
			}
		}
		//process tortures
		for (int i=0; i<MaxTortureId; i++) {
			if (TortureSecondUsage[i]>0) {
				TortureSecondUsage[i]--;
			}
		}

		BodyCycle(buf1,sizeof(buf1)-1);
	}
}

void _medUpdateStrings(char** Symptoms, char** Diagnostics, char** MedLog)
{
//1100 char max
	GatherDescs(buf1, sizeof(buf1)-1);

	ArmletApi::snprintf(buf2, sizeof(buf2)-1, 
		"Температура: %d.%d\nДавление: %d/%d\nПульс: %d\n\n", 
		30+Body.Temperature/10,Body.Temperature%10, Body.HighPressure, Body.LowPressure, Body.Pulse);
	//"Температура: 36.6\nПульс:120\n\nСостояние: среднее\nПравая рука:болит\nТошнота\n";

	*MedLog = "Ваша лицензия пользования Индивидуальным Наручным Устройством не поддерживает режим учета медицинских событий. Обратитесь к ближайшему дистрибьютеру!";
	*Diagnostics = buf2; 
	*Symptoms = buf1;

	//for Temperature & Pulse
	//"В голове у тебя бешено молотит пульс, тело кажется тяжелым и неповоротливым, во рту пересохло.",
    //"Сердце у тебя в груди бьется медленно и тяжело, каждый удар отзывается во всем теле.",
    //"Тебе жарко, одежда душит тебя.",
    //"Тебе холодно, тебя бьет дрожь, зубы стучат.",

//show all symptoms
//show diagnostics
//DROP med log - 
//	

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
}

//simple healing
char* _medOnPillConnected(ubyte_t pill_id)
{
	if ((pill_id>=20)&&(pill_id<=26))
	{
		TORTURE_ID torture_id = (TORTURE_ID)(pill_id-20);
		ProcessTortureUsage(torture_id);
		if (CureSideEffect.AnestheticsPainReduction) {
			return (char*)TortureDesc[torture_id].EffectNoPain;
		} else {
			return (char*)TortureDesc[torture_id].Effect;
		}
	}

	if ((pill_id>=0)&&(pill_id<=15))
	{
		CURE_ID cure_id = (CURE_ID)pill_id;
		ProcessCureUsage(cure_id, true);
		return (char*)CureDesc[cure_id].Effect;
	}

	return "Неизвестно, что сделал этот препарат с тобой!";
}

#pragma region forUI
//temp
//char _buf[201];

char* _medOnExplosion(ubyte_t probability, ubyte_t explosionType)
{
	ubyte_t chance = ArmletApi::GetRandom(100);
	if (chance > probability) {
		return "Ты слышишь сильный грохот. Но вроде бы все обошлось!";
	}

	int ExplosionType;
	int Target = ArmletApi::GetRandom(MaxTarget);
	int DamageSeverity = IncreaseCategory(&Body.parts[Target].CurrSeverity);	//TODO FIX

	if (explosionType==0)
		ExplosionType = ArmletApi::GetRandom(2);
	else
		ExplosionType = explosionType - 1;

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