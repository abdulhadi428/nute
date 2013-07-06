#include "ArmletShell.h"
#include "Med.h"

namespace price_of_honor {
void OnStartLeftArmShotNone() {}
void OnTickLeftArmShotNone() {}
void OnStartLeftArmShotGraze() {}
void OnTickLeftArmShotGraze() {}
void OnStartLeftArmShotLight() {}
void OnTickLeftArmShotLight() {}
void OnStartLeftArmShotInsidious() {}
void OnTickLeftArmShotInsidious() {}
void OnStartLeftArmShotSerious() {}
void OnTickLeftArmShotSerious() {}
void OnStartLeftArmShotCritical() {}
void OnTickLeftArmShotCritical() {}

void OnStartRightArmShotNone() {}
void OnTickRightArmShotNone() {}
void OnStartRightArmShotGraze() {}
void OnTickRightArmShotGraze() {}
void OnStartRightArmShotLight() {}
void OnTickRightArmShotLight() {}
void OnStartRightArmShotInsidious() {}
void OnTickRightArmShotInsidious() {}
void OnStartRightArmShotSerious() {}
void OnTickRightArmShotSerious() {}
void OnStartRightArmShotCritical() {}
void OnTickRightArmShotCritical() {}

void OnStartLeftLegShotNone() {}
void OnTickLeftLegShotNone() {}
void OnStartLeftLegShotGraze() {}
void OnTickLeftLegShotGraze() {}
void OnStartLeftLegShotLight() {}
void OnTickLeftLegShotLight() {}
void OnStartLeftLegShotInsidious() {}
void OnTickLeftLegShotInsidious() {}
void OnStartLeftLegShotSerious() {}
void OnTickLeftLegShotSerious() {}
void OnStartLeftLegShotCritical() {}
void OnTickLeftLegShotCritical() {}

void OnStartRightLegShotNone() {}
void OnTickRightLegShotNone() {}
void OnStartRightLegShotGraze() {}
void OnTickRightLegShotGraze() {}
void OnStartRightLegShotLight() {}
void OnTickRightLegShotLight() {}
void OnStartRightLegShotInsidious() {}
void OnTickRightLegShotInsidious() {}
void OnStartRightLegShotSerious() {}
void OnTickRightLegShotSerious() {}
void OnStartRightLegShotCritical() {}
void OnTickRightLegShotCritical() {}

void OnStartChestShotNone() {}
void OnTickChestShotNone() {}
void OnStartChestShotGraze() {}
void OnTickChestShotGraze() {}
void OnStartChestShotLight() {}
void OnTickChestShotLight() {}
void OnStartChestShotInsidious() {}
void OnTickChestShotInsidious() {}
void OnStartChestShotSerious() {}
void OnTickChestShotSerious() {}
void OnStartChestShotCritical() {}
void OnTickChestShotCritical() {}

void OnStartAbdomenShotNone() {}
void OnTickAbdomenShotNone() {}
void OnStartAbdomenShotGraze() {}
void OnTickAbdomenShotGraze() {}
void OnStartAbdomenShotLight() {}
void OnTickAbdomenShotLight() {}
void OnStartAbdomenShotInsidious() {}
void OnTickAbdomenShotInsidious() {}
void OnStartAbdomenShotSerious() {}
void OnTickAbdomenShotSerious() {}
void OnStartAbdomenShotCritical() {}
void OnTickAbdomenShotCritical() {}

void OnStartHeadShotNone() {}
void OnTickHeadShotNone() {}
void OnStartHeadShotGraze() {}
void OnTickHeadShotGraze() {}
void OnStartHeadShotLight() {}
void OnTickHeadShotLight() {}
void OnStartHeadShotInsidious() {}
void OnTickHeadShotInsidious() {}
void OnStartHeadShotSerious() {}
void OnTickHeadShotSerious() {}
void OnStartHeadShotCritical() {}
void OnTickHeadShotCritical() {}

void OnStartKnockOutNone() {}
void OnTickKnockOutNone() {}
void OnStartKnockOutGraze() {}
void OnTickKnockOutGraze() {}
void OnStartKnockOutLight() {}
void OnTickKnockOutLight() {}
void OnStartKnockOutInsidious() {}
void OnTickKnockOutInsidious() {}
void OnStartKnockOutSerious() {}
void OnTickKnockOutSerious() {}
void OnStartKnockOutCritical() {}
void OnTickKnockOutCritical() {}

void OnStartExplosionBlastNone() {}
void OnTickExplosionBlastNone() {}
void OnStartExplosionBlastGraze() {}
void OnTickExplosionBlastGraze() {}
void OnStartExplosionBlastLight() {}
void OnTickExplosionBlastLight() {}
void OnStartExplosionBlastInsidious() {}
void OnTickExplosionBlastInsidious() {}
void OnStartExplosionBlastSerious() {}
void OnTickExplosionBlastSerious() {}
void OnStartExplosionBlastCritical() {}
void OnTickExplosionBlastCritical() {}

void OnStartExplosionScorchNone() {}
void OnTickExplosionScorchNone() {}
void OnStartExplosionScorchGraze() {}
void OnTickExplosionScorchGraze() {}
void OnStartExplosionScorchLight() {}
void OnTickExplosionScorchLight() {}
void OnStartExplosionScorchInsidious() {}
void OnTickExplosionScorchInsidious() {}
void OnStartExplosionScorchSerious() {}
void OnTickExplosionScorchSerious() {}
void OnStartExplosionScorchCritical() {}
void OnTickExplosionScorchCritical() {}

void OnStartExplosionRadiationNone() {}
void OnTickExplosionRadiationNone() {}
void OnStartExplosionRadiationGraze() {}
void OnTickExplosionRadiationGraze() {}
void OnStartExplosionRadiationLight() {}
void OnTickExplosionRadiationLight() {}
void OnStartExplosionRadiationInsidious() {}
void OnTickExplosionRadiationInsidious() {}
void OnStartExplosionRadiationSerious() {}
void OnTickExplosionRadiationSerious() {}
void OnStartExplosionRadiationCritical() {}
void OnTickExplosionRadiationCritical() {}

const WOUND_DESC WoundsDescs[MaxType][MaxDamageSeverity] = 
{
	{//LeftArmShot
		{
			DEF_WOUND(LeftArmShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftArmShot,Serious),
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
			DEF_WOUND(RightArmShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,Serious),
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
			DEF_WOUND(LeftLegShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,Serious),
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
			DEF_WOUND(RightLegShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,Serious),
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
			DEF_WOUND(ChestShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,Serious),
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
			DEF_WOUND(AbdomenShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,Serious),
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
			DEF_WOUND(HeadShot,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,Serious),
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
			DEF_WOUND(KnockOut,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,Serious),
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
			DEF_WOUND(ExplosionBlast,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,Serious),
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
			DEF_WOUND(ExplosionScorch,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,Serious),
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
			DEF_WOUND(ExplosionRadiation,Graze),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Light),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Insidious),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,Serious),
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
COMPILE_TIME_CHECK(sizeof(WoundDescs)/sizeof(WOUND_DESC)==MaxType*MaxDamageSeverity);

//TODO DBG
bool CheckWounds()
{
	for (int i=0; i<MaxType; i++)
	for (int j=0; j<MaxDamageSeverity; j++)
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
void OnLimbLightBeg(short limb_id)
{
	//"Ранение болит и умеренно кровоточит"
	//PainLevel = 1
	//BloodLevel-=8;
	//Tick=0
}

void OnLimbLightTick(short limb_id)
{
	//Tick++;
	//if (Tick==15) OnLimbGrazeBeg()
}



//====================================

void OnLimbLightBeg(short limb_id)
{
	//"Ранение болит и кровоточит"
	//PainLevel = 2
	//BloodLevel -= 20;
	//Tick=0
}

void OnLimbLightTick(short limb_id)
{
	//Tick++;
	//BloodLevel -= 5;
	//if (Tick==20) OnLimbLightEnd()
}

void OnLimbLightEnd(short limb_id)
{
	//OnLimbGrazeBeg()
}
"Ранение болит и кровоточит.
Уровень боли: 2
Потеря крови: 20 ед. + 5 ед./мин.
Переходит в ""Смертельное"" без лечения через 20 минут.
Переходит в ""Неопасное"" при должном лечении как указано в лекарствах."
#endif
