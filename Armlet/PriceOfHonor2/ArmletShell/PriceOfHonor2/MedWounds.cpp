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
    "Тебе обожгло левую руку как огнем. Похоже, ранение серьезное, кровь так и течет.",
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
    "Тебе обожгло правую руку как огнем. Похоже, ранение серьезное, кровь так и течет.",
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
    "Тебе обожгло левую ногу как огнем. Похоже, ранение серьезное, кровь так и течет.",
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
    "Тебе обожгло правую ногу как огнем. Похоже, ранение серьезное, кровь так и течет.",
    //    критическое
    "Тебя серьезно ранило в правую ногу. Течет кровь! Очень больно!",
},

{ //Ранение в корпус
    //царапина
    "Выстрел задел лишь вскользь, почти не больно.",
    //    неопасное 
    "Похоже, тебя подстрелили! Боль в груди.",
    //    коварное 
	"Выстрел задел лишь вскользь. Боль в груди.",
    //    опасное
    "Похоже, тебя подстрелили! Острая боль в груди.",
    //    критическое
    "Похоже, тебя подстрелили! Острая боль в груди. Сколько крови!",
},
{ //Ранение в живот
    //царапина
    "Выстрел задел лишь вскользь, почти не больно.",
    //    неопасное 
    "Похоже, тебя подстрелили! Боль в животе.",
    //    коварное 
    "Выстрел задел лишь вскользь. Боль в животе.",
    //    опасное
    "Похоже, тебя подстрелили! Острая боль в животе.",
    //    критическое
    "Похоже, тебя подстрелили! Острая боль в животе. Сколько крови!",
},
{ //Ранение в спину
    //царапина
    "Выстрел задел лишь вскользь, почти не больно.",
    //    неопасное 
    "Похоже, тебя подстрелили! Боль в спине.",
    //    коварное 
    "Выстрел задел лишь вскользь. Боль в спине.",
    //    опасное
    "Похоже, тебя подстрелили! Острая боль в спине.",
    //    критическое
    "Похоже, тебя подстрелили! Острая боль в спине. Сколько крови!",
},

{ //ранение в голову
    //царапина
    "Выстрел задел лишь вскользь, почти не больно.",
    //    неопасное
    "Похоже, тебя подстрелили! У тебя здоровенная рана. Голова раскалывается.",
    //    коварное
    "Выстрел задел лишь вскользь, голова раскалывается.",
    //    опасное
    "Похоже, тебя серьезно подстрелили! У тебя здоровенная рана. Очень больно. Кровь застилает глаза и почти ничего не видно. Как минимум - ужасный шрам на всю жизнь!",
    //    критическое
    "Все очень серьезно. Рана такая, что страшно смотреть. Очень больно. Даже не знаешь, сможешь ли выкарабкаться.",
},
{ //Последствия нокаута
    //царапина
    "Тебя сбили с ног! Вставай!..",
    //    неопасное
    "Тебя сбили с ног! Голова болит.",
    //    коварное
    "Тебя сбили с ног! Голова болит.",
    //    опасное
    "Тебя сбили с ног! Голова сильно болит.",
    //    критическое
    "Тебя сбили с ног! Голова ужасно болит.",
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
{ //Ожог (взрыв в отсеке)
    //царапина
    "Раскаленные капли металла пролетели мимо. Кроме одной. Острая боль почти утихла!",
    //    неопасное
    "Почти все раскаленные капли металла пролетели мимо. Почти! Острая боль быстро утихла. Жжется!",
    //    коварное
    "Раскаленные капли металла пролетели мимо. Кроме пары. Острая боль почти утихла!",
    //    опасное
    "Тебя зацепило раскалленое облако капель металла. Острая боль заставляет кричать!",
    //    критическое
    "Тебя накрыло раскалленое облако капель металла. Ты лежишь в позе эмбриона. Мир - это боль. Боль - это ты!",
},
{ //Где-то вдалеке произошел выброс из реактора (взрыв в отсеке)
    //царапина
    "Вдалеке ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые вкоре потухли.",
    //    неопасное
    "Вдалеке ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые вкоре потухли.",
    //    коварное
    "Вдалеке ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые вкоре пропали.",
    //    опасное
    "Вдалеке ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые долго не пропадали.",
    //    критическое
    "Вдалеке ты слышишь сильный грохот. На твоей одежде появились светящиеся пятна, которые долго не пропадали.",
},
};
COMPILE_TIME_CHECK(sizeof(WoundEffects)/sizeof(char*)==MaxWoundType*MaxDamageSeverity);
#pragma endregion

const DAMAGE_SEVERITY RandomSelectPerTenPercent[10] =
{ //for random number from 0..9
	Graze, Graze, Graze, Graze,		//40% царапина
	Light, Light, Light,			//30% неопасное
	Insidious, Insidious,			//20% коварное
	Serious							//10% опасное
};
COMPILE_TIME_CHECK(sizeof(RandomSelectPerTenPercent)/sizeof(DAMAGE_SEVERITY)==10);

const DAMAGE_EFFECT WoundToDamageEffect[MaxWoundType] =
{ 
	RuptureLimb, RuptureLimb, RuptureLimb, RuptureLimb,	//LimbShot
	Rupture, Rupture, Rupture, Rupture,					//other shots
	Blow,												//KnockOut
	Blow, Thermal, Radiation							//Explosion
};

int sdeToPain[MaxDamageEffect][MaxDamageSeverity] = {
	{1,1,1,2,3}, //RuptureLimb
	{1,2,2,3,3}, //Rupture
	{0,1,1,2,3}, //Blow
	{1,2,1,3,4}, //Thermal
	{0,0,0,0,0}, //Radiation
};
COMPILE_TIME_CHECK(sizeof(sdeToPain)/sizeof(int)==MaxDamageEffect*MaxDamageSeverity);

int sdeToBloodLoss[MaxDamageEffect][MaxDamageSeverity] = {
	{10, 15, 20, 25, 40},	//RuptureLimb
	{20, 25, 30, 35, 50},	//Rupture
	{0, 1, 1, 5, 15},		//Blow
	{10, 15, 20, 25, 40},		//Thermal
	{20, 25, 30, 35, 50},		//Radiation
};
int sdeToBleeding[MaxDamageEffect][MaxDamageSeverity] = {
	{0,1,5,7,10},	//RuptureLimb
	{0,1,5,7,10},	//Rupture
	{0,0,3,5,7},	//Blow
	{0,0,3,5,7},	//Thermal
	{0,1,5,7,10},	//Radiation
};
int sdeToToxinsAdd[MaxDamageEffect][MaxDamageSeverity] = {
	{0,0,0,0,0},	//RuptureLimb
	{0,0,0,0,0},	//Rupture
	{0,0,0,0,0},	//Blow
	{0,5,5,10,15},	//Thermal
	{5,10,10,20,30},//Radiation
};
int sdeToToxinating[MaxDamageEffect][MaxDamageSeverity] = {
	{0,0,0,0,0},	//RuptureLimb
	{0,0,0,0,0},	//Rupture
	{0,0,0,0,0},	//Blow
	{0,1,2,3,5},	//Thermal
	{0,1,1,5,10},	//Radiation
};
int sdeToNecroPoints[MaxDamageEffect][MaxDamageSeverity] = {
	{0,5,15,15,25},		//RuptureLimb
	{0,10,25,25,40},	//Rupture
	{0,5,10,10,15},		//Blow
	{0,20,30,45,60},	//Thermal
	{0,20,30,45,60},	//Radiation
};

WOUND_DESC WoundDescs[MaxWoundType][MaxDamageSeverity];

void InitWounds()
{
	for (int i=0;i<MaxWoundType;i++)
		for(int j=0;j<MaxDamageSeverity;j++)
	{
		WoundDescs[i][j].message = WoundEffects[i][j];

		WoundDescs[i][j].target = (TARGET)i;
		WoundDescs[i][j].severity = (DAMAGE_SEVERITY)j;
		
		DAMAGE_EFFECT de = WoundToDamageEffect[i];
		WoundDescs[i][j].de = de;

		WoundDescs[i][j].PainLevel = sdeToPain[de][j];
		WoundDescs[i][j].BloodLoss = sdeToBloodLoss[de][j];
		WoundDescs[i][j].Bleeding = sdeToBleeding[de][j];
		WoundDescs[i][j].ToxinsAdd = sdeToToxinsAdd[de][j];
		WoundDescs[i][j].Toxinating = sdeToToxinating[de][j];
		WoundDescs[i][j].NecroPoints = sdeToNecroPoints[de][j];
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

void ApplyWound(int wound, int ds, PPART part)
{
	if (ds==None) {
		part->wound = UnknownTarget;
		part->CurrSeverity = None;
		part->RemainingTicks = 0;
		part->PainLevel = 0;
		part->Bleeding = 0;
		part->Toxinating = 0;
		return;
	}

	part->wound = wound;
	part->CurrSeverity = (DAMAGE_SEVERITY)ds;			

	Body.BloodCapacity -= WoundDescs[wound][ds].BloodLoss;
	Body.ToxinsCapacity += WoundDescs[wound][ds].ToxinsAdd;
	part->NecroPoints += WoundDescs[wound][ds].NecroPoints;

	part->Bleeding = WoundDescs[wound][ds].Bleeding;
	part->Toxinating = WoundDescs[wound][ds].Toxinating;
	part->PainLevel = WoundDescs[wound][ds].PainLevel;

	part->RemainingTicks = MED_MEGA_TICK;
}

}//namespace
