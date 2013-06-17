#pragma once

#include "string.h"

#define MAX_ARMLET 255
// статичная общая конфигурация - таблица браслетов
//namespace configs {

	struct config {
		unsigned char userId; // игрок
		unsigned char groupId; // группа игроков
		unsigned char maxForce; // макс силы
		bool evil; // сторона силы
		unsigned char fph; // скорость восстановления силы (force per hour)
		char name[32]; // имя персонажа
		char fname[32]; //боевое имя персонажа
		signed short rights; // права браслета (см. AL_CAN в Osanve.h)
		/*config() {};
		config(ubyte_t u,ubyte_t g,ubyte_t m,bool e,ubyte_t f,char* na, char* fna, sword_t r):
		   userId(u), groupId(g), maxForce(m), evil(e),fph(f), rights(r)
		   {strcpy(name,na); strcpy(fname,fna);};*/
	};



//using namespace configs;

extern struct config CONF[MAX_ARMLET];
