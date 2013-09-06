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
		Part->wound = UnknownTarget;
		Part->CurrSeverity = None;
		Part->RemainingTicks = 0;
		Part->PainLevel = 0;
		Part->Bleeding = 0;
		Part->Toxinating = 0;
		Part->NecroPoints = 0;
	}

	void InitBody()
	{
		TickFromFirstWound = -1;
		HackRegsurplus = 0;

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

		Body.PainLevel = 0;
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
			if ((de == Thermal)||(de == Radiation)) {
				return pain;
			}
			pain -= 1;
			if (pain < 0)
				pain = 0;
			return pain;
		}
		return pain;
	}

	void IncreaseBloodCapacity(int val, bool bReduceToxinsCapacity)
	{
		Body.BloodCapacity += val;
		if (Body.BloodCapacity >= 200) {
			val-= (Body.BloodCapacity-200);
			Body.BloodCapacity = 200;
		}
		if (bReduceToxinsCapacity)
			DecreaseToxinsCapacity(val);
	}

	void DecreaseBloodCapacity(int val, bool bIncreaseToxinsCapacity)
	{
			Body.BloodCapacity -= val;
			if (Body.BloodCapacity <= 0) {
				Body.BloodCapacity = 0;
				Body.Symptom[DeathTrauma] = true;
				return;
			}
			if (bIncreaseToxinsCapacity) {
				IncreaseToxinsCapacity(val);
			}
	}

	void DecreaseToxinsCapacity(int val)
	{
		Body.ToxinsCapacity -= val;
		if (Body.ToxinsCapacity < 0) {
			Body.ToxinsCapacity = 0;
		}
		if (Body.BloodCapacity - Body.ToxinsCapacity > 0) {
				Body.Symptom[Unconciuous] = false;
		}
	}

	void IncreaseToxinsCapacity(int val)
	{
		Body.ToxinsCapacity += val;
		if (Body.BloodCapacity - Body.ToxinsCapacity < 0) {
				Body.Symptom[Unconciuous] = true;
		}
	}

	void BodyCycle()
	{
		int TotalBleeding = 0;

		Body.HasInsidious = false;
		Body.HasSerious = false;
		Body.HasCritical = false;

		//for (int i=0; i < MaxSymptom; i++)
		//	Body.Symptom[i] = false;
		//for (int i=0; i < MaxFeeling; i++)
		//	Body.Feeling[i] = false;

		for (int i=0; i<MaxTarget; i++) {
		for (int j=0; j < MaxDamageEffect; j++)
		{
			PPART part = &Body.Part[i][j];
		
			if (part->CurrSeverity == Insidious) 
				Body.HasInsidious = true;
			if (part->CurrSeverity == Serious) 
				Body.HasSerious = true;
			if (part->CurrSeverity == Critical) 
				Body.HasCritical = true;

			int pain = GetPartPain(part->PainLevel,(DAMAGE_EFFECT)j,part->CurrSeverity);
			if (Body.PainLevel < pain)
				Body.PainLevel = pain;

			TotalBleeding += part->Bleeding;
			if (!CureAction[CoagulationFactor].IsUsing)
				DecreaseBloodCapacity(part->Bleeding,false);
			IncreaseToxinsCapacity(part->Toxinating); //FUCK - what should block this?

			//FUCK process NecroPoint
		
			part->RemainingTicks--;
			if (part->RemainingTicks == 0) {
				part->RemainingTicks = MED_MEGA_TICK;				
				DAMAGE_SEVERITY ds = NextCategory(part->CurrSeverity);
				ApplyWound(part->wound,ds,part);
			}

			//boxer pose
			if ((part->wound == Thermal)&&(part->CurrSeverity >= Serious)&&(!CureAction[Myorelaxant].IsUsing)) {
				Body.Symptom[BoxerPose] = true;
			} else {
				Body.Symptom[BoxerPose] = false;
			}
			//can breath
			if (CureAction[Aspirator].IsUsing)
				Body.Symptom[BreathStop] = false;
		}}

		//process pressures
		int PressureAdjust = 
			CureAction[PlasterNanoPack].CumulativeValue
			- CureAction[Absorber].CumulativeValue
			- CureAction[Anesthetics].CumulativeValue;
		//process temperature
		int TemperatureAdjust = 
			CureAction[VisceraNanoPack].CumulativeValue
			+ CureAction[SyntheticBlood].CumulativeValue
			- CureAction[Pyretic].CumulativeValue;
		//process pulse
		//TotalBleeding => VisibleBleeding
		//make natural BloodCapacityIncrease depending on VisibleBleeding
		DecreaseToxinsCapacity(2*Body.RegenerationLevel);
		
		//process feelings
		//process symptoms
		
		if ((Body.HasInsidious)&&(Body.Symptom[Analgetic])) {
			Body.Symptom[Hallucination] = true;
		} else {
			Body.Symptom[Hallucination] = false;
		}

		//clear all other symptoms
		if (Body.Symptom[DeathTrauma]) {
			for (int i=0; i<MaxSymptom; i++)
				Body.Symptom[i] = false;
			Body.Symptom[DeathTrauma] = true;
		}
	}

	void BodyChangeCategory(DAMAGE_EFFECT _de, bool bIncrease)
	{
		for (int i=0; i<MaxTarget; i++) {
		for (int j=0; j<MaxDamageEffect; j++) {
			PPART part = &Body.Part[i][j];
			DAMAGE_EFFECT de = WoundToDamageEffect[part->wound];
			if (de == _de) {
				ApplyWound(
					part->wound, 
					bIncrease ? IncreaseCategory(part->CurrSeverity)
						: DecreaseCategory(part->CurrSeverity), 
					part);
			}
		}}
	}

	void IncreaseThermal()
	{
		BodyChangeCategory(Thermal,true);
	}

	void DecreaseThermal()
	{
		BodyChangeCategory(Thermal,false);
	}

	void DecreaseRadiation()
	{
		BodyChangeCategory(Radiation,false);
	}

	void StopRupturesAndBlows()
	{
		BodyChangeCategory(RuptureLimb,false);
		BodyChangeCategory(Rupture,false);
		BodyChangeCategory(Blow,false);
	}

	void DecreaseNecropoints(CURE_ID cure_id)
	{

	}
}