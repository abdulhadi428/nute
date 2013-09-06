#pragma once
#ifndef _MED_
	#error _MED_
#endif

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

extern const char* PainDesc[MaxPain];
extern const char* BleedingDesc[4];
extern const char* FeelingDesc[MaxFeeling];
extern const char* SymptomDesc[MaxSymptom];