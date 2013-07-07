#pragma once
#ifndef _MED_
	#error _MED_
#endif

typedef enum _SYMPTOM {
	NoSymptom = -1,
    //Боль 1
    Pain1 = 0,
    //Боль 2
    Pain2 = 1,
    //Боль 3
    Pain3 = 2,
    //Боль 4
    Pain4 = 3,
    //Болевой шок
    PainShock = 4,
    //Мигрень
    HalfHeadAke = 5,
    //Зрительные галлюцинации 
    Hallucination = 6,
    //Тошнота
    Funny = 7,
    //Кашель слабый
    CoughLight = 8,
    //Кашель сильный
    CoughStrong = 9,
    //Судороги слабые
    SeizureLight = 10,
    //Судороги сильные
    SeizureStrong = 11,
    //Без сознания
    Unconciuous = 12,
    //Головокружение
    HeadSpin = 13,
    //Бред
    Delusions = 14,
    //Поза боксера
    BoxerPose = 15,
    //Остановка дыхания
    PreathStop = 16,
    //Озноб
    Shake = 17,
    //Кровотечение
    Bloodloss = 18,
    //Сознание спутано
    ConcinuousMess = 19,
    //Восприятие нарушено
    ConentrationLoss = 20,
    //Мелкая моторика нарушена
    HandShake = 21,
    //Слабость рук 1
    HandWeak1 = 22,
    //Слабость рук 2
    HandWeak2 = 23,
    //Слабость ног 1
    LegWeak1 = 24,
    //Слабость ног 2
    LegWeak2 = 25,
    //Паралич конечности 
    LimbParalyze = 26,
    //Боль перeлома кости
    BoneBreak = 27,
    //Боль трещины кости
    BoneCrack = 28,
    //Боль осколка кости или пули (ощущения чужеродного предмета)   
    BonePartPain = 29,
    //Остая боль
    SharpPain = 30,
    //Дергающая боль в области органа
    PulsePain = 31,
    //Одышка
    HardBreathe = 32,
    //Слабая тошнота
    FunnyLight = 33,
    //Сильная тошнота
    FunnyStrong = 34,
    //Кровь горлом 
    BlodThroat = 35,
    //Кровь горлом и рвота
    BloodPuke = 36,
    //Позывы к рвоте (спазмы)
    Puke = 37,
    //Симптом спины 1 (почки-печень)
    BackPain1 = 38,
    //Симтом спины 2 (кости)
    BackPain2 = 39,
    //скованность/паралич корпуса
    TorsoParalyze = 40,
    //скованность/паралич шеи
    NeckParalyze = 41,
    //Зуд
    ItchLight = 42,
    //Чесотка 
    ItchStrong = 43,
    //Ожоги легкие
    FireLight = 44,
    //Ожоги тяжелые
    FireHard1 = 45,
    //Кровавые волдыри (ожоги 3)
    FireHard2 = 46,
    //Сильное кровотечение 
    BloodLossStrong1 = 47,
    //Ощущение внутреннего кровотечения в полости (колыхание жидкости в животе, груди, голове)
    BloodLossInner = 48,
    //Приступ судорог (Событие для отыгрыша)
    Seizure = 49,
    //Приступ спазмов (событие для отыгрыша) 
    Spasm = 50,
    //Общая слабость
    GeneralWeak = 51,
    //Приступ обморока (событие)
    Faint = 52,
    //Ощущения от слабой потери крови
    BloodLossLight = 53,
    //Ощущения от средней потери крови 
    BloodLossMed = 54,
    //Ощущения от сильной потери крови
    BloodLossStrong = 55,
    //Ощущения от очень сильной потери крови
    BloodLossCritical = 56,
    //Кислородное голодание
    OxygenStarve1 = 57,
    //Общая токсикация организма 
    Toxication = 58,
    //Инсульт (событие) 
    Insult = 59,
    //Инфаркт (событие)
    Infarct = 60,
    //Замедленное сердцебиение
    SlowHeartBite = 61,
    //Пульсация в висках = слабая мигрень
    HalfHeadAke2 = 62,
    //Последствия сотрясения мозга (отыгрыш)
    HeadShake1 = 63,
    //Последствия сотрясения мозга (ощущения)
    HeadShake2 = 64,
    //Отказ печени *цирроз) 
    LiverFail = 65,
    //Отказ почек
    ButtonFail = 66,
    //Ощущение жара
    Hot = 67,
    //Ощущение холода (озноб)
    Cold = 68,
    //Жажда (хочу пить - действие к отыгрышу)
    Thirst = 69,
    //Лихорадка (отыгрыш)
    Fever = 70,
    //Загноение раны / воспаление
    FeveredWound = 71,
    //килородное голодание 
    OxygenStarve2 = 72,
    //интоксикация
    Intoxication = 73,
    //Смерть
    DeathTrauma = 74,
    //last
    MaxSymptom = 75,
} SYMPTOM;

extern const char* SymptomEffects[MaxSymptom];