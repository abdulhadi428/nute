#include "ArmletKernel.h"
#include "ServerProtocol.h"

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
		ArmletKernel::LowLog("Screen Bightness was set to %d",percent);
		return; 
	}

	//draws a pixel
	void __SYSCALL DrawPixel(int x, int y, unsigned short c)
	{
		if (x<0) return;
		if (y<0) return;
		if (x>=SCREENX) return;
		if (y>=SCREENY) return;
		KernelApi::VideoMemory[SCREENX*y+x] = c;
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
		KernelApi::Vibro();
	}

	//writes cure to pill
	bool __SYSCALL WritePill(int cure_id, int charges)
	{
		ArmletKernel::LowLog("Cure/Torture %d was written %d charges", cure_id, charges);
		if ((cure_id < 0)||(cure_id > 14)) {
			if ((cure_id < 20)||(cure_id > 26))
			return false;
			ArmletKernel::TortureCharges[cure_id-20] = charges;
			KernelApi::UpdateCurrentTorture();
		}
		ArmletKernel::CureCharges[cure_id] = charges;
		KernelApi::UpdateCurrentCure();
		return true;
	}

	//returns lustra id (UNKNOWN_ID=none)
	unsigned char __SYSCALL GetLustraId()
	{
		if (ArmletKernel::bLustraInRange)
			return ArmletKernel::LastLustraId;
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
		if (len < sizeof(SRV_FROM_ARMLET_HEADER)) {
			ArmletKernel::LowLog("PROTOCOL: len < SRV_FROM_ARMLET_HEADER");
			return;
		}
		SRV_FROM_ARMLET_HEADER* header = (SRV_FROM_ARMLET_HEADER*)packet;
		if (header->msg_type != MSG_TASK_COMPLETED) {
			ArmletKernel::LowLog("PROTOCOL: msg_type != MSG_TASK_COMPLETED");
			return;
		}
		SRV_TASK_COMPLETED* pkt = (SRV_TASK_COMPLETED*)packet;
		if (len < sizeof(SRV_TASK_COMPLETED)) {
			ArmletKernel::LowLog("PROTOCOL: len < SRV_FROM_ARMLET_HEADER");
			return;
		}
		ArmletKernel::LowLog("PROTOCOL: receive MSG_TASK_COMPLETED with arm_msg_num %d for srv_msg_num %d",
			header->arm_msg_num, pkt->srv_msg_num, pkt->bResult);
		return; 
	}

	void __SYSCALL SendAppState(unsigned char packet[APP_STATE_LEN]) {
		KernelApi::UpdateAppState(packet[0],packet[1],packet[2],packet[3]);
		return;
	}

	void __SYSCALL GetRadioStatus(int* gate_id, int* signal_level) //-20..-120
	{
		*gate_id = 3;
		
		unsigned char p = (unsigned char)(100 - (GetUpTime() / 1000)); //1% per sec
		if (p>100) p = 100;
		*signal_level = -120+p;
		return;
	}

	unsigned short __SYSCALL GetArmletId()
	{
		return 50;
	}

	//SPECIAL PLATFORM DEPENDENT
	void __SYSCALL SetPillName(int pill_id, char* name)
	{
		KernelApi::SetPillName(pill_id, name);
	}

} //namespace