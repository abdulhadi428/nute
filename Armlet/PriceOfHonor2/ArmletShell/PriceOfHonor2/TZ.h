Blood-Toxins should be >0
also there is BloodRegeneration and ToxicDecrising //on some if

				if (CureActions[Analgetic].IsUsing)
					if (Body.HaveInsidiousDamageSeverity) Body.Hallucination = true;
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

pain4 - unstoppable by cures
pain5 - pain shock
pain6 - uncons

show bleeding level

//17 symptoms & feelins

typedef enum _SYMPTOM {
	NoSymptom = -1,
    //Боль 1
    Pain1 = 0,
    //Боль 2 (отыгрыш)
    Pain2 = 1,
    //Боль 3 (отыгрыш)
    Pain3 = 2,
    //Боль 4 (отыгрыш)
    Pain4 = 3,
    //Болевой шок
    PainShock = 4,
    //Зрительные галлюцинации (отыгрыш)
    Hallucination = 5,
    //Тошнота
    Funny = 6,
    //Судороги (отыгрыш)
    Seizure = 7,
    //Без сознания (отыгрыш)
    Unconciuous = 8,
    //Головокружение
    HeadSpin = 9,
    //Поза боксера (отыгрыш)
    BoxerPose = 10,
    //Остановка дыхания (отыгрыш)
    BreathStop = 11,
    //Озноб
    Shake = 12,
    //Восприятие нарушено
    ConentrationLoss = 13,
    //Мелкая моторика нарушена (отыгрыш)
    HandShake = 14,
    //Паралич конечности (отыгрыш)
    LimbParalyze = 15,
    //Общая слабость
    GeneralWeak = 16,
    //Смерть (отыгрыш)
    DeathTrauma = 17,
    //last
    MaxSymptom = 18,
} SYMPTOM;

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
//	Боль, зрительные галлюцинанции, слабость и головокружение, тошнота, судороги, без сознания, 
//	поза боксера/эмбриона, тремор или дисфункция конечностей, паралич, приступ какого-либо симптома

//feelings & diagnostics (take partially from symptoms)
//	слабость – малый уровень кислорода (мало крови или интоксикация)
//	тошнота, судороги – признак интоксикации
//	боль – нервы говорят мозгу, что что-то не так
//	ИТД. (в процессе...)

//traumas - needed for коварное
//	FRAGMENT (bullet from shot or bonebreak)
//	TROMB?
//	INTERNAL BLEEDING
//	PERITONIT
//	разрыв осколком или уд. волной тканей (мышц и внутренних органов), некроз тканей (ожог) и эффекты лучевой болезни.

NecroPoint Reason
or Traumas have NecroPoints that initiates categorizing

//TRAUMAS