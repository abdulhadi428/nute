#include "ArmletApi.h"
#include "ArmletShell.h"
#include "Med.h"

namespace medicine {

//temp
/*
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
*/

	void InitPart(PART* Part)
	{
		Part->CurrSeverity = None;
		Part->RemainingTicks = 0;
		Part->PainLevel = 0;
		Part->Bleeding = 0;
		Part->Toxinating = 0;
		Part->NecroPoints = 0;
	}

	void InitBody()
	{
		StrPad((char*)&Body,0,0,sizeof(BODY));
//DIAG PARAMETERS
		Body.RegenerationLevel = RegenLow;
		Body.HighPressure = 120;				
		Body.LowPressure = 80;					
		Body.Temperature = 66;
		Body.Pulse = 60;
		Body.VisibleBleeding = 0;
//MAIN PARAMETERS
		Body.BloodCapacity = 200;
		Body.ToxinsCapacity = 5;

		Body.MaxPain = 0;
		for (int i=0; i < MaxSymptom; i++)
			Body.Symptom[i] = false;
		for (int i=0; i < MaxTarget; i++) {
		for (int j=0; j < MaxDamageEffect; j++) {
			InitPart(&Body.Part[i][j]);
		}}
	}

	int GetPartPain(int pain, DAMAGE_EFFECT de, DAMAGE_SEVERITY ds)
	{
		if (CureAction[Anesthetics].IsUsing) {
			pain -= 2;
			if (pain < 0)
				pain = 0;
			return pain;
		}
		if (CureAction[Analgetic].IsUsing) {
			if (ds == Insidious) 
				Body.Symptom[Hallucination] = true;
			if ((de == Thermal)||(de == Radiation)) {
				return pain;
			}
			pain -= 1;
			if (pain < 0)
				pain = 0;
			return pain;
		}
	}

	void BodyCycle()
	{
		for (int i=0; i<MaxTarget; i++) {
		for (int j=0; j < MaxDamageEffect; j++)
		{
			PPART part = &Body.Part[i][j];
			
			Body.BloodCapacity -= part->Bleeding;
			Body.ToxinsCapacity += part->Toxinating;

			int pain = GetPartPain(part->PainLevel,(DAMAGE_EFFECT)j,part->CurrSeverity);
			if (Body.MaxPain < pain)
				Body.MaxPain = pain;

			if (Body.BloodCapacity < 0)
				Body.BloodCapacity = 0;
			
			part->RemainingTicks--;
			if (part->RemainingTicks == 0) {
				part->RemainingTicks = MED_MEGA_TICK;
				NextCategory(&part->CurrSeverity);
			}
		}}
	}

	void BodyDecreaseCategory()
	{
		for (int i=0; i<MaxTarget; i++) {
		for (int j=0; j<MaxDamageEffect; j++) {
			DecreaseCategory(&Body.Part[i][j].CurrSeverity);
			if (Body.Part[i][j].PainLevel > 0)
				Body.Part[i][j].PainLevel =  (Body.Part[i][j].PainLevel-1);
		}}
	}

}

void IncreaseBloodCapacity(int val, bool bReduceToxinsCapacity);
void DecreaseBloodCapacity(int val, bool bIncreaseToxinsCapacity);
void DecreaseToxinsCapacity(int val);

void IncreaseThermal();
void DecreaseThermal();
void DecreaseRadiation();
bool HaveSeriousOrCritical();

void DecreaseNecropoints(CURE_ID cure_id);

//FEATURE CUT
/*

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

namespace medicine {

void IncreaseBloodCapacity(int val, bool bReduceToxinsCapacity)
{
	Body.BloodCapacity += val;
	if (Body.BloodCapacity >= 200) {
		val-= (Body.BloodCapacity-200);
		Body.BloodCapacity = 200;
	}
	Body.ToxinsCapacity -= val;
	if (Body.ToxinsCapacity < 0) {
		Body.ToxinsCapacity = 0;
	}
}

void DecreaseBloodCapacity(int val, bool bIncreaseToxinsCapacity)
{
}

void DecreaseToxinsCapacity(int val)
{
}

void IncreaseThermal()
{
}

void DecreaseThermal()
{
}

void DecreaseRadiation()
{
}

bool HaveSeriousOrCritical()
{
	return true;
}

void DecreaseNecropoints(CURE_ID cure_id)
{
}

}