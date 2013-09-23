#include "OpenSpace.h"




fresult OSBehaviourLogic::Init()
{
	fresult fres;
	
	fres = InitBehaviours();
	ENSURESUCCESS(fres);

//DEBUG;
	Patterns[bhpWoundHead].IsActive = TRUE;
	Patterns[bhpFuckingIllness].IsActive = TRUE;
	Patterns[bhpFuckingIllness].IsNew = TRUE;
	Patterns[bhpEgoCritical].IsActive = TRUE;


	return SUCCESS;
}

fresult OSBehaviourLogic::InitBehaviours()
{

	Patterns[bhpWoundHead].Text = "Очень болит раненая рука. Ты не можешь ей ничего делать";
	Patterns[bhpWoundHead].Title = "Рана конечности";
	Patterns[bhpWoundHead].IsCritical = FALSE;
	Patterns[bhpWoundHead].CritText = NULL;

	Patterns[bhpWoundTorso].Text = "Рана все еще дает о себе знать. Действительно тяжело ходить и сгибаться. Когда же это пройдет?";
	Patterns[bhpWoundTorso].Title = "Рана низа корпуса";
	Patterns[bhpWoundTorso].IsCritical = FALSE;
	Patterns[bhpWoundTorso].CritText = NULL;

	Patterns[bhpFuckingIllness].Text = "Полная аппатия. Темнеет в глазах и дрожат руки. Ты можешь только сидеть и лежать";
	Patterns[bhpFuckingIllness].Title = "Ебота";
	Patterns[bhpFuckingIllness].IsCritical = TRUE;
	Patterns[bhpFuckingIllness].CritText = "Внимание!\nПродолжение болезни вызовет критическое состояние. \nПродолжить болезнь?";

	Patterns[bhpEgoCritical].Text = "В тебе поселилась неуверенность, и это раздражает. Все ты знаешь о себе и мире? У тебя есть желание задавать больше вопросов.";
	Patterns[bhpEgoCritical].Title = "Стресс эго";
	Patterns[bhpEgoCritical].IsCritical = FALSE;
	Patterns[bhpEgoCritical].CritText = NULL;

	for (ubyte_t i =0; i< bhpLast; i++)
	{
		Patterns[i].IsActive = FALSE;
	}

	return SUCCESS;
}
