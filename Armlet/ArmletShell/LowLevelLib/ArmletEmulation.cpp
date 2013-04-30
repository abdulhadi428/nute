#include "DownInterface.h"
#include "ArmletApi.h"
#include "Helper.h"

static bool bLustraInRange = false;
static unsigned char LastLustraId = UNKNOWN_ID;
static unsigned char CurrentLockId = UNKNOWN_ID;

namespace LowLevel {
	//ARMLET emulation	DrawPixel/DrawArea
	unsigned short VideoMemory[SCREENX*SCREENY];

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

	//SPECIAL PLATFORM DEPENDENT
	void NextMedTick()
	{
		ArmletApi::NextMedTick();
	}

} //namespace

namespace ArmletApi {

	//__SYSCALL returns 0-100%
	unsigned char __SYSCALL GetBatteryLevel()
	{
		unsigned char p = (unsigned char)(GetRunningTime() / 100);
		if (p>100) p = 100;
		return p;
	}

	//draws a pixel
	void __SYSCALL DrawPixel(int x, int y, unsigned short c)
	{
		if (x<0) return;
		if (y<0) return;
		if (x>=SCREENX) return;
		if (y>=SCREENY) return;
		LowLevel::VideoMemory[SCREENX*y+x] = c;
	}

	//draws an area of pixels
	void __SYSCALL DrawArea(int x, int y, unsigned short *const pc, int len,
					int sx, int sy)
	{
		if (x<0) return;
		if (y<0) return;
		if (sx<=0) return;
		if (sy<=0) return;
		if (sx*sy!=len) return;
		int k=0;
		for(int j=0; j<sy; j++)
		{
			for (int i=0; i<sx; i++)
			{
				int px=x+i;
				int py=y+j;
				if (px>=SCREENX) return;
				if (py>=SCREENY) return;
				DrawPixel(px,py,pc[k++]);
			}
		}
	}

	//performs vibration for duration
	void __SYSCALL DoVibroAndBeep(int msecs)
	{
		UNREFERENCED_PARAMETER(msecs);
		LowLevel::Vibro();
	}

	//writes cure to pill
	bool __SYSCALL WritePill(int cure_id, int charges)
	{
		LowLog("Cure %d was written %d charges", cure_id, charges);
		LowLevel::DecreasePillCharges();
		return true;
	}

	//returns lustra id (UNKNOWN_ID=none)
	unsigned char __SYSCALL GetLustraId()
	{
		if (bLustraInRange)
			return LastLustraId;
		return UNKNOWN_ID;
	}

	//return lock id (UNKNOWN_ID=none)
	unsigned char __FUTURE GetLockId()
	{
		return CurrentLockId;
	}

	//request lock to open
	bool __FUTURE OpenLock()
	{
		return true;
	}

	//request lock to show access denied
	bool __FUTURE DenyLock()
	{
		return true;
	}

	//SPECIAL PLATFORM DEPENDENT
#ifdef _MSC_VER
	void __SYSCALL SetCureName(int cure_id, char* name)
	{
		LowLevel::SetCureName(cure_id, name);
	}
#endif

} //namespace
