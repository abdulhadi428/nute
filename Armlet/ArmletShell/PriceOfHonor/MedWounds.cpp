#include "ArmletShell.h"
#include "Med.h"

namespace price_of_honor {
void OnStartLeftArmShotNoWound() {}
void OnTickLeftArmShotNoWound() {}
void OnStartLeftArmShotLight() {}
void OnTickLeftArmShotLight() {}
void OnStartLeftArmShotMedium() {}
void OnTickLeftArmShotMedium() {}
void OnStartLeftArmShotSerious() {}
void OnTickLeftArmShotSerious() {}
void OnStartLeftArmShotInsidious() {}
void OnTickLeftArmShotInsidious() {}
void OnStartLeftArmShotCritical() {}
void OnTickLeftArmShotCritical() {}

void OnStartRightArmShotNoWound() {}
void OnTickRightArmShotNoWound() {}
void OnStartRightArmShotLight() {}
void OnTickRightArmShotLight() {}
void OnStartRightArmShotMedium() {}
void OnTickRightArmShotMedium() {}
void OnStartRightArmShotSerious() {}
void OnTickRightArmShotSerious() {}
void OnStartRightArmShotInsidious() {}
void OnTickRightArmShotInsidious() {}
void OnStartRightArmShotCritical() {}
void OnTickRightArmShotCritical() {}

void OnStartLeftLegShotNoWound() {}
void OnTickLeftLegShotNoWound() {}
void OnStartLeftLegShotLight() {}
void OnTickLeftLegShotLight() {}
void OnStartLeftLegShotMedium() {}
void OnTickLeftLegShotMedium() {}
void OnStartLeftLegShotSerious() {}
void OnTickLeftLegShotSerious() {}
void OnStartLeftLegShotInsidious() {}
void OnTickLeftLegShotInsidious() {}
void OnStartLeftLegShotCritical() {}
void OnTickLeftLegShotCritical() {}

void OnStartRightLegShotNoWound() {}
void OnTickRightLegShotNoWound() {}
void OnStartRightLegShotLight() {}
void OnTickRightLegShotLight() {}
void OnStartRightLegShotMedium() {}
void OnTickRightLegShotMedium() {}
void OnStartRightLegShotSerious() {}
void OnTickRightLegShotSerious() {}
void OnStartRightLegShotInsidious() {}
void OnTickRightLegShotInsidious() {}
void OnStartRightLegShotCritical() {}
void OnTickRightLegShotCritical() {}

void OnStartChestShotNoWound() {}
void OnTickChestShotNoWound() {}
void OnStartChestShotLight() {}
void OnTickChestShotLight() {}
void OnStartChestShotMedium() {}
void OnTickChestShotMedium() {}
void OnStartChestShotSerious() {}
void OnTickChestShotSerious() {}
void OnStartChestShotInsidious() {}
void OnTickChestShotInsidious() {}
void OnStartChestShotCritical() {}
void OnTickChestShotCritical() {}

void OnStartAbdomenShotNoWound() {}
void OnTickAbdomenShotNoWound() {}
void OnStartAbdomenShotLight() {}
void OnTickAbdomenShotLight() {}
void OnStartAbdomenShotMedium() {}
void OnTickAbdomenShotMedium() {}
void OnStartAbdomenShotSerious() {}
void OnTickAbdomenShotSerious() {}
void OnStartAbdomenShotInsidious() {}
void OnTickAbdomenShotInsidious() {}
void OnStartAbdomenShotCritical() {}
void OnTickAbdomenShotCritical() {}

void OnStartHeadShotNoWound() {}
void OnTickHeadShotNoWound() {}
void OnStartHeadShotLight() {}
void OnTickHeadShotLight() {}
void OnStartHeadShotMedium() {}
void OnTickHeadShotMedium() {}
void OnStartHeadShotSerious() {}
void OnTickHeadShotSerious() {}
void OnStartHeadShotInsidious() {}
void OnTickHeadShotInsidious() {}
void OnStartHeadShotCritical() {}
void OnTickHeadShotCritical() {}

void OnStartKnockOutNoWound() {}
void OnTickKnockOutNoWound() {}
void OnStartKnockOutLight() {}
void OnTickKnockOutLight() {}
void OnStartKnockOutMedium() {}
void OnTickKnockOutMedium() {}
void OnStartKnockOutSerious() {}
void OnTickKnockOutSerious() {}
void OnStartKnockOutInsidious() {}
void OnTickKnockOutInsidious() {}
void OnStartKnockOutCritical() {}
void OnTickKnockOutCritical() {}

void OnStartExplosionBlastNoWound() {}
void OnTickExplosionBlastNoWound() {}
void OnStartExplosionBlastLight() {}
void OnTickExplosionBlastLight() {}
void OnStartExplosionBlastMedium() {}
void OnTickExplosionBlastMedium() {}
void OnStartExplosionBlastSerious() {}
void OnTickExplosionBlastSerious() {}
void OnStartExplosionBlastInsidious() {}
void OnTickExplosionBlastInsidious() {}
void OnStartExplosionBlastCritical() {}
void OnTickExplosionBlastCritical() {}

void OnStartExplosionScorchNoWound() {}
void OnTickExplosionScorchNoWound() {}
void OnStartExplosionScorchLight() {}
void OnTickExplosionScorchLight() {}
void OnStartExplosionScorchMedium() {}
void OnTickExplosionScorchMedium() {}
void OnStartExplosionScorchSerious() {}
void OnTickExplosionScorchSerious() {}
void OnStartExplosionScorchInsidious() {}
void OnTickExplosionScorchInsidious() {}
void OnStartExplosionScorchCritical() {}
void OnTickExplosionScorchCritical() {}

void OnStartExplosionRadiationNoWound() {}
void OnTickExplosionRadiationNoWound() {}
void OnStartExplosionRadiationLight() {}
void OnTickExplosionRadiationLight() {}
void OnStartExplosionRadiationMedium() {}
void OnTickExplosionRadiationMedium() {}
void OnStartExplosionRadiationSerious() {}
void OnTickExplosionRadiationSerious() {}
void OnStartExplosionRadiationInsidious() {}
void OnTickExplosionRadiationInsidious() {}
void OnStartExplosionRadiationCritical() {}
void OnTickExplosionRadiationCritical() {}

const WOUND_DESC WoundsDescs[MaxType][MaxSeverity] = 
{
	{//LeftArmShot
		{
			DEF_WOUND(LeftArmShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Critical),
			"",
			0,0
		},
	},
	{//RightArmShot
		{
			DEF_WOUND(RightArmShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Critical),
			"",
			0,0
		},
	},
	{//LeftLegShot
		{
			DEF_WOUND(LeftLegShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Critical),
			"",
			0,0
		},
	},
	{//RightLegShot
		{
			DEF_WOUND(RightLegShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Critical),
			"",
			0,0
		},
	},
	{//ChestShot
		{
			DEF_WOUND(ChestShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Critical),
			"",
			0,0
		},
	},
	{//AbdomenShot
		{
			DEF_WOUND(AbdomenShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Critical),
			"",
			0,0
		},
	},
	{//HeadShot
		{
			DEF_WOUND(HeadShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Critical),
			"",
			0,0
		},
	},
	{//KnockOut
		{
			DEF_WOUND(KnockOut,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Critical),
			"",
			0,0
		},
	},
	{//ExplosionBlast
		{
			DEF_WOUND(ExplosionBlast,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Critical),
			"",
			0,0
		},
	},
	{//ExplosionScorch
		{
			DEF_WOUND(ExplosionScorch,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Critical),
			"",
			0,0
		},
	},
	{//ExplosionRadiation
		{
			DEF_WOUND(ExplosionRadiation,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Medium),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Serious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Critical),
			"",
			0,0
		},
	}
};
COMPILE_TIME_CHECK(sizeof(WoundDescs)/sizeof(WOUND_DESC)==MaxType*MaxSeverity);

//TODO DBG
bool CheckWounds()
{
	for (int i=0; i<MaxType; i++)
	for (int j=0; j<MaxSeverity; j++)
	{
			if (WoundDescs[i][j].type != i)	goto out;
			if (WoundDescs[i][j].severity != j)	goto out;
	}
	return true;
out:
	return false;
}

}//namespace

#if 0
void OnLimbMediumBeg(short limb_id)
{
	//"Ранение болит и умеренно кровоточит"
	//PainLevel = 1
	//BloodLevel-=8;
	//Tick=0
}

void OnLimbMediumTick(short limb_id)
{
	//Tick++;
	//if (Tick==15) OnLimbLightBeg()
}



//====================================

void OnLimbMediumBeg(short limb_id)
{
	//"Ранение болит и кровоточит"
	//PainLevel = 2
	//BloodLevel -= 20;
	//Tick=0
}

void OnLimbMediumTick(short limb_id)
{
	//Tick++;
	//BloodLevel -= 5;
	//if (Tick==20) OnLimbMediumEnd()
}

void OnLimbMediumEnd(short limb_id)
{
	//OnLimbLightBeg()
}
"Ранение болит и кровоточит.
Уровень боли: 2
Потеря крови: 20 ед. + 5 ед./мин.
Переходит в ""Смертельное"" без лечения через 20 минут.
Переходит в ""Неопасное"" при должном лечении как указано в лекарствах."
#endif
