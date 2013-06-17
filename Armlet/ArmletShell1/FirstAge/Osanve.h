#pragma once
#include "ArmletShell.h"
#include "conf.h"
// сообщение-статус осанве
#define OSANVE_COUNT 9 // число состояний
#define OSANVE_NORM 0 // все норм (по умолчанию)
#define OSANVE_CREA 1 // творю
#define OSANVE_GOOD 2 // радуюсь
#define OSANVE_HELP 3 // жду помощь
#define OSANVE_NOTH 4 // скучаю
#define OSANVE_FEAR 5 // подавлен
#define OSANVE_BAD  6 // страдаю
#define OSANVE_WOUN 7 // ранен
#define OSANVE_FGHT 8 // веду бой (можно исп как флаг)

// Отображение абонента в Осанве, с
#define OSANVE_MAX_WAIT_TIME 60
// Период рассылки своего состояния, мс
#define OSANVE_SEND_TIME 4*1000
// Время предбоевого режима, мс
#define PREFIGHT_TIME 10*1000
// Отображение игроков в боевом режиме без обновления, с
#define FIGHT_MAX_WAIT_TIME 30
// Период опроса заряда батарейки, мс
#define BATTERYSTATUS_POLL_TIME 10*1000
// Время нахождения в последствии, в циклах увеличения силы FORCE_TICK_TIME
#define DEFAULT_DEFEAT_TIME 20

// состояния браслета
#define AL_STATUS_OSANVE  0	//ОСАНВЕ, основной режим
#define AL_STATUS_OSSEL   1 // изменение своего осанве
#define AL_STATUS_COMBAT  2 // боевой режим
#define AL_STATUS_FIGHT   3 // режим атаки
#define AL_STATUS_DEFENSE 4 // режим защиты
#define AL_STATUS_CONSEL  5 // выбор последствия
#define AL_STATUS_DEFEAT  6 // наступившее последствие
#define AL_STATUS_PREPARE 7 // предбоевой

// сообщение-статус осанве
extern char* OSANVES[OSANVE_COUNT];

// Боевые константы
#define FORCE_MAX 255
#define FORCE_STEP 5
#define MAX_ATACK_SEQ 7

// последствия
#define CONS_COUNT  6
#define CONS_TO1HIT 0
#define CONS_MIRAGE 1
#define CONS_CRAZY  2
#define CONS_SAD    3
#define CONS_PAIN   4
#define CONS_BERSERK 5

// названия последствий
extern char* CONS[CONS_COUNT];
// описания последствий
extern char* CTEXT[CONS_COUNT];

// Разрешения-права (флаги)
#define AL_CAN_FIGHT  1 // Сражаться
#define AL_CAN_DEFSEL 2 // Выбирать последствия
#define AL_CAN_MASTER 4 // Управлять

//namespace osanve {
	struct player {
//		uword_t _armletId; // ID браслета
		ubyte_t userId; // игрок
		ubyte_t groupId; // группа игроков
		ubyte_t enemyId; // противник
		ubyte_t atackSeq; // номер следующей атаки
		ubyte_t atack;  // номер следующей атаки
		ubyte_t osanve; // сообщение-статус осанве
		ubyte_t cons;   // выбранное последствие
		ubyte_t status; // состояние браслета
		ubyte_t force; // сила
		ubyte_t maxForce; // макс силы
///		ubyte_t defPress; // была нажата кнопка
        ubyte_t penaltyCons;   // отложенное последствие
		ubyte_t penalty; // сила отложенной атаки на игрока (на время ввода в боевой режим)
		uword_t defeatTime; // оставшееся время нахождения в последствии, минуты!
		bool evil; // сторона силы
		ubyte_t fph; // скорость восстановления силы (force per minute)
		char name[32]; // имя персонажа
		char fname[32]; //боевое имя персонажа
		sword_t rights; // права браслета (AL_CAN)
	};

	void SetOsanveNum(ubyte_t no);
	bool OnForceTick(void); // true при выходе из последствия
  void GetOsanvePacket(unsigned char* osanve_packet); // формирование пакета осанве
  void GetAtackPacket(unsigned char* a_packet, ubyte_t a); // формирование пакета атаки
  void GetHealPacket(unsigned char* h_packet, ubyte_t h); // формирование пакета лечения
	bool PreFightEnd(void);
	void SetFPH(ubyte_t newFPH);
	ubyte_t Atack(ubyte_t a);
	ubyte_t Atacked(ubyte_t a, ubyte_t cons, ubyte_t enemyId);
	ubyte_t Healed(ubyte_t h);
  void DefenceON(void); // Нажата кнопка защиты
  void DefenceOFF(void); // Отжата кнопка защиты
	void PlayerInit(void);
/// заполнение структуры игрока по ID инфой из таблицы
/// false - ошибка (неверный ID)
   bool InitPlayer(struct player* p);

	struct nick { // Описывает момент получения отклика от игрока
		ubyte_t userId; // игрок
		ubyte_t force;
		ubyte_t maxForce;
		ubyte_t osanve; // сообщение-статус осанве
		int time; // милисекунда, на которой была получена инфа
	};
//}

//using namespace osanve;
// дин конф игрока
extern struct player Player;
// осанве
// extern struct nick OSNV[MAX_ARMLET];
// бойцы (фактически динамические параметры всех встреченных браслетов, индекс == userId)
extern struct nick FGHT[MAX_ARMLET];
