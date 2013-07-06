#if 0

/*
  disfunctions
  {
{NoSymptom,NoSymptom},
{HandWeak1,HandWeak1},
{Pain2,Pain2},
{PulsePain,PulsePain},
{NoSymptom,NoSymptom},
{HandWeak1,HandWeak1},
{Pain2,Pain2},
{PulsePain,PulsePain},
{NoSymptom,NoSymptom},
{LegWeak1,LegWeak1},
{Pain2,Pain2},
{PulsePain,PulsePain},
{NoSymptom,NoSymptom},
{LegWeak1,LegWeak1},
{Pain2,Pain2},
{PulsePain,PulsePain},
{NoSymptom,NoSymptom},
{SharpPain,SharpPain},
{CoughStrong,CoughStrong},
{HardBreathe,HardBreathe},
{NoSymptom,NoSymptom},
{GeneralWeak,GeneralWeak},
{Puke,Puke},
{BackPain1,BackPain1},
{NoSymptom,NoSymptom},
{BackPain2,BackPain2},
{BloodLossMed,BloodLossMed},
{Pain2,Pain2},
{NoSymptom,NoSymptom},
{HeadShake1,HeadShake1},
{NeckParalyze,NeckParalyze},
{ConcinuousMess,ConcinuousMess}
}
*/

void SYMPTOMS getBloodLossSymptom(ubyte_t bloodCapacity)
{
	if(bloodCapacity > 161)
		return SYMPTOMS::NoSymptom;
	else if (bloodCapacity > 121)
		return SYMPTOMS::BloodLossLight;
	else if (bloodCapacity > 81)
		return SYMPTOMS::BloodLossMed;
	else if (bloodCapacity > 41)
		return SYMPTOMS::BloodLossStrong;
	else if (bloodCapacity > 1)
		return SYMPTOMS::BloodLossCritical;
	else 
		return SYMPTOMS::Death;
}

/*
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

*/
#endif