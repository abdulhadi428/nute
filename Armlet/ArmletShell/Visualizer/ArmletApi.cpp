#include "stdafx.h"
#include "LowLevel.h"

namespace ArmletApi {

	//__SYSCALL returns 0-100%
	unsigned char __SYSCALL GetBatteryLevel()
	{
		unsigned char p = (unsigned char)(100 - (GetUptime() / 100));
		if (p>100) p = 100;
		return p;
	}

	void __SYSCALL SetScreenBrightness(unsigned char percent)
	{
		LowLog("Screen Bightness was set to %d",percent);
		return; 
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
	//unsigned char __FUTURE GetLockId()
	//{
	//	return CurrentLockId;
	//}

	//request lock to open
	//bool __FUTURE OpenLock()
	//{
	//	return true;
	//}

	//request lock to show access denied
	//bool __FUTURE DenyLock()
	//{
	//	return true;
	//}

	void __SYSCALL SendRadioPacket(unsigned char* packet, int len) {
		return; //TODO implement
	}

	unsigned short __SYSCALL GetArmletId()
	{
		return 50;
	}

	//SPECIAL PLATFORM DEPENDENT
	void __SYSCALL SetCureName(int cure_id, char* name)
	{
		LowLevel::SetCureName(cure_id, name);
	}

} //namespace