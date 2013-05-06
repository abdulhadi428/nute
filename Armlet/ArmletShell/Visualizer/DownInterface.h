#pragma once
#include "Common.h"

namespace LowLevel {

	void LowLevelMain(void);										//ARMLET InitializeShell

	//ARMLET EMULTION
	//input
	void OnButtonPress(int button_id);								//ARMLET emulation same 
	void OnButtonRelease(int button_id);							//ARMLET emulation same
																	//(OnButtonClick/OnButtonHold is only inside MiddleWareLib)
	void OnPillConnect(int cure_id, int charges);					//ARMLET emulation	OnPillConnect
	void LustraInRange(bool bInRange);								//ARMLET emulation	GetLustraId
	void SetCurrentLustra(int lustra_id);							//ARMLET emulation	GetLustraId
	void SetCurrentLock(int lock_id);								//ARMLET emulation	GetLockId/OpenLock/DenyLock
	//output
	extern unsigned short VideoMemory[SCREENX*SCREENY];				//ARMLET emulation	DrawPixel/DrawArea
	typedef void (__stdcall VIBRO_CALLBACK)();
	extern VIBRO_CALLBACK* Vibro;									//ARMLET emulation	DoVibro

	//SERVER EMULATION
	// OnRadioPacket / SendRadioPacket
	void SetPlayerName(char const*const name);						//SERVER emulation
	void SetRegenerationLevel(int level);							//SERVER emulation
	void SetLockAccess(int lock_id, bool bAllow);					//SERVER emulation
	void SendMessage(char const*const msg);							//SERVER emulation
	void Explosion(int room_id);									//SERVER emulation

	//LOG EMULATION
	typedef void (__stdcall LOG_CALLBACK)(char* msg);
	extern LOG_CALLBACK* Log;										//LOG emulation special processing
																	//	of "log.txt"											
	//SPECIAL
	//input
	void CheckTimers();												//for timers emulation, in OsEmulation.c
	void NextMedTick();												//for medicine emulation
	int GetPillCharges(int cure_id);								//for medicine emulation
	//output
	typedef void (__stdcall SET_CURE_NAME_CALLBACL)(int cure_id, char* cure_name);
	extern SET_CURE_NAME_CALLBACL* SetCureName;						//for medicine emulation
	typedef void (__stdcall UPDATE_CURRENT_CURE_CALLBACK)();
	extern UPDATE_CURRENT_CURE_CALLBACK* UpdateCurrentCure;			//for medicine emulation
} //namespace
