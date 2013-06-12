#pragma once

#define FIRMWARE_VERSION "0.7"

void AppMainThread(void* param);


// void _OnPillConnected(sword_t cure_id, sword_t charges);
void _OnSetPlayerName(char* name);
// void _OnExplosion(sword_t roomId);
// void _OnServerMessage(char* msg);
void _OnSetRegenerationRate(sword_t regenRate);
void _OnAtackPacket(ubyte_t enemyId, ubyte_t atack, ubyte_t cons);
void _OnHealPacket(ubyte_t enemyId, ubyte_t heal);
void _OnOsanvePacket(ubyte_t userId, ubyte_t force,ubyte_t maxForce, ubyte_t osanve);

//callbacks
bool __CALLBACK _OsanveSendTimer(int elapsed);
bool __CALLBACK _ForceTimerTickCallback(int elapsed);
bool __CALLBACK _QueryBatteryStatusTimerCallback(int elapsed);
bool __CALLBACK _PreFightTimerCallback(int elapsed);

