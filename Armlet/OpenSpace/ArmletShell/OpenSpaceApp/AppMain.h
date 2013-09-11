#pragma once

#define FIRMWARE_VERSION "0.9"

void AppMainThread(void* param);


void _OnPillConnected(sword_t cure_id, sword_t charges);
void _OnSetPlayerName(char* name);
void _OnExplosion(sword_t roomId, sword_t crap, sword_t crap1);
void _OnServerMessage(char* msg);
void _OnSetRegenerationRate(sword_t regenRate);


//callbacks
bool __CALLBACK _QueryLustraTimerCallback(int elapsed);
bool __CALLBACK _MedicineTimerTickCallback(int elapsed);
bool __CALLBACK _QueryBatteryStatusTimerCallback(int elapsed);


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