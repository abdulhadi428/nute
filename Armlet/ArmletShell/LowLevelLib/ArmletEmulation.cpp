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

} //namespace

namespace ArmletApi {

	//syscall returns 0-100%
	unsigned char GetBatteryLevel()
	{
		return (unsigned char)(GetRunningTime() / 100);
		return 100; //TODO
	}

	//draws a pixel
	void DrawPixel(int x, int y, unsigned short c)
	{
		if (x<0) return;
		if (y<0) return;
		if (x>=SCREENX) return;
		if (y>=SCREENY) return;
		LowLevel::VideoMemory[SCREENX*y+x] = c;
	}

	//draws an area of pixels
	void DrawArea(int x, int y, unsigned short *const pc, int len,
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
	void DoVibro(int msecs)
	{
		UNREFERENCED_PARAMETER(msecs);
		LowLevel::Vibro();
	}

	//writes cure to pill
	bool WritePill(int cure_id, int charges)
	{
		LowLog("Cure %d was written %d charges", cure_id, charges);
		LowLevel::DecreasePillCharges();
		return true;
	}

	//returns lustra id (UNKNOWN_ID=none)
	unsigned char GetLustraId()
	{
		if (bLustraInRange)
			return LastLustraId;
		return UNKNOWN_ID;
	}

	//return lock id (UNKNOWN_ID=none)
	unsigned char GetLockId()
	{
		return CurrentLockId;
	}

	//request lock to open
	bool OpenLock()
	{
		return true;
	}

	//request lock to show access denied
	bool DenyLock()
	{
		return true;
	}

} //namespace
