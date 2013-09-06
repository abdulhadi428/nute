#include "ArmletApi.h"
#include "ArmletShell.h"
#include "AppMain.h"
#include "Med.h"

namespace medicine {

ArmletApi::FILE MedFile;
BODY Body;

void GatherSymptoms(char* buf, int len)
{
	StrPad(buf,0,0,len);
	int pos=0;

	for (int i=0; i<MaxSymptom; i++) {
		if (Body.Symptom[i]) {
			ArmletApi::snprintf(buf+pos,len-pos,"%s\n",SymptomDesc[i]);
			pos += Length(SymptomDesc[i]);
		}
	}
}

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

		BodyCycle();
	}
}

char buf1[1001];
char buf2[1001];
void _medUpdateStrings(char** Symptoms, char** Diagnostics, char** MedLog)
{
	GatherSymptoms(buf1,1000);

	ArmletApi::snprintf(buf2, sizeof(buf2)-1, 
		"Температура: %d.%d\nДавление: %d/%d\nПульс: %d\n\n", 
		30+Body.Temperature/10,Body.Temperature%10, Body.HighPressure, Body.LowPressure, Body.Pulse);
	//"Температура: 36.6\nПульс:120\n\nСостояние: среднее\nПравая рука:болит\nТошнота\n";

	*MedLog = "Ваша лицензия пользования Индивидуальным Наручным Устройством не поддерживает режим учета медицинских событий. Обратитесь к ближайшему дистрибьютеру!";
	*Diagnostics = buf2; 
	*Symptoms = buf1;

	//FUCK
	//for Temperature & Pulse
	//"В голове у тебя бешено молотит пульс, тело кажется тяжелым и неповоротливым, во рту пересохло.",
    //"Сердце у тебя в груди бьется медленно и тяжело, каждый удар отзывается во всем теле.",
    //"Тебе жарко, одежда душит тебя.",
    //"Тебе холодно, тебя бьет дрожь, зубы стучат.",

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
		if (CureAction[Anesthetics].IsUsing) {
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

char* _medOnExplosion(ubyte_t probability, ubyte_t explosionType)
{
	ubyte_t chance = ArmletApi::GetRandom(100);
	if (chance > probability) {
		return "Ты слышишь сильный грохот. Но вроде бы все обошлось!";
	}

	int ExplosionType;
	if (explosionType==0)
		ExplosionType = ArmletApi::GetRandom(2);
	else
		ExplosionType = explosionType - 1;

	DAMAGE_EFFECT de = WoundToDamageEffect[MaxTarget+1+ExplosionType];
	int Target = ArmletApi::GetRandom(MaxTarget);
	int DamageSeverity = IncreaseCategory(&Body.Part[Target][de].CurrSeverity);

	ApplyWound(MaxTarget+1+ExplosionType,DamageSeverity,&Body.Part[Target][de]);
	const char* msg =  WoundDescs[MaxTarget+1+ExplosionType][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[MaxTarget],
//		DamageSeverity,
//		msg);
	return (char*)msg;
}

char* _medOnKnockout ()
{
	int Target = ArmletApi::GetRandom(MaxTarget);
	int DamageSeverity = IncreaseCategory(&Body.Part[Target][Blow].CurrSeverity);
	ApplyWound(MaxTarget,DamageSeverity,&Body.Part[Target][Blow]);
	const char* msg =  WoundDescs[MaxTarget][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[MaxTarget],
//		DamageSeverity,
//		msg);
	return (char*)msg;
}

char* _medNewWound(ubyte_t place)
{
	DAMAGE_EFFECT de = Rupture;
	if (place<=RightLegTarget)
		de = RuptureLimb;

	int DamageSeverity = IncreaseCategory(&Body.Part[place][de].CurrSeverity);
	ApplyWound(place,DamageSeverity,&Body.Part[place][de]);
	const char* msg =  WoundDescs[place][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[place],
//		DamageSeverity,
//		msg);
//	return _buf;
	return (char*)msg;
}

} //namespace

/*
	char* getBleeding()
	{
		if(bloodCapacity > 160)
			return NoSymptom;
		else if (bloodCapacity > 120)
			return NoSymptom;
		else if (bloodCapacity > 80)
			return NoSymptom;
		else if (bloodCapacity > 40)
			return NoSymptom;
		else if (bloodCapacity > 0)
			return NoSymptom;
		else 
			return DeathTrauma;
	}
*/
