#pragma once

//Место повреждения
typedef enum _MED_WOUND_TARGET {
	mwtUnknownTarget = -1,
	//LIMBS:
	mwtLimbTarget = 0,
	//TORSO:
	mwtUpTorso = 1,
	mwtDownTorso = 2,
	//HEAD:
	mwtHeadTarget = 3,
	//BEATEN:
	mwtBeatenTarget = 4,
	//STUNNED:
	mwtStunnedTarget = 5,
	//DYING:
	mwtDyingTarget = 6,
	mwtTargetLast
} MedWoundTarget;