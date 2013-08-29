#pragma once
#ifndef _MED_
	#error _MED_
#endif

typedef enum _EXPLOSION_EFFECT {
	BlastWave = 0,
	FireCloud = 1,
	Radiation = 2,
	MaxExplosionEffect = 3
} EXPLOSION_EFFECT;

typedef enum _DAMAGE_SEVERITY {
	RandomSelect = -3,
	Death = -2,
	None = -1,
	Graze = 0,				//царапина
	Light = 1,				//неопасное
	Insidious = 2,			//коварное
	Serious = 3,			//опасное
	Critical =4,			//смертельное
	MaxDamageSeverity = 5
} DAMAGE_SEVERITY;

extern const DAMAGE_SEVERITY	RandomSelectPerTenPercent[10];
extern const char* ExplosionDesc[MaxExplosionEffect][MaxDamageSeverity];

//CURE ids:
typedef enum _CURE_ID {
	UnknownCure = -1,
	Analgetic = 0,			//"Общее обезболивающее - Анальгетик" 
	Antispasmodic = 1,		//"Спазмолитик"
	Pyretic = 2,			//"Жаропонижающее"
	Aspirator =3,			//"Аспиратор"
	CoagulationFactor =4,	//"Фактор свертываемости крови - Коагулятор"
	SyntheticBlood =5,		//"Искусственная кровь"
	Leatherette =6,			//"Искусственная кожа"
	Myorelaxant =7,			//"Миорелаксант"
	VisceraNanoPack =8,		//"Пак нанохирургов для внутренних органов => Нанопак стволовых клеток"
//	RespiratoryNanoPack,	//"Пак дыхательных нанохирургов"
//more
	Anesthetics =9,			//"Местный наркоз"		?
	Antibiotic =10,			//"Антибиотик"			мед. отсек
	Absorber =11,			//"Абсорбент"			мед. отсек
	PlasterNanoPack =12,	//"Наногипс"			мед. отсек
	NanoExoFrame =13,		//"Наноэкзоскелет"		1-2 эксперимент. образца
	MagicCure =14,			//"Волшебная таблетка"	мастерский
	MaxCureId = 15
} CURE_ID;

extern const char* CureNames[];