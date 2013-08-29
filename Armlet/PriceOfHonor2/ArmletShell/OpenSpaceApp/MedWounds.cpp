#include "ArmletApi.h"
#include "ArmletShell.h"
#include "Med.h"

namespace medicine {

#pragma region WoundEffects
const char* WoundEffects[MaxWoundType][MaxDamageSeverity] = {
{ //левую руку
    //царапина    
    "Тебе обожгло левую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    неопасное
    "Тебе обожгло левую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    коварное
    "Тебе обожгло левую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    опасное
    "Тебе обожгло левую руку как огнем. Похоже, ранение серьезное, кровь так и течет",
    //    критическое
    "Тебя серьезно ранило в левую руку. Течет кровь! Очень больно!",
},
{ //правую руку
    //царапина    
    "Тебе обожгло правую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    неопасное
    "Тебе обожгло правую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    коварное
    "Тебе обожгло правую руку как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    опасное
    "Тебе обожгло правую руку как огнем. Похоже, ранение серьезное, кровь так и течет",
    //    критическое
    "Тебя серьезно ранило в правую руку. Течет кровь! Очень больно!",
},
{ //левую ногу
    //царапина    
    "Тебе обожгло левую ногу как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    неопасное
    "Тебе обожгло левую ногу как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    коварное
    "Тебе обожгло левую ногу как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    опасное
    "Тебе обожгло левую ногу как огнем. Похоже, ранение серьезное, кровь так и течет",
    //    критическое
    "Тебя серьезно ранило в левую ногу. Течет кровь! Очень больно!",
},
{ //правую ногу
    //царапина    
    "Тебе обожгло правую ногу как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    неопасное
    "Тебе обожгло правую ногу как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    коварное
    "Тебе обожгло правую ногу как огнем. Довольно-таки больно, но похоже ничего важного не задето.",
    //    опасное
    "Тебе обожгло правую ногу как огнем. Похоже, ранение серьезное, кровь так и течет",
    //    критическое
    "Тебя серьезно ранило в правую ногу. Течет кровь! Очень больно!",
},

{ //Ранение в корпус
    //царапина
    "Выстрел задел лишь вскользь, почти не больно",
    //    неопасное 
    "Похоже, тебя подстрелили! Острая боль в груди",
    //    коварное 
    "Похоже, тебя подстрелили! Острая боль в груди",
    //    опасное
    "Выстрел задел лишь вскользь, почти не больно",
    //    критическое
    "Похоже, тебя подстрелили! Острая боль в груди Сколько крови!",
},
{ //Ранение в живот
    //царапина
    "Выстрел задел лишь вскользь, почти не больно",
    //    неопасное 
    "Похоже, тебя подстрелили! Острая боль в животе",
    //    коварное 
    "Выстрел задел лишь вскользь, почти не больно",
    //    опасное
    "Похоже, тебя подстрелили! Острая боль в животе",
    //    критическое
    "Похоже, тебя подстрелили! Острая боль в животе Сколько крови!",
},
{ //Ранение в спину
    //царапина
    "Выстрел задел лишь вскользь, почти не больно",
    //    неопасное 
    "Похоже, тебя подстрелили! Острая боль в спине",
    //    коварное 
    "Выстрел задел лишь вскользь, почти не больно",
    //    опасное
    "Похоже, тебя подстрелили! Острая боль в спине",
    //    критическое
    "Похоже, тебя подстрелили! Острая боль в спине Сколько крови!",
},

{ //ранение в голову
    //царапина
    "Выстрел задел лишь вскользь, почти не больно",
    //    неопасное
    "Похоже, тебя подстрелили!  У тебя здоровенная рана. Болит.",
    //    коварное
    "Похоже, тебя серьезно подстрелили!  У тебя здоровенная рана. Кровь застилает глаза и почти ничего не видно. Как минимум - ужасный шрам на всю жизнь!",
    //    опасное
    "Тебя сбили с ног! Голова сильно болит",
    //    критическое
    "Все очень серьезно. Рана такая, что страшно смотреть. Очень больно.  Даже не знаешь, сможешь ли выкарабкаться.",
},
{ //Последствия нокаута
    //царапина
    "Тебя сбили с ног! Голова  болит",
    //    неопасное
    "Тебя сбили с ног! Голова  болит.",
    //    коварное
    "Тебя сбили с ног! Голова болит болит",
    //    опасное
    "Тебя сбили с ног! Голова сильно болит",
    //    критическое
    "Тебя сбили с ног! Голова ужасно болит",
},
{ //Ударная волна (взрыв в отсеке)
    //царапина
    "Сильный невидимый кулак ударил тебя, но, вроде бы, все в порядке.",
    //    неопасное
    "Тебя как будто ударил невидимый кулак. Больно, но жить будешь.",
    //    коварное
    "Тебя как будто ударил невидимый кулак. Больно, но жить будешь.",
    //    опасное
    "Тебя как будто ударил невидимый кулак. Ты никак не можешь прийти в себя. Похоже, дело серьезное.",
    //    критическое
    "Тебя как будто ударил пневматический молот. Тебе очень больно, и, кажется, ты больше не боец.",
},
{ //Последствия нокаута
    //царапина
    "Ожог-0",
    //    неопасное
    "Ожог-1",
    //    коварное
    "Ожог-2",
    //    опасное
    "Ожог-1",
    //    критическое
    "Ожог-4",
},
{ //Последствия нокаута
    //царапина
    "Ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые вкоре потухли.,",
    //    неопасное
    "Ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые вкоре потухли.,",
    //    коварное
    "Ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые вкоре пропали.,",
    //    опасное
    "Ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые долго не пропадали.,",
    //    критическое
    "Ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые долго не пропадали.",
},
};
COMPILE_TIME_CHECK(sizeof(WoundEffects)/sizeof(char*)==MaxWoundType*MaxDamageSeverity);
#pragma endregion

const DAMAGE_SEVERITY RandomSelectPerTenPercent[10] =
{ //for random number from 0..9
	//Graze, Graze, Graze, Graze, 
	Light, Light, Light, Light,		//40% царапина
	Light, Light, Light,			//30% неопасное
	Insidious, Insidious,			//20% коварное
	Serious							//10% опасное
};
COMPILE_TIME_CHECK(sizeof(RandomSelectPerTenPercent)/sizeof(DAMAGE_SEVERITY)==10);

DAMAGE_EFFECT WoundToDamageEffect[MaxWoundType] =
{ 
	Rupture, Rupture, Rupture, Rupture,
	Rupture, Rupture, Rupture, Rupture,
	Blow, Blow, Thermal, Radiation
};

int sdeToPain[MaxDamageEffect][MaxDamageSeverity] = {
	{1,1,1,2,3},
	{1,1,1,2,3},
	{0,0,0,0,0},
	{0,0,0,0,0},
};
int sdeToDisfnLevel[MaxDamageEffect][MaxDamageSeverity] = {
	{0,0,1,1,2},
	{0,0,1,1,2},
	{0,0,1,1,2},
	{0,0,1,1,2},
};
int sdeToBloodLoss[MaxDamageEffect][MaxDamageSeverity] = {
	{5/*000*/,8/*000*/,10/*000*/,20/*000*/,50/*000*/}, //Rupture
	{5/*000*/,8/*000*/,10/*000*/,20/*000*/,50/*000*/}, //Blow
	{0,0,0,0,0},	//Thermal - FEATURE CUT
	{10,10,20,30,40},	//Radiation
};
int sdeToBleeding[MaxDamageEffect][MaxDamageSeverity] = {
	{0,1/*000*/,1/*000*/,5/*000*/,10/*000*/},
	{0,1/*000*/,1/*000*/,5/*000*/,10/*000*/},
	{0,0,0,0,0},
	{0,0,0,0,0},	//Radiation
};
int sdeToToxinsAdd[MaxDamageEffect][MaxDamageSeverity] = {
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{2/*000*/,5/*000*/,10/*000*/,15/*000*/,25/*000*/},	//Radiation
};
int sdeToToxinating[MaxDamageEffect][MaxDamageSeverity] = {
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,1/*000*/,1/*000*/,5/*000*/,10/*000*/},	//Radiation
};

WOUND_DESC WoundDescs[MaxWoundType][MaxDamageSeverity];

void InitWounds()
{
	for (int i=0;i<MaxWoundType;i++)
		for(int j=0;j<MaxDamageSeverity;j++)
	{
		WoundDescs[i][j].target = (TARGET)i;
		WoundDescs[i][j].severity = (DAMAGE_SEVERITY)j;
		WoundDescs[i][j].message = WoundEffects[i][j];
		
		DAMAGE_EFFECT de = WoundToDamageEffect[i];

		WoundDescs[i][j].PainLevel = (PAIN_LEVEL)sdeToPain[de][j];
		WoundDescs[i][j].DisfnLevel = (DISFUNCTION_LEVEL)sdeToDisfnLevel[de][j];
		WoundDescs[i][j].BloodLoss = sdeToBloodLoss[de][j];
		WoundDescs[i][j].Bleeding = sdeToBleeding[de][j];
		WoundDescs[i][j].ToxinsAdd = sdeToToxinsAdd[de][j];
		WoundDescs[i][j].Toxinating = sdeToToxinating[de][j];
	}
}

DAMAGE_SEVERITY NextCategory(DAMAGE_SEVERITY curr)
{
	if (curr <= None) return None;
	if (curr == Graze) return None;
	if (curr == Light) return Graze;
	if (curr == Insidious) return Serious;
	if (curr == Serious) return Critical;
	if (curr == Critical) return Critical;
	return None;
}

DAMAGE_SEVERITY DecreaseCategory(DAMAGE_SEVERITY curr)
{
	if (curr <= None) return None;
	if (curr == Graze) return None;
	if (curr == Light) return None;
	if (curr == Insidious) return Light;
	if (curr == Serious) return Light;
	if (curr == Critical) return Serious;
	return None;
}

DAMAGE_SEVERITY IncreaseCategory(DAMAGE_SEVERITY curr)
{
	if (curr <= None) return RandomSelectPerTenPercent[ArmletApi::GetRandom(10)];
	if (curr == Graze) return Serious;
	if (curr == Light) return Serious;
	if (curr == Insidious) return Critical;
	if (curr == Serious) return Critical;
	if (curr == Critical) return Critical;
	return None;
}

DAMAGE_SEVERITY NextCategory(DAMAGE_SEVERITY* curr) {*curr = NextCategory(*curr); return *curr;}
DAMAGE_SEVERITY DecreaseCategory(DAMAGE_SEVERITY* curr) {*curr = DecreaseCategory(*curr); return *curr;}
DAMAGE_SEVERITY IncreaseCategory(DAMAGE_SEVERITY* curr) {*curr = IncreaseCategory(*curr); return *curr;}

void ApplyWound(int wound,int ds, PPART part)
{
	Body.BloodCapacity -= WoundDescs[wound][ds].BloodLoss;
	part->Bleeding = WoundDescs[wound][ds].Bleeding;
	part->DisfnLevel = WoundDescs[wound][ds].DisfnLevel;
	part->PainLevel = WoundDescs[wound][ds].PainLevel;
	part->RemainingTicks = MED_MEGA_TICK;
}

}//namespace
