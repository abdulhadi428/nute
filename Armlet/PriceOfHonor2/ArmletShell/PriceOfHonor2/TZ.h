//UNUSED???
typedef enum _SYMPTOM {
    //Судороги (отыгрыш)
    Seizure = 1,
    //Мелкая моторика нарушена (отыгрыш)
    HandShake = 3,
 } SYMPTOM;

//UNUSED
typedef enum _FEELING {
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

/*
typedef struct _CURE_SIDE_EFFECT {
	bool AntispasmodicBlockSudorogi; +Myorelaxant
	bool AntispasmodicHeadPain;
	bool AntispasmodicBleedingIncrease;
	int PyrecticTemperatureDecrease; //(Следствие) Снижает давление и пульс, может вызвать тремор.
	int VisceraNanoPackTemperatureIncrease;
	bool CoagulationFactorBleedingBlocked;
	int SyntheticBloodTemperature;
	bool AntibioticIncreaseToxication;	//FUCK each tick remove NecroPoint and add some Toxins
	int AbsorberLowPressure; (cumulative)
	bool AnestheticsPressure;

	int PlasterNanoPackHigPressure;
	bool NanoExoFrame;
} CURE_SIDE_EFFECT;
*/

Perception depend on Temperature
				big PyrecticTemperatureDecrease => decrease pressure
					//temperature-=; pulse-; pressure-;					//15 min
					//temperature--=; pulse--; pressure--; tremorOn
				//(Следствие) Снижает давление и пульс, может вызвать тремор.

наногипс повышает давление
[04.09.2013 1:19:56] aleksko_sw: абсорбент понижает
[04.09.2013 1:20:05] Ольга Захарова: ок
[04.09.2013 1:20:21] aleksko_sw: местный наркоз рандомно влияет на давление

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

Necropoints?
//when category increases we need to process trauma
//TRAUMAS:
//	common => additional necropoints			(for thermal & radiation)
//	common TROMB => just as if new shot/blow	(for rupture & blow)
//	inner bloodloss => peritonit				(for any)
//	bullet or bonebreak	=> fragment				(for rupture & blow)
// +++ tortures?

//pressure can be lower vessels are squezed (in case of bleeding) + side effects
//high temperature in case of disease/fragment (воспаление)

//traumas - needed for коварное
//	FRAGMENT (bullet from shot or bonebreak)
//	TROMB?
//	INTERNAL BLEEDING
//	PERITONIT
//	разрыв осколком или уд. волной тканей (мышц и внутренних органов), некроз тканей (ожог) и эффекты лучевой болезни.
