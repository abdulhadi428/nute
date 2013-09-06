#pragma once
#ifndef _MED_
	#error _MED_
#endif

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

extern const char* SymptomEffects[MaxSymptom];