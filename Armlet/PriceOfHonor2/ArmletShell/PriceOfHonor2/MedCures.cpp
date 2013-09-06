#include "ArmletApi.h"
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
		"Миорелаксант",					//med		//inst/15 min
		"Нанопак стволовых клеток",		//med		//inst with delay
		"Местный наркоз",				//med		//15 min
		"Антибиотик",					//common	//inst with delay
		"Абсорбент",					//med		//15min/inst/inst with delay
		"Наногипс",						//med		//inst
		"Наноэкзоскелет",				//med		//inst
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

	CURE_ACTION CureAction[MaxCureId];
	int TortureSecondUsage[MaxTortureId];

	void InitCureActions()
	{
		for (int i=0; i<MaxCureId; i++) {
			CureAction[i].RemainingTicks = 0;
			CureAction[i].IsUsing = false;
		}
		for (int i=0; i<MaxTortureId; i++) {
			TortureSecondUsage[i] = 0;
		}
	}

	void ProcessCureUsage(CURE_ID cure_id, bool bStarting)
	{
		CureAction[cure_id].IsUsing = bStarting;
		CureAction[cure_id].RemainingTicks = bStarting ? 15 : 0; //minutes
		if (bStarting) 
			CureAction[cure_id].CumulativeValue += 1;
		else {
			if (CureAction[cure_id].CumulativeValue != 0)
				CureAction[cure_id].CumulativeValue -= 1;
		}

		switch (cure_id) {
			case Analgetic:
				//Общее обезболивающее
				//Снимает слабую и умеренную боль. 
				//Не действует на ожоги и радиационные поражении.
				//Побочно - может вызывать галлюцинации.
				break;
			case Antispasmodic:
				//Спазмолитик
				//Прекращает судороги и дрожь конечностей.
				//Если применено при позе эмбриона => то расчесал нахуй все => категория ожога увеличивается
				if (bStarting)
					if (Body.Symptom[BoxerPose]) IncreaseThermal();
				break;
			case Pyretic:
				//Жаропонижающее
				//Понижает температуру пациента (кумулятивно)
				//(Следствие) Снижает давление и пульс, может вызвать тремор.
				break;
			case Aspirator:
				//Аспиратор
				//Облегчает дыхание пациента (при любых причинах затрудненности дыхания)
				break;
			case CoagulationFactor:
				//Коагулятор
				//Останавливает кровотечение, как скрытое, так и открытое. Возможны побочные эффекты.
				if (bStarting) {
					IncreaseBloodCapacity(
						5*(Body.RegenerationLevel-1),false); //leave same ToxinsCapacity
				}
				break;
			case SyntheticBlood:
				//Искусственная кровь
				//Быстро восполняет кровопотерю пациента. Побочно - повышает температуру (кумулятивно).
				if (bStarting) { //inst
					IncreaseBloodCapacity(
						20+10*Body.RegenerationLevel,true); //reduce ToxinsCapacity by value
					StopRupturesAndBlows();
				}
				break;
			case Leatherette:
				//Искусственная кожа
				//Восстанавливает поврежденные кожные покровы - лечит ожоги. 
				//Не применяется к пациентам, находящимся в данный момент в “позе эмбриона”. 
				//Побочно - вызывает небольшую единовременную потерю крови.
				CureAction[cure_id].IsUsing = false;
				CureAction[cure_id].RemainingTicks = 0; //inst
				if (Body.Symptom[BoxerPose]) 
					IncreaseThermal();
				else 
					DecreaseThermal();
				DecreaseBloodCapacity(25,true);
				break;
			case Myorelaxant:
				//Миорелаксант
				//Прекращает судороги, дрожь конечностей, разгибает “позу эмбриона”. 
				//Возможны побочные эффекты.
				if (bStarting) { //inst effect
					if ((Body.HasSerious)||(Body.HasCritical))
						Body.Symptom[BreathStop] = true;
				}
				break;
			case VisceraNanoPack:
				//Нанопак стволовых клеток
				//Излечивает ожоги и следы воздействия радиации, оказывает пролонгированное 
				//куративное воздействие. Побочно - повышает температуру.
				if (!bStarting) {//inst delay
					DecreaseNecropoints(VisceraNanoPack);
					if (!Body.Symptom[BoxerPose])
						DecreaseThermal();
					DecreaseRadiation();
					Body.Symptom[LimbParalyze] = false;
				}
				break;
			case Anesthetics:
				//Местный наркоз
				//Сильное обезболивающее, позволяет снять мощные болевые эффекты 
				//(в том числе, болевые последствия пыток). Побочно - оказывает влияние на кровяное давление.
				break;
			case Antibiotic:
				//Антибиотик
				//Лечит воспалительные процессы. Воспаление может возобновиться. 
				//Побочно - увеличивает общую интоксикацию.
				if (!bStarting) {//inst delay effect
					DecreaseNecropoints(Antibiotic);
				}
				break;
			case Absorber:
				//Абсорбент
				//Понижает интоксикацию, способствует выведению токсинов из организма.
				//Побочно - понижает кровяное давление.
				DecreaseToxinsCapacity(25); //inst and delay inst effect
				break;
			case PlasterNanoPack:
				//Наногипс
				//Излечивает переломы, раздробления и осколочные повреждения тканей и 
				//внутренних органов, в том числе растворяет и/или выводит инородные тела (осколки). 
				//Побочно - повышает кровяное давление.
				DecreaseNecropoints(PlasterNanoPack); //inst/inst with delay
				break;
			case NanoExoFrame:
				//Наноэкзоскелет
				//Позволяет в случае необходимости перемещаться и действовать пациенту со средней 
				//и тяжелой степенями тяжести повреждений. При этом движения приобретают характерную “деревянность”. 
				//Отрицательно сказывается на темпах выздоровления в целом.
				CureAction[cure_id].RemainingTicks = 0; //inst
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
				_medOnExplosion(100,2);
				break;
			case CrumblingTorture:	//"Пытка раздроблением конечности"
				{
					int Target = ArmletApi::GetRandom(RightLegTarget);
					Body.Symptom[LimbParalyze] = true;
					ApplyWound(Target,Serious,&Body.Part[Target][Blow]);
				}
				break;
			case EyeSqueezingTorture://"Пытка выдавлиаванием глаза"
				ApplyWound(HeadTarget,Serious,&Body.Part[HeadTarget][Blow]);
				break;
			case SuffocationTorture://"Пытка удушьем"
				if (bNonFirstUsage)
					Body.Symptom[BreathStop] = true;
				break;
			case PoisonTorture:		//"Укол смертельного яда"
				Body.Symptom[DeathTrauma] = true;
				break;
			case DrowingTorture:	//"Пытка утоплением"
				if (bNonFirstUsage)
					Body.Symptom[BreathStop] = true;
				break;
			case FireTorture:		//"Пытка огнем"
				_medOnExplosion(100,2);
				break;
			default:
				break;
		}
	}

} //namespace
