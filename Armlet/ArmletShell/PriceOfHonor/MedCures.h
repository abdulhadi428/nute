#pragma once
#ifndef _MED_
	#error _MED_
#endif

// When this list updated, update server list 
// /Armlet/Armlet3/BastiliaGate_sw/Server/HonorLogic/Pills.cs

//CURE ids:
typedef enum _CURE_ID {
	UnknownCure = -1,
	Analgetic = 0,			//"Общее обезболивающее - Анальгетик" 
	Antispasmodic,			//"Спазмолитик"
	Pyretic,				//"Жаропонижающее"
	Aspirator,				//"Аспиратор"
	CoagulationFactor,		//"Фактор свертываемости крови"
	SyntheticBlood,			//"Искусственная кровь"
	Leatherette,			//"Искусственная кожа"
	Myorelaxant,			//"Миорелаксант"
	VisceraNanoPack,		//"Пак нанохирургов для внутренних органов" => "Нанопак стволовых клеток"
	RespiratoryNanoPack,	//"Пак дыхательных нанохирургов"
//more
	Anesthetics,			//"Местный наркоз"		?
	Antibiotic,				//"Антибиотик"			мед. отсек
	Absorber,				//"Абсорбент"			мед. отсек
	PlasterNanoPack,		//"Наногипс"			мед. отсек
	NanoExoFrame,			//"Наноэкзоскелет"		1-2 эксперимент. образца
	MagicCure,				//"Волшебная таблетка"	мастерский
	MaxCureId
} CURE_ID;

class Cure_t {
public:
	//static members
	static CURE_ID const id = UnknownCure;
	//static members as inline getters
	static inline char const*const CureName(void);					//cure name
	static inline char const*const CurePurpose(void);				//cure purpose description
	static inline char const*const CureEffect(void);				//cure healing effect description
	static inline char const*const CureContraindications(void);		//cure contraindications description
	static inline char const*const CureSideEffects(void);			//cure side effects description
	//methods
	virtual void OnUse(void) const {};								//cure effect on first use
	virtual void OnTick(void) const {};								//cure effect on each tick
	virtual ~Cure_t(void) {return;}									//empty virtual destructor is required
};

extern inline Cure_t const*const GetCureById(CURE_ID id);
//DBG only
extern bool CheckCures();
