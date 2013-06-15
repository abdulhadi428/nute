#include "ArmletApi.h"
#include "ArmletShell.h"
#include "AppMain.h"
#include "UserInterface.h"
#include "Osanve.h"
#include "conf.h"

extern UserInterface UI;

//namespace osanve {

ArmletApi::FILE PlayerFile; // файл динамической конфигурации игрока
struct player Player; // структура описания игрока
struct nick FGHT[MAX_ARMLET]; // массив сведений о других встреченных браслетах

// Установить скорость регенерации сил
void SetFPH(ubyte_t newFPH)
{
	if ((newFPH<1)/*||(regenRate>3)*/) return;
	Player.fph = newFPH;
	return;
}

// читаем начальные установки
// Для инициализации браслета записать в файл player.bin 1 байт - ID персонажа
void PlayerInit()
{
	int sz = 0;
	bool ret = ArmletApi::OpenFile(&PlayerFile, "player.bin",false);
	if (ret) {
		sz = ArmletApi::ReadFile(&PlayerFile,(char*)&Player,sizeof(player));
	} /* else {
		ArmletApi::OpenFile(&PlayerFile, "player.bin",true);
	} */
	if (sz!=sizeof(player)) { // если считан только байт userId
		ArmletApi::OpenFile(&PlayerFile, "player.bin",true);
		InitPlayer( &Player); // см. conf.cpp
	}
//	memset( OSNV, 0, MAX_ARMLET*sizeof(nick));
	memset( FGHT, 0, MAX_ARMLET*sizeof(nick));

}

// Тик таймера восстановления (1 min)
bool OnForceTick(void)
{
	bool rval = false;
//save
	if( ArmletApi::OpenFile(&PlayerFile, "player.bin",true) )
	  ArmletApi::WriteFile(&PlayerFile,(char*)&Player,sizeof(player));
// увеличение сил
  if( Player.force + Player.fph > Player.maxForce)
  	Player.force = Player.maxForce;
  else
  	Player.force += Player.fph;
// отсчет времени поражения
  if( Player.status == AL_STATUS_DEFEAT) {
  	if(Player.defeatTime > 0)
  		Player.defeatTime--;
  		// Последствие поражения закончилось
  	if(Player.defeatTime <= 0){
  		Player.status = AL_STATUS_OSANVE;
		Player.defeatTime = 0;
		rval = true;
	}
  }
  return rval;
}

// сформировать пакетик атаки
// только формирует пакет, не уменьшает силу персонажа
void GetAtackPacket(unsigned char* a_packet, ubyte_t a)
{
	if(!a_packet) return;
	// собираем пакетик
	*a_packet = Player.userId; // наш ID
	if(Player.status!=AL_STATUS_FIGHT)
	  return;
	*(a_packet+1) = 0x80 | ((Player.cons<<4)&0x70);
	*(a_packet+1) |= Player.atackSeq++;
	Player.atackSeq %= MAX_ATACK_SEQ;
	*(a_packet+2) = Player.enemyId;
    *(a_packet+3) = a; // наша атака, без проверки
}

// сформировать пакетик атаки
// только формирует пакет, не уменьшает силу персонажа
void GetHealPacket(unsigned char* h_packet, ubyte_t h)
{
	if(!h_packet) return;
	// собираем пакетик
	GetAtackPacket(h_packet,h);
	*(h_packet+1) |= FORCE_HEAL; // флаг лечения
}

// сформировать пакетик осанве
void GetOsanvePacket(unsigned char* osanve_packet)
{	
	if(!osanve_packet) return;
	// собираем пакетик
	*osanve_packet = Player.userId; // наш ID
	switch(Player.status) {
		case AL_STATUS_FIGHT:
		case AL_STATUS_DEFENSE:
		case AL_STATUS_PREPARE: *(osanve_packet+1) = OSANVE_FGHT;
			break;
	    case AL_STATUS_DEFEAT: *(osanve_packet+1) = OSANVE_FEAR;
			break;
		default: *(osanve_packet+1) = Player.osanve; // наш статус
			break;
	}
	*(osanve_packet+2) = Player.force;  // наша сила
	*(osanve_packet+3) = (Player.status==AL_STATUS_DEFEAT)?(0):(Player.maxForce); // наш максимум силы
	// флаг зла во 2 байте
	if (Player.evil) *(osanve_packet+1) |= FORCE_SIDE_EVIL;
}

// закончить подготовку к битве (сработал таймер)
bool PreFightEnd(void)
{ // игрок уже может быть в защите, тогда ничего не делаем
	if( Player.status != AL_STATUS_PREPARE)
		return false;
	
	Player.status = AL_STATUS_FIGHT;
	
	Atacked( Player.penalty, Player.penaltyCons, Player.enemyId);
	
	Player.penalty = 0;
	
	return false;
}

// ??
//void _osanveAfterWoundUpdate(char** oSymptoms)
//{
//	GatherDescs(buf1, sizeof(buf1)-1);
//	*oSymptoms = buf1;
//}

//simple healing
ubyte_t Healed(ubyte_t h)
{ // лечение срабатывает в любом состоянии, но не выводит из последствия
	if( (h + Player.force) > Player.maxForce )
		Player.force = Player.maxForce;
	else
		Player.force += h;
  return Player.force;
}

// атакует кого-то с силой а
ubyte_t Atack(ubyte_t a)
{ // проверка боевого режима
	if( Player.status == AL_STATUS_FIGHT)
	{
		if( a > Player.force)
			a = Player.force;
		Player.force -= a;
	}
	return Player.force;
}

// атакован кем-то enemyId с силой а и последствием cons
// cons и enemyId нужны для запоминания при переходе в предбоевой режим
ubyte_t Atacked(ubyte_t a, ubyte_t cons, ubyte_t enemyId)
{
	ubyte_t atack = 0;
	if(cons>CONS_COUNT)
		cons = 0;

	Player.enemyId = enemyId; // от кого

	switch( Player.status){
		case AL_STATUS_DEFENSE: // в защите
		{ // сила атаки не может быть меньше 5
			atack = (a + 5) / 10;
			break;
		}
		case AL_STATUS_FIGHT: // в бою
		{ // сила атаки не может быть меньше 5
			atack = a;
			break;
		}
		case AL_STATUS_DEFEAT: // уже поражен, в последствии
		{ // сила атаки не может быть меньше 5
			atack = 0;
			break;
		}
		default: // все остальные состояния: перевод в предбоевой режим
		{ // сила атаки не может быть меньше 5
			atack = 0; // Сейчас игрок не получает ничего
			if( (Player.penalty + a) > FORCE_MAX) // потом получит сполна
			  Player.penalty = FORCE_MAX;
			else
			  Player.penalty += a;
			Player.penaltyCons = cons; // и последствие
			Player.status = AL_STATUS_PREPARE;
			//Launch PreFight Timer
	    ArmletApi::RequestTimer(_PreFightTimerCallback, PREFIGHT_TIME);
		//////////////////////////////////////////////////////////////////
		/////////////// ВСТАВИТЬ ПРОРИСОВКУ ПРЕДБОЕВОГО РЕЖИМА ///////////
		//////////////////////////////////////////////////////////////////
			break;
		}
	}

// Расчитываем потерю очков
	if( Player.force < atack)
	{
		Player.force = 0;
		// поражение, переход в последствие
		Player.status = AL_STATUS_DEFEAT;
		Player.penalty = 0;
		Player.penaltyCons = cons; // запоминаем последствие
		Player.defeatTime = DEFAULT_DEFEAT_TIME;
		//////////////////////////////////////////////////////////////////
		/////////////// ВСТАВИТЬ ПРОРИСОВКУ ПОСЛЕДСТВИЯ //////////////////
		//////////////////////////////////////////////////////////////////
	} else
		Player.force -= atack;
	
	return Player.force;
}

// Нажата кнопка защиты
void DefenceON(void)
{
	Player.status = AL_STATUS_DEFENSE;
	// В игрока прилетает все, что накопилось
	Atacked( Player.penalty, Player.penaltyCons, Player.enemyId);
	
	Player.penalty = 0;
}

// Отжата кнопка защиты
void DefenceOFF(void) { 
  Player.status = AL_STATUS_FIGHT;
}

// смена осанве
void SetOsanveNum(ubyte_t no)
{
	Player.osanve = no;
}

//#pragma region forUI


//temp
//char _buf[201];

//char* _osanveOnExplosion()
//{
//	int Target = ArmletApi::GetRandom(MaxTarget);
//	int DamageSeverity = IncreaseCategory(&Player.parts[Target].CurrSeverity);
//	int ExplosionType = ArmletApi::GetRandom(2);
//	if (ExplosionType == 1)
//		ExplosionType = 2; //skip scorch
//	ApplyWound(MaxTarget+1+ExplosionType,DamageSeverity,&Player.parts[Target]);
//	const char* msg =  WoundDescs[MaxTarget+1+ExplosionType][DamageSeverity].message;
////temp
////	ArmletApi::snprintf(_buf,200,"%s-%d\n%s",
////		TargetNames[MaxTarget+1+ExplosionType],
////		DamageSeverity,
////		msg);
////	return _buf;
//	return (char*)msg;
//}


//#pragma endregion

//} //namespace

