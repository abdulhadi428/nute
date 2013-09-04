#include "ArmletShell.h"
#include "Med.h"

namespace medicine {

	const char* CureName[] = {			//where		//purpose-contradictions-side effects				//duration
		"Общее обезболивающее",			//common	//EffectivePain--; if (Insidious) Hallucination		//10 min
		"Спазмолитик",					//common	//blocks symptoms from group1; HeadPain++			//10 min
		"Жаропонижающее",				//common	//temperature-=; no									//10 min
		"Аспиратор",					//common	//restores breath; cough on							//10 min
		"Коагулятор",					//common	//stop bleeding & BloodCapacity+=5*(R-1); no		//10 min
		"Искусственная кровь",			//med		//BloodCapacity+=10*R; temperature+=				//inst
		"Искусственная кожа",			//med		//Reduce NECRO; non-usable if BoxerPose				//inst
		"Миорелаксант",					//med		//blocks symptoms from group1, remove BoxerPose; BloodLoss 5
																										//inst
		"Нанопак стволовых клеток",		//med		//decrease category of Radiation/Thermal;temperature+=
																										//inst delay 20
		"Местный наркоз",				//med		//EffectivePain-=2; ?								//10 min
		"Антибиотик",					//common	//blocks inflammation; toxinadd ?					//10 min
		"Абсорбент",					//med		//detoxing											//inst
		"Наногипс",						//med		//blocks FRAGMENT traumas
		"Наноэкзоскелет",				//med		//?
		"Панацея"						//MG		//cures everything; no								//inst
	};

	const char* CureEffect[] = {
		"Тебе сделали укол... Кончик языка немеет.",
		"Тебе сделали укол... Ты чувствуешь мгновенное облегчение.",
		"Тебе сделали укол... Чувствуешь прохладу.",
		"Дышать стало сильно легче...",
		"Тебе сделали укол... Дыхание участилось.",
		"Тебе сделали укол... Ты чувствуешь сильный жар.",
		"Приятно холодящая тонкая пыль оседает на твоих ранах пленкой...",
		"Тебе сделали укол... Все тело расслабляется.",
		"Тебе сделали укол... Тебя словно колят иголками изнутри.",
		"Тебе сделали укол... Место укола совсем не чувствуется.",
		"Ты выпил таблетку...",
		"Ты жуешь невкусную массу, пахнущую резиной...",
		"Тебе сделали укол... пара капель пасты быстро затвердела и отвалилась.",
		"---",
		"Тебе сделали укол...Ты моргнул и почувствовал, что родился заново."
	};

	COMPILE_TIME_CHECK(sizeof(CureName)/sizeof(char*)==MaxCureId);
	COMPILE_TIME_CHECK(sizeof(CureEffect)/sizeof(char*)==MaxCureId);

	const char* TortureName[] = {
		"Пытка холодом",					//no charges
		"Пытка раздроблением конечности",	//no charges
		"Пытка выдавлиаванием глаза",		//no charges
		"Пытка удушьем",					//no charges
		"Укол смертельного яда",			//charges
		"Пытка утоплением",					//no charges
		"Пытка огнем"						//no charges
	};

	const char* TortureEffect[] = {
		"Из пипетки на кожу падают капли жидкого азота... Очень больно!",
		"Твою руку сжимают в тисках, ломая кости... Ты теряешь сознание от боли!",
		"Поджимая веко, о твой глаз тушат горящую сигару... Боль ужасна!",
		"Удавка стягивает горло, ты пытаешься сделать вдох и не можешь!..",
		"Тебе сделали укол. Ты умираешь в страшных мучениях...",
		"Тебя окунули в воду, ты задерживал дыхание сколько мог... Вдох и легкие раздирает нестерпимая боль!",
		"Вдоль твоего тела водят струей пламени... Очень больно!"
	};

	const char* TortureEffectNoPain[] = {
		"Из пипетки на кожу падают капли жидкого азота... Прикольно смотреть как они испаряются.",
		"Твою руку сжимают в тисках, слышен треск костей... Печально, что твоё тело уродуют.",
		"Поджимая веко, о твой глаз тушат горящую сигару... Пахнет гарью и слегка жутко.",
		"Удавка стягивает горло, ты пытаешься сделать вдох и не можешь!..",
		"Тебе сделали укол. Ты умираешь с улыбкой на губах...",
		"Тебя окунули в воду, ты задерживал дыхание сколько мог... Вдох и легкие заполняет вода! Сознание мутнеет.",
		"Вдоль твоего тела водят струей пламени... Пахет гарью."
	};

	COMPILE_TIME_CHECK(sizeof(TortureName)/sizeof(char*)==MaxTortureId);
	COMPILE_TIME_CHECK(sizeof(TortureEffect)/sizeof(char*)==MaxTortureId);
	COMPILE_TIME_CHECK(sizeof(TortureEffectNoPain)/sizeof(char*)==MaxTortureId);

	CURE_DESC CureDesc[MaxCureId];
	TORTURE_DESC TortureDesc[MaxTortureId];

	void InitCureAndTortureDescs()
	{
		for (int i=0;i<MaxCureId;i++) {
			CureDesc[i].id = (CURE_ID)i;
			CureDesc[i].Name = CureName[i];
			CureDesc[i].Effect = CureEffect[i];
	}
		for (int i=0;i<MaxTortureId;i++) {
			TortureDesc[i].id = (TORTURE_ID)i;
			TortureDesc[i].Name = TortureName[i];
			TortureDesc[i].Effect = TortureEffect[i];
			TortureDesc[i].EffectNoPain = TortureEffectNoPain[i];
		}
}

} //namespace
