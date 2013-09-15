#include "ArmletApi.h"
#include "ArmletAppSDK.h"
#include "Med.h"

namespace medicine {

	const char* TargetNames[MaxWoundType] = {
		"левая рука",
		"правая рука",
		"левая нога",
		"правая нога",
		"грудь",
		"живот",
		"спина",
		"голова",
		"нокаут",
		"волна",
		"ожог",
		"радиация"
	};

	const int BodyStructure[MaxTarget][MaxOrganType] = {						
		{	0,	1,	2,	2	}, //LA
		{	0,	1,	2,	2	}, //RA
		{	0,	1,	2,	2	}, //LL
		{	0,	1,	2,	2	}, //RL
		{	0,	1,	4,	5	}, //Chest
		{	0,	1,	6,	7	}, //Abdomen
		{	0,	2,	8,	9	}, //Back
		{	0,	2,	1,	3	}  //Head
	};	

/*
	const SYMPTOM SymptomsByBodyStructure[MaxTarget*MaxOrganType][2] = 
	{
{NoSymptom,NoSymptom},
{HandWeak1,HandWeak2},
{Pain2,BoneBreak},
{PulsePain,BloodLossInner},
{NoSymptom,NoSymptom},
{HandWeak1,HandWeak2},
{Pain2,BoneBreak},
{PulsePain,BloodLossInner},
{NoSymptom,NoSymptom},
{LegWeak1,HandWeak2},
{Pain2,BoneBreak},
{PulsePain,BloodLossInner},
{NoSymptom,NoSymptom},
{LegWeak1,LegWeak2},
{Pain2,BoneBreak},
{PulsePain,BloodLossInner},
{NoSymptom,NoSymptom},
{SharpPain,BonePartPain},
{CoughStrong,BlodThroat},
{HardBreathe,Infarct},
{NoSymptom,NoSymptom},
{GeneralWeak,BloodLossInner},
{Puke,FunnyStrong},
{BackPain1,Spasm},
{NoSymptom,NoSymptom},
{BackPain2,TorsoParalyze},
{BloodLossMed,BloodLossInner},
{Pain2,ButtonFail},
{NoSymptom,NoSymptom},
{HeadShake1,FunnyStrong},
{NeckParalyze,Faint},
{ConcinuousMess,Insult}
	};

	COMPILE_TIME_CHECK(sizeof(SymptomsByBodyStructure)/sizeof(SYMPTOM)==MaxTarget*MaxOrganType*2);
*/

	const SYMPTOM SymptomsByPart[MaxTarget][2] = 
	{
{HandWeak1,HandWeak2},
{HandWeak1,HandWeak2},
{LegWeak1,HandWeak2},
{LegWeak1,LegWeak2},
{HardBreathe,CoughStrong},
{Puke,BloodLossInner},
{PulsePain,TorsoParalyze},
{ConcinuousMess,Unconciuous}
	};
	COMPILE_TIME_CHECK(sizeof(SymptomsByPart)/sizeof(SYMPTOM)==MaxTarget*2);

	SYMPTOM getDisfnSymptomByTargetOrgan(TARGET part, PART* pPart)
	{
		return SymptomsByPart[part][pPart->DisfnLevel];
	}

	SYMPTOM getBloodLossSymptom(ubyte_t bloodCapacity)
	{
		if(bloodCapacity > 160/*000*/)
			return NoSymptom;
		else if (bloodCapacity > 120/*000*/)
			return BloodLossLight;
		else if (bloodCapacity > 80/*000*/)
			return BloodLossMed;
		else if (bloodCapacity > 40/*000*/)
			return BloodLossStrong;
		else if (bloodCapacity > 0)
			return BloodLossCritical;
		else 
			return DeathTrauma;
	}

	void InitBody()
	{
		StrPad((char*)&Body,0,0,sizeof(BODY));
		Body.BloodCapacity = 200/*000*/;
		Body.BleedingFactor = 1;
		Body.PainReduction = 0;
		Body.RegenerationLevel = RegenLow;
		for (int i=0; i < MaxTarget; i++) {
			Body.parts[i].Bleeding = 0;
			Body.parts[i].DisfnLevel = NoDisfn;
			Body.parts[i].PainLevel = NoPain;
			Body.parts[i].CurrSeverity = None;
		}
		//Body.ToxinsCapacity = 5000;
	}

	void GatherDescs(char* buf, int len)
	{
		StrPad(buf,0,0,len);
		int plus;
			SYMPTOM sym = getBloodLossSymptom(Body.BloodCapacity);
			if (sym!=-1) {
				char* str = (char*)SymptomEffects[sym];
				plus = ArmletApi::snprintf(buf,len,"%s\n",str);
				buf += plus; len-=plus;
			}

		for (int i=0; i<MaxTarget; i++) {
			PPART part = &Body.parts[i];
			DISFUNCTION_LEVEL lvl = part->DisfnLevel;
			if ((lvl==1)||(lvl==2)) {
				sym = getDisfnSymptomByTargetOrgan((TARGET)i,part);
				if (sym!=-1) {
					char* str = (char*)SymptomEffects[sym];
					plus = ArmletApi::snprintf(buf,len,"%s\n",str);
					buf += plus; len-=plus;
				}
			}
		}
	}

	void BodyCycle(char* buf, int len) //TODO -inline
	{
		GatherDescs(buf,len);
		/*
		//Get Oxygen
		Body->OxygenDelivered = 50000;	//TODO modify by trauma
		//Oxygen delivery
		if (Body->OxygenDelivered > Body->BloodCapacity) {
			Body->OxygenDelivered = Body->BloodCapacity;
		}
		//Toxins delivery
		Body->ToxinsDelivered = Body->ToxinsCapacity;
		if (Body->ToxinsDelivered > Body->BloodCapacity / 2)
			Body->ToxinsDelivered = Body->BloodCapacity / 2;
		Body->ToxinsCapacity -= Body->ToxinsDelivered;
		Body->OxygenDelivered -= Body->ToxinsDelivered;
		//Organ Cycle
		Body->ToxinsCapacity -= Body->ToxinsDelivered;
		//
		*/
//		int plus;
//		plus = ArmletApi::snprintf(buf,len,"%s=%d\n","BloodCapacity",Body.BloodCapacity);
//		buf += plus; len-=plus;
		for (int i=0; i<MaxTarget; i++) {
			PPART part = &Body.parts[i];
			Body.BloodCapacity -= 
				part->Bleeding / Body.BleedingFactor;
			if (Body.BloodCapacity < 0)
				Body.BloodCapacity = 0;
			
			part->RemainingTicks--;
			if (part->RemainingTicks == 0) {
				part->RemainingTicks = MED_MEGA_TICK;
				NextCategory(&part->CurrSeverity);
			}

		}
	}

	void BodyDecreaseCategory()
	{
		for (int i=0; i<MaxTarget; i++) {
			DecreaseCategory(&Body.parts[i].CurrSeverity);
			if (Body.parts[i].DisfnLevel > 0)
				Body.parts[i].DisfnLevel = (DISFUNCTION_LEVEL)(Body.parts[i].DisfnLevel - 1);
			if (Body.parts[i].PainLevel > 0)
				Body.parts[i].PainLevel =  (PAIN_LEVEL)(Body.parts[i].PainLevel-1);
		}
	}

}

//FEATURE CUT
/*
int DisfnFactor(DISFUNCTION_LEVEL disfn_level)
{
	return 1;
}

void OrganCycle(PBODY Body, PORGAN Organ)	//TODO remove inline
{
	//Bleeding; B-=X
	Body->BloodCapacity -= Organ->Bleeding*Body->BleedingFactor;
	//Blood regeneration; B+=X						//TODO modify by trauma/disfn
	Body->BloodCapacity += Organ->DefBloodRegen*DisfnFactor(Organ->DisfnLevel)*Body->BloodRegenFactor;
	//Toxication internal; P+=X
	Organ->PoisonCapacity += Organ->Toxinating*Body->ToxinatingFactor;
	//Metabolism; CC += X; OO-=X					//TODO modify by trauma/disfn
	if (Body->OxygenDelivered > Organ->DefOxygenUse) {
		Body->CO2_Delivered += Organ->DefOxygenUse;
		Body->OxygenDelivered -= Organ->DefOxygenUse;
	} else { //no OO poison check
		Organ->PoisonCapacity += (Organ->DefOxygenUse - Body->OxygenDelivered);
		Body->CO2_Delivered += Body->OxygenDelivered;
		Body->OxygenDelivered = 0;
	}
	//Detoxication									//TODO modify by trauma/disfn		
	if (Body->ToxinsDelivered > 0) {
		Body->ToxinsDelivered -= Organ->DefDetox;
	}
	if (Body->ToxinsDelivered < 0) {
		Body->ToxinsDelivered = 0;
	}
	//Restore										//TODO modify by cures/truma/disfn
	if (Organ->Bleeding > Organ->DefRestore) {
		Organ->Bleeding -= Organ->DefRestore;
		Organ->PoisonCapacity += Organ->DefRestore*100; //TODO
	} else {
		//...
	}
	if (Organ->Toxinating > Organ->DefRestore) {
		Organ->Toxinating -= Organ->DefRestore;
		Organ->PoisonCapacity += Organ->DefRestore*100; //TODO
	} else {
		//...
	}
}

void OrganCycle2() //TODO -inline
{
	//Unpoisoning
	//if (Body->Toxin
	//Unpoisoning
	//if (Organ->PosionCapacity < Body->OxygenDelivered
}
*/
/*
Poisoning A		if (T>B) P++			
Unpoisoning		P--			
PoisonCheck		if (P==20) P==0; IncreaseDisfunction			
				if (P==-20) P=0; DecreaseDisfunction			
*/

//ArmletApi::snprintf(buf1,100,"12345678901234567890123456");