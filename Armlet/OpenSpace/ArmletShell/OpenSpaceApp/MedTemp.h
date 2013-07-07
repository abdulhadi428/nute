//WOUND severity:
typedef enum _WOUND_SEVERITY {
	NoWound = -1,		//"нет ранения"
	GrazeWound = 0,		//"царапина"
	LightWound,			//"неопасное"
	SeriousWoundWound,		//"опасное"
	InsidiousWoundWound,		//"коварное"
	CriticalWoundWound,		//"критическое"
	MaxSeverity
} WOUND_SEVERITY;

//DAMAGE types:DAMAGE_TYPE {
	UnknownWound = -1,
	//SHOTS:
		//LIMBS:
		LeftArmShot = 0,	//"огнестрел в конечность, левая рука"
		RightArmShot,		//"огнестрел в конечность, правая рука"
		LeftLegShot,		//"огнестрел в конечность, левая нога"
		RightLegShot,		//"огнестрел в конечность, правая нога"
		//TORSO:
		ChestShot,			//"огнестрел в грудь"
		AbdomenShot,		//"огнестрел в живот"
		//HEAD:
		HeadShot,			//"огнестрел в голову"
	//KNOCKOUT:
	KnockOut,				//"нокаут"
	//EXPLOSIONS:
	ExplosionBlast,			//"поражающий фактор взрыва, ударная волна"
	ExplosionScorch,		//"поражающий фактор взрыва, ожог"
	ExplosionRadiation,		//"поражающий фактор взрыва, радиация"
	MaxType
} DAMAGE_TYPE;



typedef struct _WOUND {
	DAMAGE_TYPE type;			//damage type
	WOUND_SEVERITY severity;	//wound severity
	char* message1;
	char* message2;
	char intial_blood_loss;
	char tick_blood_loss;
	char duration;
	char severity_after_duration;
	char severity_on_next_hit;
	char severity_on_healing; //облегчение
} WOUND;

#define DEF_WOUND(x,y)	x,y

WOUND Wounds[MaxType][MaxSeverity] = 
{
	{//LeftArmShot
		{
			DEF_WOUND(LeftArmShot,NoWound),
			"",
			0,0,INFINITE,NoWound,RandomSelect,NoWound
		},
		{
			DEF_WOUND(LeftArmShot,GrazeWound),
			"Тебе обожгло левую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
			"(Название конечности) болит. [1]",
			5,0,25-Reg*5,NoWound,SeriousWound,NoWound
		},
		{
			DEF_WOUND(LeftArmShot,LightWound),
			"Тебе обожгло левую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
			"(Название конечности) болит. [1]",
			8,1
		},
		{
			DEF_WOUND(LeftArmShot,SeriousWound),
			"Тебе обожгло левую руку как огнем. Похоже, ранение серьезное, кровь так и течет",
			"(Название конечности) сильно болит [2]. Не можешь пользоваться (название конечности)",
			20,5
		},
		{
			DEF_WOUND(LeftArmShot,InsidiousWound),
			"Тебе обожгло левую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето",
			"ничего",
			10,1
		},
		{
			DEF_WOUND(LeftArmShot,CriticalWound),
			"Тебя серьезно ранило в левую руку. Течет кровь! Очень больно!",
			"(Название конечности) очень сильно болит [3]. Путаешься в словах, неудержимая дрожь."
				"Не можешь пользоваться (название конечности)"
			50,10
		},
	},
	{//RightArmShot
		{
			DEF_WOUND(RightArmShot,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightArmShot,CriticalWound),
			"",
			0,0
		},
	},
	{//LeftLegShot
		{
			DEF_WOUND(LeftLegShot,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(LeftLegShot,CriticalWound),
			"",
			0,0
		},
	},
	{//RightLegShot
		{
			DEF_WOUND(RightLegShot,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(RightLegShot,CriticalWound),
			"",
			0,0
		},
	},
	{//ChestShot
		{
			DEF_WOUND(ChestShot,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ChestShot,CriticalWound),
			"",
			0,0
		},
	},
	{//AbdomenShot
		{
			DEF_WOUND(AbdomenShot,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(AbdomenShot,CriticalWound),
			"",
			0,0
		},
	},
	{//HeadShot
		{
			DEF_WOUND(HeadShot,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(HeadShot,CriticalWound),
			"",
			0,0
		},
	},
	{//KnockOut
		{
			DEF_WOUND(KnockOut,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(KnockOut,CriticalWound),
			"",
			0,0
		},
	},
	{//ExplosionBlast
		{
			DEF_WOUND(ExplosionBlast,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionBlast,CriticalWound),
			"",
			0,0
		},
	},
	{//ExplosionScorch
		{
			DEF_WOUND(ExplosionScorch,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionScorch,CriticalWound),
			"",
			0,0
		},
	},
	{//ExplosionRadiation
		{
			DEF_WOUND(ExplosionRadiation,NoWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,GrazeWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,LightWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,SeriousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,InsidiousWound),
			"",
			0,0
		},
		{
			DEF_WOUND(ExplosionRadiation,CriticalWound),
			"",
			0,0
		},
	}
};













//NAME (FirstName, LastName)
//BODY
	//STATE
	//blood
		RegenerationLevel: 1,2,3	//naming
		BloodLossLevel: 0..4		//naming
		BloodLoss: 0..10						=> provides regeneration if 0, cumulutive
		BloodCapacityLevel: 0..6	//naming	=> function to symptoms
		BloodCapacity: 0..200
	//...
	//WOUNDS
//SYMPTOMS
	«легкая слабость, кружится голова»
	«сильная слабость, трудно стоять»
	«очень сильная слабость, не можешь стоять
	«потерял сознание», обезболивание 3
	умер
	«Поза боксера»	Да/нет	«Сжимаю кулаки, если разожмут — сожму снова»	Функция	
	«Вероятность судорог»	Да/нет	ничего		Непонятно, когда устанавливается
	Обезболивание	0..2	Отфильтровывает лишние сообщения, меняет описание боли		
	Нарушение мелкой моторики	да/нет	«Ничего не могу удержать в руках»		заменить на не могу пользоваться частью тела
	Рвота	да/нет	«Меня часто рвет»		
	Судороги	да/нет	«У меня частые мелкие судороги»		
	В сознании	да/нет	«Я без сознания»		
	Боль 1	Побавливает			блокируется обезбаливанием -1	
	Боль 2	Болит			блокируется обезбаливанием -2, чувствуется как боль 1 при обезбаливании -1	
	Боль 3	Сильно болит			не блокируется обезбаливанием?	Невозможно пользоваться частью тела
	Боль 4	Потеря сознания			?	
	Болевой шок	Естесственное обезбаливание (-1), часто сопутствует коварному ранению			Проходит через X+Y*рег. минут	
	Мигрень					
	Зрительные галлюцинации					
	Жар					
	Тошнота+Рвота					
	Кашель			Слабый, сильный		
	Судороги			Слабые, сильные		
	Хрипы дыхания			Эпизодические, слабые, сильные		
	Нарушение восприятия			Головокружение, Восприятие нарушено, Сознание спутано, Бред, Без сознания		
	Поза боксера					
	Больной не может дышать					
	Кровотечение
//CURES
//DAMAGES

					