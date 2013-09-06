/*
typedef struct _CURE_SIDE_EFFECT {
	bool AntispasmodicBlockSudorogi; +Myorelaxant
	bool AntispasmodicHeadPain;
	bool AntispasmodicBleedingIncrease;
	int PyrecticTemperatureDecrease; //(Следствие) Снижает давление и пульс, может вызвать тремор.
	int VisceraNanoPackTemperatureIncrease;
	bool AspiratorBreath; 
	bool CoagulationFactorBleedingBlocked;
	int SyntheticBloodTemperature;
	bool AntibioticIncreaseToxication;	//FUCK each tick remove NecroPoint and add some Toxins
	int AbsorberLowPressure; (cumulative)
	bool AnestheticsPressure;

	int PlasterNanoPackHigPressure;
	bool NanoExoFrame;
} CURE_SIDE_EFFECT;
*/

typedef struct _WOUND_DESC {
	unsigned short target;			//wound subtype
	unsigned short severity;		//wound severity
	DAMAGE_EFFECT de;				//damage effect 
	int PainLevel;				//pain from wound
	int BloodLoss;				//instant bloodloss
	int Bleeding;				//bloodloos per tick
	int ToxinsAdd;				//instant intoxication
	int Toxinating;				//intoxication per tick
	int NecroPoints;			//Amount of "DEAD" flesh
	const char* message;
} WOUND_DESC;

pain4 - unstoppable by cures
pain5 - pain shock
pain6 - uncons

show bleeding level

//both rpg symptom and diag feeling
typedef enum _PAIN {
	NoPain = 0,
    //Боль 1
    Pain1 = 1,
    //Боль 2 (отыгрыш)
    Pain2 = 2,
    //Боль 3 (отыгрыш)
    Pain3 = 3,
    //Боль 4 (отыгрыш)
    Pain4 = 4,
    //Болевой шок
    PainShock = 5,
	//Без сознания
	PainUncon = 6,
	MaxPain = 7
} PAIN;

typedef enum _FEELING {
	NoFeeling = -1,
    //Тошнота
    Funny = 0,
    //Головокружение
    HeadSpin = 1,
    //Озноб
    Shake = 2,
    //Общая слабость
    GeneralWeak = 3,
    //Восприятие нарушено
    ConentrationLoss = 4,
	MaxFeeling = 5
} FEELING;

typedef enum _SYMPTOM {
	NoSymptom = -1,
    //Зрительные галлюцинации (отыгрыш)
    Hallucination = 0,
    //Судороги (отыгрыш)
    Seizure = 1,
    //Поза боксера (отыгрыш)
    BoxerPose = 2,
    //Мелкая моторика нарушена (отыгрыш)
    HandShake = 3,
    //Паралич конечности (отыгрыш)
    LimbParalyze = 4,
    //Остановка дыхания (отыгрыш)
    BreathStop = 5,
    //Без сознания (отыгрыш)
    Unconciuous = 6,
    //Смерть (отыгрыш)
    DeathTrauma = 7,
    //last
    MaxSymptom = 8
} SYMPTOM;

Blood-Toxins should be >0
also there is BloodRegeneration and ToxicDecrising //on some if


				//Thermal or Radiation - don't use PainReduction 1

				if (CureActions[Aspirator].IsUsing)
					//can breath

				Perception depend on Temperature

				big PyrecticTemperatureDecrease => decrease pressure
					//temperature-=; pulse-; pressure-;					//15 min
					//temperature--=; pulse--; pressure--; tremorOn
				//(Следствие) Снижает давление и пульс, может вызвать тремор.

наногипс повышает давление
[04.09.2013 1:19:56] aleksko_sw: абсорбент понижает
[04.09.2013 1:20:05] Ольга Захарова: ок
[04.09.2013 1:20:21] aleksko_sw: местный наркоз рандомно влияет на давление

/*
Пострадавший дезориентирован, не может самостоятельно передвигаться, 
наличествует наружное кровотечение, переломы, заметная дрожь конечностей.

У пострадавшего судороги, пострадавший находится без сознания, пострадавший лежит 
на боку скорчившись, закрыв голову руками, сжатыми в кулаки (“поза эмбриона”, встречается 
															 при тяжелых ожогах и поражении радиацией).

УРОВЕНЬ КРОВОТЕЧЕНИЯ (степень)
*/

Кровяное давление. На него оказывают влияние потеря крови, стресс и побочные эффекты лекарств.
Нормальное (120/80 - нет потери крови)
Пониженное (90/60 - серьезная потеря крови)
Сильно пониженное (75/50 - критическая потеря крови)
Нет (60/40 - пострадавший технически мертв)

Температура
Нормальная (36.6 0 для среднего здорового человека)
Повышенная (37-40 признак воспаления в результате болезни или наличия инородного предмета (осколка)).
Радикально повышенная (40-42 опасная для жизни)

Внешнее кровотечение. Уровень кровотечения свидетельствует о степени опасности травмы. 
Внутреннее кровотечение таким образом не устанавливается.
слабое (1)
среднее(2)
сильное (3)

Пульс
Замедленный (40-59 - обычно сопуствует повышению температуры)
Нормальный (60-80)
Повышенный (81-150 - признак интоксикации)

//when category increases we need to process trauma
//TRAUMAS:
//	common => additional necropoints			(for thermal & radiation)
//	common TROMB => just as if new shot/blow	(for rupture & blow)
//	inner bloodloss => peritonit				(for any)
//	bullet or bonebreak	=> fragment				(for rupture & blow)
// +++ tortures?

/*
Cough
Weakness
LimbParalyze
*/

//priority for symptoms

//pressure can be lower vessels are squezed (in case of bleeding) + side effects
//high temperature in case of disease/fragment (воспаление)

//symptoms - to roleplay!!
//	Боль, зрительные галлюцинанции,  без сознания, 
//	поза боксера/эмбриона, тремор или дисфункция конечностей, паралич, приступ какого-либо симптома

//	тошнота, судороги – признак интоксикации
//	слабость, головокружение – малый уровень кислорода (мало крови или интоксикация)

//traumas - needed for коварное
//	FRAGMENT (bullet from shot or bonebreak)
//	TROMB?
//	INTERNAL BLEEDING
//	PERITONIT
//	разрыв осколком или уд. волной тканей (мышц и внутренних органов), некроз тканей (ожог) и эффекты лучевой болезни.

NecroPoint Reason
or Traumas have NecroPoints that initiates categorizing

//TRAUMAS