#pragma once
#ifndef _MED_
	#error _MED_
#endif

typedef enum _REGENERATION_LEVEL {
} REGENERATION_LEVEL;

typedef struct _BODY {
	//RegenerationLevel


	unsigned char PainReduction;

	unsigned char BloodLevel;
	unsigned char RegenerationLevel;
	//unsigned char Wounds[MAX_DAMAGE_FACTOR]; //store wounds

	//Symptoms:

} BODY, *PBODY; //TODO MOVE

extern BODY Body;

#if 0
/*
0 "Нет"
1 "Боль 1"
2 "Боль 2"
3 "Боль 3"
4 "Режущая боль"
5 "Дергающая боль"
6 "Мигрень"
7 "Зрительные галлюцинации"
8 "Жар"
9 "Тошнота"
10 "Рвота"
11 "Кашель"
12 "Сильный кашель"
13 "Слабые судороги"
14 "Сильные судороги"
15 "Слабый хрип"
16 "Сильный хрип"
17 "Мелкая моторика нарушена"
18 "Поза боксера"
19 "Больной не может дышать"
20 "Восприятие нарушено"
21 "Сознание спутано"
22 "Без сознания"
23 "Бред"
24 "Головокружение"
*/

#define CURE_TICK	60000		//1 minute in msecs



//РАНЕНИЯ
//конечности
#define LEFT_ARM	0	//"левая рука"
#define RIGHT_ARM	1	//"правая рука"
#define LEFT_LEG	2	//"левая нога"
#define RIGHT_LEG	3	//"правая нога"
//корпус
#define	HEAD		4	//"в голову"
#define	TORSO_1		5	//"в грудь"
#define	TORSO_2		6	//"в живот"
//НОКАУТ
#define	NOCKOUT		7	//"нокаут"
//ВЗРЫВ
#define EXPLOSION_1	8	//"поражен ударной волной"
#define EXPLOSION_2	9	//"поражен радиацией"
#define EXPLOSION_3	10	//"ожог"

//ТИПЫ РАНЕНИЙ			//random
#define DAMAGE_0	0
#define DAMAGE_1	1	//"царапина" 40%
#define DAMAGE_2	2	//"Неопасное ранение" 30%
#define DAMAGE_3	3	//«Опасное ранение» (20%),
#define DAMAGE_4	4	//«Коварное ранение» (10%),
#define DAMAGE_5	5	//«Критическое ранение» (0%)
#define DAMAGE_6	6	//"смерть"

DAMAGE_BETTER[] = {0,-1,-2,-1,-2,-2}
DAMAGE_WORSE[] =  {!,+2,+1,+2,+1, 0} //random
//Same hit into same body part = DAMAGE_WORSE

damage_t
{
	char* first_message;
}
Начальное сообщение (показывается, когда ранение только случилось)	
Начальное действие	
Постоянное действие (раз в минуту)	
Постоянное сообщение (в скобках указан уровень боли. 
					  Если текущий уровень обезболивания >= уровню боли, сообщение не показывается)	
Временной фактор	
При лечении	
При ухудшении










typedef enum _regeneration_level_t {
	reg_no = 0,
	reg_norm = 1,
	reg_high = 2
} regeneration_level_t;

/*
"161..200 нет сообщений
121..160 «легкая слабость, кружится голова»
81..120 «сильная слабость, трудно стоять»
41..80 «очень сильная слабость, не можешь стоять
1..40 «потерял сознание», обезболивание 3
0 умер"
*/

typedef struct _med_state_t {
	regeneration_level_t regeneration_level;	//can be set from server
												//config
	uint8_t blood_level;						//0..200
	uint8_t blood_loss;							//0..10
} med_state_t

/*
«Вероятность судорог»	Да/нет	ничего	Функция от процессов
Обезболивание	0..2	Отфильтровывает лишние сообщения	Функция от процессов
Имя персонажа	Строка	Константа	
Нарушение мелкой моторики	да/нет	«Ничего не могу удержать в руках»	Функция от процессов
Рвота	да/нет	«Меня часто рвет»	Функция от процессов
Судороги	да/нет	«У меня частые мелкие судороги»	Функция от процессов
В сознании	да/нет	«Я без сознания»	Функция от процессов
*/













namespace PriceOfHonor {

	//SYMPTOMS
	typedef enum _SYMPTOMS {
		NO_SYMPTOM,
		PAIN_1,
		PAIN_2,
		PAIN_3,
		PAIN_4,
		PAIN_5,
	} SYMPTOMS;
};

typedef struct _SYMPTOM_DESC {
	char* SymptomName;
	char* BriefDesc;
	char* DescForPatient;
} SYMPTOM_DESC;
#endif