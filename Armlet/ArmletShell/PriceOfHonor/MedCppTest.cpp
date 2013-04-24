namespace price_of_honor_test {

#define COMPILE_TIME_CHECK(e) extern char _COMPILE_TIME_CHECK_[(e)?1:-1]

//CURE ids:
typedef enum _CURE_ID {
	Unknown = -1,
	Anesthetics = 0,		//"Общее обезболивающее"
	Antispasmodic,			//"Спазмолитик"
	Pyretic,				//"Жаропонижающее"
	Aspirator,				//"Аспиратор"
	CoagulationFactor,		//"Фактор свертываемости крови"
	SyntheticBlood,			//"Искусственная кровь"
	Leatherette,			//"Искусственная кожа"
	Myorelaxant,			//"Миорелаксант"
	VisceraNanoPack,		//"Пак нанохирургов для внутренних органов"
	RespiratoryNanoPack,	//"Пак дыхательных нанохирургов"
	MaxCureId
} CURE_ID;

const class Cure_t {
public:
	static CURE_ID const id = Unknown;
	inline virtual const CURE_ID GetCureId(void) const {return Unknown;}	//cure id
	//members as inline methods
	inline char const*const GetCureName(void) const;				//cure name
	inline char const*const GetCurePurpose(void) const;				//cure purpose description
	inline char const*const GetCureEffect(void) const;				//cure healing effect description
	inline char const*const GetCureContraindications(void) const;	//cure contraindications description
	inline char const*const GetCureSideEffects(void) const;			//cure side effects description
	//methods
	virtual void OnUse() const;										//cure effect on first use
	virtual void OnTick() const;									//cure effect on each tick
};

#define DECLARE_CURE(x)	\
	class x##Cure_t : public Cure_t { \
		static CURE_ID const id = x; \
		inline virtual const CURE_ID GetCureId(void) const {return x;} \
	}

DECLARE_CURE(Anesthetics);
DECLARE_CURE(Antispasmodic);
DECLARE_CURE(Pyretic);
DECLARE_CURE(Aspirator);
DECLARE_CURE(CoagulationFactor);
DECLARE_CURE(SyntheticBlood);
DECLARE_CURE(Leatherette);
DECLARE_CURE(Myorelaxant);
DECLARE_CURE(VisceraNanoPack);
DECLARE_CURE(RespiratoryNanoPack);

//extern inline const Cure_t* const GetCureById(CURE_ID id);
//DBG only
extern bool CheckCures();

typedef struct _CureStrings_t {
	const CURE_ID id;			//cure id
	const char* const name;		//cure name
	const char* const purpose;	//cure purpose description
	const char* const effect;	//cure effect description
	const char* const contra;	//cure contraindications description
	const char* const side;		//cure side effects description
} CureStrings_t;

const CureStrings_t CureStrings[MaxCureId] = 
{
	{
		Anesthetics,
		"Общее обезболивающее",
		"Снижение болевых ощущений",
		"Понижает уровень боли на 1",
		"Коварные ранения",
		"Мигрень, галлюцинации и уровень боли 2 (не снимается обезболиванием)"
	},
	{
		Antispasmodic,
		"Спазмолитик",
		"Подавление дрожи конечностей, предотвращение кашля, рвоты и судорог",
		"Подавляет озноб и дрожь, предотвращает кашель, судороги и рвоту."
			"Может восстановить мелкую моторику и 'разогнуть' из позы боксера", //regeneration based
		"Без противопоказаний",
		"Головная боль, уровень боли 1"
	},
	{
		Pyretic,
		"Жаропонижающее",
		"Понижение жара, прояснение сознания",
		"Сбивает жар, проясняет сознание, улучшает восприятие",
		"Коварные ожоги, коварная радиация, кашель",
		"Ощущение сильного холода в животе, усилиние кашля, хрипы"
	},
	{
		Aspirator,
		"Аспиратор",
		"Обеспечение дыхания",
		"Обеспечивает дыхание пациенту в течение 10 минут",
		"Без противопоказаний",
		"Сильные хрипы, слабый кашель"
	},
	{
		CoagulationFactor,
		"Фактор свертываемости крови",
		"Остановление кровопотери",
		"Прекращение кровотечения. Увеличивает запас крови.", //regeneration based
		"Без противопоказаний",
		"При коварных ранениях живота и груди повышает уровень боли на 1"
		"При применении на ожогах и радиационном поражении переводит ранение в следующую категорию (действует 1 раз на ранение)"

	},
	{
		SyntheticBlood,
		"Искусственная кровь",
		"Восстановление запаса крови",
		"Увеличивает запас крови. Облегчение огнестрельных ранений", //regeneration based
		"Без противопоказаний",
		"Жажда, жар"
	},
	{
		Leatherette,
		"Искусственная кожа",
		"Лечение ожогов",
		"Понижает уровень боли, восстонавливает запас крови, облегчает ожог", //regeneration based
		"Поза боксера",
		"Нет"
	},
	{
		Myorelaxant,
		"Миорелаксант",
		"Прекращение судорог, расслабление позы боксера",
		"'Разгибает' из позы боксера, судороги и дрожь прекращаются",
		"Рвота, огнестрельные ранения груди",
		"Потеря крови, смерть при противопоказаниях, совместно со спазмолитиком ухудшает ранения"
	},
	{
		VisceraNanoPack,
		"Пак нанохирургов для внутренних органов",
		"Лечение ожогов и радиации",
		"",
		"",
		""
	},
	{
		RespiratoryNanoPack,
		"Пак дыхательных нанохирургов",
		"Лечение ударных повреждений и огнестрельных ранений груди",
		"",
		"",
		""
	}
};
COMPILE_TIME_CHECK(sizeof(CureStrings)/sizeof(CureStrings_t)==MaxCureId);

#define DEFINE_CURE1(x)	const x##Cure_t x##Cure
#define DEFINE_CURE2(x)	&x##Cure
//#define DEFINE_CURE2(x)	(Cure_t*)&x##Cure
//#define DEFINE_CURE2(x)	dynamic_cast<Cure_t*>(&x##Cure)

DEFINE_CURE1(Anesthetics);
DEFINE_CURE1(Antispasmodic);
DEFINE_CURE1(Pyretic);
DEFINE_CURE1(Aspirator);
DEFINE_CURE1(CoagulationFactor);
DEFINE_CURE1(SyntheticBlood);
DEFINE_CURE1(Leatherette);
DEFINE_CURE1(Myorelaxant);
DEFINE_CURE1(VisceraNanoPack);
DEFINE_CURE1(RespiratoryNanoPack);

Cure_t const*const Cures[MaxCureId] = {
	DEFINE_CURE2(Anesthetics),
	DEFINE_CURE2(Antispasmodic),
	DEFINE_CURE2(Pyretic),
	DEFINE_CURE2(Aspirator),
	DEFINE_CURE2(CoagulationFactor),
	DEFINE_CURE2(SyntheticBlood),
	DEFINE_CURE2(Leatherette),
	DEFINE_CURE2(Myorelaxant),
	DEFINE_CURE2(VisceraNanoPack),
	DEFINE_CURE2(RespiratoryNanoPack)
};

inline const Cure_t* const GetCureById(CURE_ID id)
{
	return Cures[id];
}

typedef const Cure_t* const PCure_t;
//TODO DBG
bool CheckCures()
{
	for (int i=0; i<MaxCureId; i++) 
	{
		PCure_t obj = GetCureById((CURE_ID)i);
		CURE_ID j = obj->GetCureId();
		if (obj->GetCureId() != i) goto out;
	}
	return true;
out:
	return false;
}

inline const char* const Cure_t::GetCureName(void) const				{return CureStrings[GetCureId()].name;}
inline const char* const Cure_t::GetCurePurpose(void) const				{return CureStrings[GetCureId()].purpose;}
inline const char* const Cure_t::GetCureEffect(void) const				{return CureStrings[GetCureId()].effect;}
inline const char* const Cure_t::GetCureContraindications(void) const	{return CureStrings[GetCureId()].contra;}
inline const char* const Cure_t::GetCureSideEffects(void) const			{return CureStrings[GetCureId()].side;}

void Cure_t::OnUse() const
{
}

void Cure_t::OnTick() const
{
}

}//namespace
