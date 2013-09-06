#pragma once
#ifndef _MED_
	#error _MED_
#endif

#pragma region damage_severity
//Категория повреждений
typedef enum _DAMAGE_SEVERITY {
	//RandomSelect = -3,
	//Death = -2,
	None = -1,
	Graze = 0,				//царапина
	Light = 1,				//неопасное
	Insidious = 2,			//коварное
	Serious = 3,			//опасное
	Critical =4,			//смертельное
	MaxDamageSeverity = 5
} DAMAGE_SEVERITY;
#pragma endregion
#pragma region target
//Место повреждения
typedef enum _TARGET {
	UnknownTarget = -1,
		//LIMBS:
	LimbTarget = 0,
		LeftArmTarget = 0,	//"огнестрел в конечность, левая рука"
		RightArmTarget = 1,	//"огнестрел в конечность, правая рука"
		LeftLegTarget = 2,	//"огнестрел в конечность, левая нога"
		RightLegTarget = 3,	//"огнестрел в конечность, правая нога"
		//TORSO:
	TorsoTarget = 4,
		ChestTarget = 4,		//"огнестрел в грудь"
		AbdomenTarget = 5,	//"огнестрел в живот"
		BackTarget = 6,		//"огнестрел в спину"
		//HEAD:
	HeadTarget = 7,			//"огнестрел в голову"
	MaxTarget = 8
} TARGET;
#pragma endregion
#pragma region damage_effect
//Повреждающие воздействия
typedef enum _DAMAGE_EFFECT {
	RuptureLimb = 0,	//разрывное воздействие в конечность
	Rupture = 1,		//разрывное воздействие
	Blow = 2,			//ударное воздействие
	Thermal = 3,		//термическое воздействие
	Radiation = 4,		//радиационное воздействие
	MaxDamageEffect = 5
} DAMAGE_EFFECT;
#pragma endregion

//Регенерация
typedef enum _REGENERATION_LEVEL {
	NoRegen = 0,
	RegenLow = 1,
	RegenMed = 2,
	RegenHigh = 3
} REGENERATION_LEVEL;
