#pragma once

void AppMainThread(void* param);

void _OnPillConnected(sword_t cure_id, sword_t charges);
void _SetPlayerName(char* name);
void _SetBatteryLevel(ubyte_t batLevel);
void _SetRoom(uword_t room);

void _OnExplosion(uword_t room);



//extern APP_TASK AppTasks[];
//BOOL InitializeLogic();

//TODO
//AppTask[]
//InputQueue
//OutputThread
//WorkQueue

//tasks, events, mutex

//Medicine:
//Body & Parts
//Damage
//Healing

//Location:
//Lustra
//Zamok

//Save/Load:
//Configuration
//Pills

//Communication:
//Messages
//Commands
//Notifications

//Screen:
//Texts
//Statuses
//Menus