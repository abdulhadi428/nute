#include "ArmletShell.h"
#include "Med.h"

namespace medicine {

	const char* CureName[] = {			//where		//duration
		"Общее обезболивающее",			//common	//15 min
		"Спазмолитик",					//common	//15 min
		"Жаропонижающее",				//common	//15 min
		"Аспиратор",					//common	//15 min
		"Коагулятор",					//common	//inst/15 min
		"Искусственная кровь",			//med		//inst/15 min

		"Искусственная кожа",			//med		//inst
		"Миорелаксант",					//med		//inst
		"Нанопак стволовых клеток",		//med		//inst with delay
		"Местный наркоз",				//med		//15 min
		"Антибиотик",					//common	//inst with delay
		"Абсорбент",					//med		//inst/15min
		"Наногипс",						//med		//inst with delay
		"Наноэкзоскелет",				//med		//15 min
		"Панацея"						//MG		//inst
	};

	const char* CureEffect[] = {
		"Тебе сделали укол... Кончик языка немеет.",
		"Тебе сделали укол... Движения стали четкими и плавными.",
		"Тебе сделали укол... Чувствуешь легкий озноб.",
		"Дышать стало сильно легче...",
		"Тебе сделали укол... Дыхание участилось.",
		"Тебе сделали укол... Ты чувствуешь сильный жар.",
		"Приятно холодящая тонкая пыль оседает на твоих ранах пленкой...",
		"Тебе сделали укол... Все тело расслабляется, движения стали 'вялыми'.",
		"Тебе сделали укол... Тебя словно колят иголками изнутри.",
		"Тебе сделали укол... Место укола совсем не чувствуется.",
		"Ты выпил таблетку...",
		"Ты жуешь невкусную массу, пахнущую резиной...",
		"Тебе сделали укол... пара капель пасты быстро затвердела и отвалилась.",
		"В твое тело вливается вязкая жидкость. Из кожи вырастают наросты. Твои движения 'деревянные'.",
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
		"Вдоль твоего тела водят струей пламени... Пахет паленой плотью."
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

	CURE_ACTION CureAction [MaxCureId];
	int TortureSecondUsage[MaxTortureId];
	CURE_SIDE_EFFECT CureSideEffect;

	void InitCureActions()
	{
		for (int i=0; i<MaxCureId; i++) {
			CureAction[i].RemainingTicks = 0;
			CureAction[i].IsUsing = false;
		}
		for (int i=0; i<MaxTortureId; i++) {
			TortureSecondUsage[i] = 0;
		}
		for (int i=0; i<sizeof(CureSideEffect); i++)
			((char*)&CureSideEffect)[i] = 0;
	}

	void ProcessCureUsage(CURE_ID cure_id, bool bStarting)
	{
		CureAction[cure_id].IsUsing = bStarting;
		CureAction[cure_id].RemainingTicks = bStarting ? 15 : 0; //minutes

		switch (cure_id) {
			case Analgetic:
				//Общее обезболивающее
				//Снимает слабую и умеренную боль. 
				//Не действует на ожоги и радиационные поражении.
				//Побочно - может вызывать галлюцинации.
				CureSideEffect.AnalgeticPainReduction = bStarting;
				break;
			case Antispasmodic:
				//Спазмолитик
				//Прекращает судороги и дрожь конечностей.
				//Побочно - вызывает головную боль.
				//Если применено при позе эмбриона => то расчесал нахуй все => категория ожога увеличивается
//FUCK			if (bStarting)
//						if (Body.HaveBoxerPose) IncreaseScorches();
				CureSideEffect.AntispasmodicBlockSudorogi = bStarting;
				CureSideEffect.AntispasmodicHeadPain = bStarting;
				CureSideEffect.AntispasmodicBleedingIncrease = bStarting;
				break;
			case Pyretic:
				//Жаропонижающее
				//Понижает температуру пациента.
				//(Следствие) Снижает давление и пульс, может вызвать тремор.
				if (bStarting) 
					CureSideEffect.PyrecticTemperatureDecrease += 1;
				else {
					if (CureSideEffect.PyrecticTemperatureDecrease != 0)
						CureSideEffect.PyrecticTemperatureDecrease -= 1;
				}
				break;
			case Aspirator:
				//Аспиратор
				//Облегчает дыхание пациента (при любых причинах затрудненности дыхания)
				CureSideEffect.AspiratorBreath = bStarting;
				break;
			case CoagulationFactor:
				//Коагулятор
				//Останавливает кровотечение, как скрытое, так и открытое. Возможны побочные эффекты.
				CureSideEffect.CoagulationFactorBleedingBlocked = bStarting;
				if (bStarting) {
					IncreaseBloodCapacity(
						5*(Body.RegenerationLevel-1),false); //leave same ToxinsCapacity
				}
				break;
			case SyntheticBlood:
				//Искусственная кровь
				//Быстро восполняет кровопотерю пациента. Побочно - повышает температуру.
				if (bStarting) { //inst
					IncreaseBloodCapacity(
						20+10*Body.RegenerationLevel,true); //reduce ToxinsCapacity by value
				}
				if (bStarting) 
					CureSideEffect.SyntheticBloodTemperature += 1;
				else {
					if (CureSideEffect.SyntheticBloodTemperature != 0)
						CureSideEffect.SyntheticBloodTemperature -= 1;
				}
				break;
//HERE
			case Leatherette:
				//Искусственная кожа
				//Восстанавливает поврежденные кожные покровы - лечит ожоги. 
				//Не применяется к пациентам, находящимся в данный момент в “позе эмбриона”. 
				//Побочно - вызывает небольшую единовременную потерю крови.
			//CureActions[cure_id].RemainingTicks = 0; //inst
			//if (bStarting)
			//		if (Body.HaveBoxerPose) InсreaseCategoryForScorches();
			//DecreaseCategoryForScorches();
			//BloodLoss(?);
				break;
			case Myorelaxant:
				//Миорелаксант
				//Прекращает судороги, дрожь конечностей, разгибает “позу эмбриона”. 
				//Возможны побочные эффекты.
			//CureActions[cure_id].RemainingTicks = 0; //inst
			//Body.HaveBoxerPose = false;
			//CureEffects.MyorelaxantBlockSudorogi //remove sudorogi & tremor
			//if (Body.HaveSeriousDamageSeverity)
			//	Body.BreathStopped = true;
				break;
			case VisceraNanoPack:
				//Нанопак стволовых клеток
				//Излечивает ожоги и следы воздействия радиации, оказывает пролонгированное 
				//куративное воздействие. Побочно - повышает температуру.
			//decrease category of Radiation/Thermal;temperature+=
			//inst delay 15
			//	if (...)
			//		BodyDecreaseCategory();
				break;
			case Anesthetics:
				//Местный наркоз
				//Сильное обезболивающее, позволяет снять мощные болевые эффекты 
				//(в том числе, болевые последствия пыток). Побочно - оказывает влияние на кровяное давление.
				CureSideEffect.AnestheticsPainReduction = bStarting;
				CureSideEffect.AnestheticsPressure = bStarting;
				break;
			case Antibiotic:
				//Антибиотик
				//Лечит воспалительные процессы. Воспаление может возобновиться. 
				//Побочно - увеличивает общую интоксикацию.
				CureSideEffect.AntibioticBlockNecroPoints = bStarting;
//FUCK each tick remove NecroPoint and add some Toxins
				break;
			case Absorber:
				//Абсорбент
				//Понижает интоксикацию, способствует выведению токсинов из организма.
				//Побочно - понижает кровяное давление.
				if (bStarting) 
					CureSideEffect.AbsorberLowPressure += 1;
				else {
					if (CureSideEffect.AbsorberLowPressure != 0)
						CureSideEffect.AbsorberLowPressure -= 1;
				}
//FUCK			DecreaseToxinsCapacity(50);
				break;
			case PlasterNanoPack:
				//Наногипс
				//Излечивает переломы, раздробления и осколочные повреждения тканей и 
				//внутренних органов, в том числе растворяет и/или выводит инородные тела (осколки). 
				//Побочно - повышает кровяное давление.
				if (bStarting) 
					CureSideEffect.PlasterNanoPackHigPressure += 1;
				else {
					if (CureSideEffect.PlasterNanoPackHigPressure != 0)
						CureSideEffect.PlasterNanoPackHigPressure -= 1;
				}
//FUCK each tick remove NecroPoint for FRAGMENT
				break;
			case NanoExoFrame:
				//Наноэкзоскелет
				//Позволяет в случае необходимости перемещаться и действовать пациенту со средней 
				//и тяжелой степенями тяжести повреждений. При этом движения приобретают характерную “деревянность”. 
				//Отрицательно сказывается на темпах выздоровления в целом.
				CureSideEffect.NanoExoFrame = true;
				CureAction[cure_id].RemainingTicks = 0;
				Body.RegenerationLevel = NoRegen;
				break;
			case MagicCure:
				InitBody();
				break;
			default:
				break;
		}
	}

	void ProcessTortureUsage(TORTURE_ID torture_id)
	{
		bool bNonFirstUsage = false;
		if (TortureSecondUsage[torture_id]!=0) {
			bNonFirstUsage = true;
		}
		TortureSecondUsage[torture_id] = 15;

		switch  (torture_id) {
			case ColdTorture:		//"Пытка холодом"
				//Scorch
				break;
			case CrumblingTorture:	//"Пытка раздроблением конечности"
				//LimbParalyze & Blow(Serious)
				break;
			case EyeSqueezingTorture://"Пытка выдавлиаванием глаза"
				//HeadShot(Serious)
				break;
			case SuffocationTorture://"Пытка удушьем"
				//Blow(Insideous) ;BreathStop on repeat
				break;
			case PoisonTorture:		//"Укол смертельного яда"
				//Death
				break;
			case DrowingTorture:	//"Пытка утоплением"
				//Blow(Insideous); BreathStop on repeat
				break;
			case FireTorture:		//"Пытка огнем"
				//Scorh
				break;
			default:
				break;
		}
	}

} //namespace
