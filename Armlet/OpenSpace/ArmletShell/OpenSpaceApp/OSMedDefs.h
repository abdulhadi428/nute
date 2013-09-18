#pragma once

//Место повреждения
typedef enum _MED_WOUND_TARGET {
	//DYING:
	mwtDyingTarget,
	//STUNNED:
	mwtStunnedTarget,
	//BEATEN:
	mwtBeatenTarget,
	//LIMBS:
	mwtLimbTarget,
	//TORSO:
	mwtUpTorso,
	mwtDownTorso,
	//HEAD:
	mwtHeadTarget,
	mwtUnknownTarget,
	MedWoundTargetLast
} MedWoundTarget;