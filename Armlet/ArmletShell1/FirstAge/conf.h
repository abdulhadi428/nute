#pragma once

#include "Osanve.h"
#include "string.h"

// статичная общая конфигурация - таблица браслетов
//namespace configs {

	struct config {
		ubyte_t userId; // игрок
		ubyte_t groupId; // группа игроков
		ubyte_t maxForce; // макс силы
		bool evil; // сторона силы
		ubyte_t fph; // скорость восстановления силы (force per hour)
		char name[32]; // имя персонажа
		char fname[32]; //боевое имя персонажа
		sword_t rights; // права браслета (см. AL_CAN в Osanve.h)
		/*config() {};
		config(ubyte_t u,ubyte_t g,ubyte_t m,bool e,ubyte_t f,char* na, char* fna, sword_t r):
		   userId(u), groupId(g), maxForce(m), evil(e),fph(f), rights(r)
		   {strcpy(name,na); strcpy(fname,fna);};*/
	};

/// заполнение структуры игрока по ID инфой из таблицы
/// false - ошибка (неверный ID)
bool InitPlayer(struct player* p);
//}

//using namespace configs;

extern struct config CONF[MAX_ARMLET];
