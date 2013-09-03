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
//Пытки
typedef enum _TORTURE_ID {
	UnknownTorture = -1,
	ColdTorture = 0,			//"Пытка холодом"						//pill 20
	CrumblingTorture = 1,		//"Пытка раздроблением конечности"		//pill 21
	EyeSqueezingTorture = 2,	//"Пытка выдавлиаванием глаза"			//pill 22
	SuffocationTorture = 3,		//"Пытка удушьем"						//pill 23
	PoisonTorture = 4,			//"Укол смертельного яда"				//pill 24
	DrowingTorture = 5,			//"Пытка утоплением"					//pill 25
	FireTorture = 6,			//"Пытка огнем"							//pill 26
	MaxTortureId =  7
} TORTURE_ID;
#pragma endregion

typedef struct _CURE_DESC {					
	CURE_ID id;						//cure id
	const char* Name;				//cure name
	const char* Effect;				//cure effect description
//	const char* Purpose;			//cure purpose description
//	const char* Contraindications;	//cure contraindications description
//	const char* SideEffects;		//cure side effects description
} CURE_DESC;

typedef struct _TORTURE_DESC {
	TORTURE_ID id;					//torture id
	const char* Name;				//torture name
	const char* Effect;				//torture effect description
	const char* EffectNoPain;		//torture effect description under anesthetics
} TORTURE_DESC;

extern const char* CureName[];
extern CURE_DESC CureDesc[MaxCureId];

extern const char* TortureName[];
extern TORTURE_DESC TortureDesc[MaxTortureId];

void InitCureAndTortureDescs();
