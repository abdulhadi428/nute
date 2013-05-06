#pragma once
#ifndef _MED_
	#error _MED_
#endif

#pragma region cure_id
// When this list updated, update server list 
// /Armlet/Armlet3/BastiliaGate_sw/Server/HonorLogic/Pills.cs
//Лекарства
typedef enum _CURE_ID {
	UnknownCure = -1,
	Analgetic = 0,			//"Общее обезболивающее" 
	Antispasmodic = 1,		//"Спазмолитик"
	Pyretic = 2,			//"Жаропонижающее"
	Aspirator =3,			//"Аспиратор"
	CoagulationFactor =4,	//"Коагулятор"
	SyntheticBlood =5,		//"Искусственная кровь"
	Leatherette =6,			//"Искусственная кожа"
	Myorelaxant =7,			//"Миорелаксант"
	VisceraNanoPack =8,		//"Нанопак стволовых клеток"
	Anesthetics =9,			//"Местный наркоз"
	Antibiotic =10,			//"Антибиотик"
	Absorber =11,			//"Абсорбент"
	PlasterNanoPack =12,	//"Наногипс"
	NanoExoFrame =13,		//"Наноэкзоскелет"
	MagicCure =14,			//"Панацея"
	MaxCureId = 15
} CURE_ID;
#pragma endregion
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
#pragma region damage_source
//Источник повреждения
typedef enum _DAMAGE_SOURCE {
	Shot = 0,			//"огнестрельное ранение"
	KnockOut = 1,		//"рукопашка"
	Explosion = 2		//"взрыв в отсеке"
} DAMAGE_SOURCE;
#pragma endregion
#pragma region explosion_effect
//Эффекты взрывов
typedef enum _EXPLOSION_EFFECT {
	ExplosionBlastWave = 0,			//"ударная волна"
	ExplosionFireCloud = 1,			//"раскалленное облако"
	ExplosionRadDoze = 2,			//"радиация"
	MaxExplosionEffect = 3
} EXPLOSION_EFFECT;
#pragma endregion
#pragma region organ
//Типы органов
typedef enum _ORGAN_TYPE {
	Cover = 0,		//"наружный"
	Surface = 1,	//"поверхностный"
	Visceral = 2,	//"внутренний"
	Visceral2 = 3,	//"внутренний"
	MaxOrganType = 4
} ORGAN_TYPE;

//Органы
typedef enum _BODY_ORGAN {
	Skin = 0,		//"кожа"
	Muscle = 1,		//"мышца"
	Bone = 2,		//"кость"
	Brains = 3,		//"мозг"
	Lungs = 4,		//"легкие"
	Heart = 5,		//"сердце"
	SBT	= 6,		//"ЖКХ"
	Liver = 7,		//"печень"
	Spleen = 8,		//"селезенка"
	Kidney = 9,		//"почки"
} BODY_ORGAN;
#pragma endregion
#pragma region body
//Часть тела
typedef enum _BODY_PART {
	Limb = 0,			//конечность
	Torso = 1,			//корпус
	Head = 2			//голова
} BODY_PART;

//Тип конечности
typedef enum _LIMB_TYPE {
	LeftArm = 0,		//"левая рука"
	RightArm = 1,		//"правая рука"
	LeftLeg = 2,		//"левая нога"
	RightLeg = 3		//"правая нога"
} _LIMB_TYPE;

//"Часть корпуса"
typedef enum _TORSO_PART {
	Chest = 0,			//"грудь"
	Abdomen = 1,		//"живот"
	Back = 2			//"спина"
} TORSO_PART;
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
COMPILE_TIME_CHECK(LeftArmTarget == (LimbTarget + LeftArm));
COMPILE_TIME_CHECK(RightArmTarget == (LimbTarget + RightArm));
COMPILE_TIME_CHECK(LeftLegTarget == (LimbTarget + LeftLeg));
COMPILE_TIME_CHECK(RightLegTarget == (LimbTarget + RightLeg));
COMPILE_TIME_CHECK(ChestTarget == (TorsoTarget + Chest));
COMPILE_TIME_CHECK(AbdomenTarget == (TorsoTarget + Abdomen));
COMPILE_TIME_CHECK(BackTarget == (TorsoTarget + Back));
#pragma endregion
#pragma region damage_effect
//Повреждающие воздействия
typedef enum _DAMAGE_EFFECT {
	Rupture = 0,		//разрывное воздействие
	Blow = 1,			//ударное воздействие
	Radiation = 2,		//радиационное воздействие
	Thermal = 3,		//термическое воздействие
	MaxDamageEffect = 4
} DAMAGE_EFFECT;
#pragma endregion

//Регенерация
typedef enum _REGENERATION_LEVEL {
	NoRegen = 0,
	RegenLow = 1,
	RegenMed = 2,
	RegenHigh = 3
} REGENERATION_LEVEL;

//Уровни дисфункции
typedef enum _DISFUNCTION_LEVEL {
	NoDisfn = 0,
	DisfnLow = 1,
	DisfnMed = 2,
	DisfnHigh = 3
} DISFUNCTION_LEVEL;

//Форма травмы/болезни
typedef enum _TRAUMA_FORM {
	Hidden = 0,		//"скрытая форма"
	Simple = 1,		//"простая форма"
	Hard = 2,		//"тяжелая форма"
} TRAUMA_FORM;

typedef enum _PAIN_LEVEL {
	NoPain = 0,
	PainMild = 1,
	PainAverage = 2,
	PainStrong = 3,
	PainIntense = 4,
	PainUnbearable = 5,
	PainUnconciuous = 6
} PAIN_LEVEL;
