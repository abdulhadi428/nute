#pragma once

#include "Osanve.h"

// статичная общая конфигурация - таблица браслетов
namespace config {
#define MAX_ARMLET 255
	struct config {
		ubyte_t userId; // игрок
		ubyte_t groupId; // группа игроков
		ubyte_t maxForce; // макс силы
		bool evil; // сторона силы
		ubyte_t fph; // скорость восстановления силы (force per hour)
		char name[16]; // имя персонажа
		char fname[16]; //боевое имя персонажа
		sword_t rights; // права браслета (см. AL_CAN в Osanve.h)
	};

/// заполнение структуры игрока по ID инфой из таблицы
/// false - ошибка (неверный ID)
bool InitPlayer(struct player* p);
}

using namespace config;

extern struct config CONF[MAX_ARMLET];
