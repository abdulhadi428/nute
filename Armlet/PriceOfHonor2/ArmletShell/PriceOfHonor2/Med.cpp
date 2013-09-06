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

	if ((Body.PainLevel>=Pain2)&&(Body.PainLevel<=Pain4)) {
		ArmletApi::snprintf(buf+pos,len-pos,"%s\n",PainDesc[Body.PainLevel]);
		pos += Length(PainDesc[Body.PainLevel])+1;
	}

}

void GatherDiagnostics(char* buf, int len)
{
	StrPad(buf,0,0,len);
	int pos=0;

	ArmletApi::snprintf(buf+pos,len-pos,"Давление: %d/%d\n",Body.HighPressure,Body.LowPressure);
	pos = Length(buf);
	ArmletApi::snprintf(buf+pos,len-pos,"Температура: %d.%d\n",30+Body.Temperature/10,Body.Temperature%10);
	pos = Length(buf);
    //"Тебе жарко, одежда душит тебя.",
    //"Тебе холодно, тебя бьет дрожь, зубы стучат.",
	ArmletApi::snprintf(buf+pos,len-pos,"Пульс: %d\n\n",Body.Pulse);
	pos = Length(buf);
	//"В голове у тебя бешено молотит пульс, тело кажется тяжелым и неповоротливым, во рту пересохло.",
    //"Сердце у тебя в груди бьется медленно и тяжело, каждый удар отзывается во всем теле.",
			
	if ((Body.VisibleBleeding>0)&&(Body.VisibleBleeding<=3)) {
		ArmletApi::snprintf(buf+pos,len-pos,"%s\n\n",BleedingDesc[Body.VisibleBleeding]);
		pos += Length(BleedingDesc[Body.VisibleBleeding])+2;
	}

	//no feelings with theese symptoms
	if ((Body.Symptom[BreathStop])||(Body.Symptom[Unconciuous])||(Body.Symptom[DeathTrauma])) {
		return;
	}

	if ((Body.PainLevel>NoPain)&&(Body.PainLevel<=PainShock)) {
		ArmletApi::snprintf(buf+pos,len-pos,"%s\n",PainDesc[Body.PainLevel]);
		pos += Length(PainDesc[Body.PainLevel])+1;
	}

	for (int i=0; i<MaxFeeling; i++) {
		if (Body.Feeling[i]) {
			ArmletApi::snprintf(buf+pos,len-pos,"%s\n",FeelingDesc[i]);
			pos += Length(FeelingDesc[i])+1;
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
	GatherDiagnostics(buf2,1000);

	*MedLog = "Ваша лицензия пользования Индивидуальным Наручным Устройством не поддерживает режим учета медицинских событий.";
	*Diagnostics = buf2; 
	*Symptoms = buf1;

	//save
	ArmletApi::WriteFile(&MedFile,(char*)&Body,sizeof(BODY));

	//to server
	ubyte_t state[5];
	state[0] = (Body.BloodCapacity);	//0..200
	state[1] = (Body.ToxinsCapacity);	//0..200
	state[2] = (Body.Pulse);			//40..150
	state[3] = (Body.Temperature);		//0..150 i.e. 30-45
	state[4] = 0;
	ArmletApi::SendAppState(state);
}

//cures and tortures
char* _medOnPillConnected(ubyte_t pill_id)
{
	if ((pill_id>=20)&&(pill_id<=26))
	{
		TORTURE_ID torture_id = (TORTURE_ID)(pill_id-20);
		ProcessTortureUsage(torture_id);
		//was torture blocked?
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

	int Target = ArmletApi::GetRandom(MaxTarget);
	DAMAGE_EFFECT de = WoundToDamageEffect[MaxTarget+1+ExplosionType];
	PPART part = &Body.Part[Target][de];
	int DamageSeverity = IncreaseCategory(part->CurrSeverity);
	ApplyWound(MaxTarget+1+ExplosionType,DamageSeverity,part,true);
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
	PPART part = &Body.Part[Target][Blow];
	int DamageSeverity = IncreaseCategory(part->CurrSeverity);
	ApplyWound(MaxTarget,DamageSeverity,part,true);
	const char* msg =  WoundDescs[MaxTarget][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[MaxTarget],
//		DamageSeverity,
//		msg);
	return (char*)msg;
}

char* _medNewWound(ubyte_t Target)
{
	DAMAGE_EFFECT de = Rupture;
	if (Target<=RightLegTarget)
		de = RuptureLimb;

	PPART part = &Body.Part[Target][de];
	int DamageSeverity = IncreaseCategory(part->CurrSeverity);
	ApplyWound(Target,DamageSeverity,part,true);
	const char* msg =  WoundDescs[Target][DamageSeverity].message;
//temp
//	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
//		TargetNames[place],
//		DamageSeverity,
//		msg);
//	return _buf;
	return (char*)msg;
}

} //namespace
