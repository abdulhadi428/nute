#include "stdafx.h"
#include "LowLevel.h"
#include "OsanveProtocol.h"

namespace ArmletApi {

	//__SYSCALL returns 0-100%
	unsigned char __SYSCALL GetBatteryLevel()
	{
		unsigned char p = (unsigned char)(100 - (GetUpTime() / 1000)); //1% per sec
		if (p>100) p = 100;
		return p;
	}

	//__SYSCALL sets screen brightness
	void __SYSCALL SetScreenBrightness(unsigned char percent)
	{
		LowLevel::LowLog("Screen Bightness was set to %d",percent);
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
		LowLevel::LowLog("Cure %d was written %d charges", cure_id, charges);
		if ((cure_id < 0)||(cure_id > 14))
			return false;
		LowLevel::CureCharges[cure_id] = charges;
		LowLevel::UpdateCurrentCure();
		return true;
	}

	//returns lustra id (UNKNOWN_ID=none)
	unsigned char __SYSCALL GetLustraId()
	{
		if (LowLevel::bLustraInRange)
			return LowLevel::LastLustraId;
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
		if (!packet || len != PACKET_LEN) {
			LowLevel::LowLog("PROTOCOL: len != PACKET_LEN");
			return;
		}
		if (IS_ATACK_PACKET(packet)) {
			struct ATACK_PACKET* ap = (struct ATACK_PACKET*)packet;
			LowLevel::LowLog("PROTOCOL: receive ATACK packet from %d to %d atack=%d and cons=%d",
				ap->src, ap->dst, ap->atack, GET_CONS(ap->flags));
		}else {
		    struct OSANVE_PACKET* op = (struct OSANVE_PACKET*)packet;
			LowLevel::LowLog("PROTOCOL: receive OSANVE packet from %d forces=%d/%d and osanve=%d",
				op->src, op->force, op->maxForce, op->flags&OSANVE_MASK);
		}
		return; 
		//TODO timer/list helper
	}

	void __SYSCALL SendAppState(unsigned char packet[APP_STATE_LEN]) {
		UNREFERENCED_PARAMETER(packet);
		return; //TODO implement
	}

	void __SYSCALL GetRadioStatus(int* gate_id, int* signal_level)
	{
		*gate_id = 3;
		*signal_level = -35;
		return;
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