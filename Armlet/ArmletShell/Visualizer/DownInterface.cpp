#include "stdafx.h"
#include "LowLevel.h"

namespace LowLevel {

	void LowLevelLibMain(void)
	{		
		if (!ArmletApi::InitializeShell()) {
			ArmletApi::CriticalError("InitializeShell failed");
		}
	}

	void RegisterCallbacks(
		VOID_ROUTINE* pVibro, 
		VOID_ROUTINE* pDecreasePillCharges,
		INT_STR_ROUTINE* pInvokeLog,
		INT_STR_ROUTINE* pSetCureName)
	{
		Vibro = pVibro;
		DecreasePillCharges = pDecreasePillCharges;
		InvokeLog = pInvokeLog;
		SetCureName = pSetCureName;
	}

	//ARMLET emulation same
	void OnButtonPress(int button_id)
	{
		ArmletApi::OnButtonPress(button_id);
	}

	//ARMLET emulation same
	void OnButtonRelease(int button_id)
	{
		ArmletApi::OnButtonRelease(button_id);
	}

	//ARMLET emulation	OnPillConnect
	void OnPillConnect(int cure_id, int charges)
	{
		ArmletApi::OnPillConnect(cure_id, charges);
	}

	//ARMLET emulation GetLustraId
	void LustraInRange(bool bInRange)
	{
		bLustraInRange = bInRange;
	}

	//ARMLET emulation	GetLustraId
	void SetCurrentLustra(int lustra_id)
	{
		if ((lustra_id < 0) || (lustra_id >= UNKNOWN_ID))
			LastLustraId = UNKNOWN_ID;
		LastLustraId = (unsigned char)lustra_id;
	}

	//ARMLET emulation	GetLockId/OpenLock/DenyLock
	void SetCurrentLock(int lock_id)
	{
		if ((lock_id <= 0) || (lock_id >= UNKNOWN_ID))
			CurrentLockId = UNKNOWN_ID;
		CurrentLockId = (unsigned char)lock_id;
	}

	//ARMLET emulation	DrawPixel/DrawArea
	unsigned short VideoMemory[SCREENX*SCREENY];
	//ARMLET emulation	WritePill
	VOID_ROUTINE* DecreasePillCharges = 0;
	//ARMLET emulation	DoVibro
	VOID_ROUTINE* Vibro = 0;

	//SERVER EMULATION
	void SetPlayerName(char const*const name)
	{
		//TODO
	}

	//SERVER EMULATION
	void SetRegenerationLevel(int level)
	{
		//TODO
	}

	//SERVER EMULATION
	void SetLockAccess(int lock_id, bool bAllow)
	{
		//TODO
	}

	//SERVER EMULATION
	void SendMessage(char const*const msg)
	{
		//TODO
	}

	//SERVER EMULATION
	void Explosion(int room_id)
	{
		//TODO
		ArmletApi::OnRadioPacket(0,0);
	}

	INT_STR_ROUTINE* InvokeLog = 0;
	INT_STR_ROUTINE* SetCureName = 0;

	//SPECIAL PLATFORM DEPENDENT
	void NextMedTick()
	{
		ArmletApi::NextMedTick();
	}

} //namespace
