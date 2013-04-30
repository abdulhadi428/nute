#pragma once

namespace LowLevel {
	#include "Common.h"

	typedef void (__stdcall VOID_ROUTINE)();
	typedef void (__stdcall INT_STR_ROUTINE)(int n, char const*const str);

	void LowLevelLibMain(void);
	void RegisterCallbacks(
		VOID_ROUTINE* pVibro, 
		VOID_ROUTINE* pDecreasePillCharges,
		INT_STR_ROUTINE* pInvokeLog,
		INT_STR_ROUTINE* pSetCureName);

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
	extern VOID_ROUTINE* DecreasePillCharges;						//ARMLET emulation	WritePill
	extern VOID_ROUTINE* Vibro;										//ARMLET emulation	DoVibro

	//LOG EMULATION
	extern INT_STR_ROUTINE* InvokeLog;								//LOG emulation special processing
																	//	of "log.txt"											

	//SERVER EMULATION
	// OnRadioPacket / SendRadioPacket
	void SetPlayerName(char const*const name);						//SERVER emulation
	void SetRegenerationLevel(int level);							//SERVER emulation
	void SetLockAccess(int lock_id, bool bAllow);					//SERVER emulation
	void SendMessage(char const*const msg);							//SERVER emulation
	void Explosion(int room_id);									//SERVER emulation

	//SPECIAL
	//input
	void NextMedTick();												//internal emulation
	//output
	extern INT_STR_ROUTINE* SetCureName;							//internal emulation

} //namespace
