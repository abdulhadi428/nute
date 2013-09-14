#pragma once

#define FIRMWARE_VERSION "0.9"

void AppMainThread(void* param);


void _OnPillConnected(ubyte_t pill_id, sword_t charges);
void _OnSetPlayerName(char* name);
void _OnExplosion(sword_t roomId, ubyte_t probability, ubyte_t explosionType);
void _OnServerMessage(char* msg);
void _OnSetRegenerationRate(sword_t regenRate);


//callbacks
bool __CALLBACK _QueryLustraTimerCallback(int elapsed);
bool __CALLBACK _MedicineTimerTickCallback(int elapsed);
bool __CALLBACK _QuerySystemStatusTimerCallback(int elapsed);


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