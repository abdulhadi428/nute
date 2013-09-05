#include "ArmletApi.h"
#include "ArmletShell.h"
#include "MiddleInterface.h"
#include "AppMain.h"

//TODO TO BUTTONS and to app
BUTTON_HANDLER* gClickButtonHandler = NULL;
BUTTON_HANDLER* gHoldButtonHandler = NULL;
void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler)
{
	gClickButtonHandler = ClickButtonHandler;
	//@KL gHoldButtonHandler = HoldButtonHandler;
}

namespace ArmletApi {

	int tick = 0;
	bool __CALLBACK TimerProc(int elapsed)
	{
		if (tick<15) {
			DrawPixel(40+2*tick,70,RED);
			tick++;
			return true;
		}

		AppMainThread(NULL);
		return false;
	}

	bool __CALLOUT InitializeShell() {
		Clear(BLUE);
		const char* str = "PRICE of HONOR!";
		DrawTextString(40,60,str,Length(str),RED,BLUE);

		GetBatteryLevel();
		SetScreenBrightness(100);
		char buff[200];
		int res = snprintf(buff,sizeof(buff),"test %d",0);
		DrawPixel(1,1,WHITE);
		DoVibroAndBeep(500);
		ubyte_t l = GetLustraId();
		SendRadioPacket(NULL,0);
		uword_t a = GetArmletId();
		FILE log;
		OpenFile(&log, "log.txt", true);
		char* s = "log test";
		WriteFile(&log, s, Length(s));

		/*
		//int i = 0;
		//Color area[100];
		//for (i=0; i<100; i++) area[i] = 0xF0F;
		//DrawArea(5,5,area,100,10,10);
		//WriteFile(&log, (char*)area, 100);
		//for (i=0; i<100; i++) area[i] = 0x0;
		//ReadFile(&log, (char*)area, 100);
		//AppendFile(&log, (char*)area, 100);
		//DrawArea(5,5,area,100,10,10);
		*/

		RequestTimer(TimerProc,100);
		int up = GetUpTime();
		int rnd = GetRandom(50);
		//SetCureName is checked in AppMainThread
		//WritePill is checked in AppMainThread
		//StartThread(AppMainThread,NULL);

		return true; //TODO
	}

	void __CALLOUT CriticalError(char* error)
	{
		//TODO
	}

	void __NOCALL  Log(char* fmt, ...)
	{
		//TODO
	}

	//__CALLOUT for  button
	void __CALLOUT OnButtonPress(int button_id)
	{
		; //TODO
	}

	//__CALLOUT for button
	void __CALLOUT OnButtonRelease(int button_id)
	{
		if (gClickButtonHandler)
			(*gClickButtonHandler)((ubyte_t)button_id);
	}

	//__CALLOUT cure/pill
	void __CALLOUT OnPillConnect(int cure_id, int charges)
	{
		//Log("Cure %d was connected, charges %d",cure_id, charges);
		DoVibroAndBeep(500);
		//UI CALL:
		_OnPillConnected(cure_id, charges);
	}

	//__CALLOUT for radio transport
	void __CALLOUT OnRadioPacket(unsigned char* packet, int len)
	{
		if (len < sizeof(SRV_TO_ARMLET_HEADER)) {
			CriticalError("PROTOCOL: len < SRV_TO_ARMLET_HEADER");
			return;
		}
		SRV_TO_ARMLET_HEADER* header = (SRV_TO_ARMLET_HEADER*)packet;
		switch(header->msg_type) {
			case MSG_SHOW_MESSAGE:
				{
					SRV_STRING* pkt = (SRV_STRING*)packet;
					int size = len - sizeof(SRV_STRING);
					if (pkt->string[size]!=0)
						pkt->string[size]=0;
					_OnServerMessage(&pkt->string[0]);
				}
				break;
			case MSG_SET_PLAYER_NAME:
				{
					SRV_STRING* pkt = (SRV_STRING*)packet;
					int size = len - sizeof(SRV_STRING);
					if (pkt->string[size]!=0)
						pkt->string[size]=0;
					_OnSetPlayerName(&pkt->string[0]);
				}
				break;
			case MSG_SET_PLAYER_REGENERATION:
				{
					SRV_SET_PLAYER_REGENERATION* pkt = (SRV_SET_PLAYER_REGENERATION*)packet;
					_OnSetRegenerationRate(pkt->level);
				}
				break;
			case MSG_ROOM_HIT:
				{
					SRV_ROOM_HIT* pkt = (SRV_ROOM_HIT*)packet;
					_OnExplosion(pkt->room_id,pkt->probability,pkt->explosion_type);
				}
				break;
			case MSG_TIME:
				{
					SRV_TIME* pkt = (SRV_TIME*)packet;
					_OnNewTime(pkt->hours,pkt->minutes); //new minute
				}
				break;
			default:
				//TODO Log
				break;
		}
		//TODO cycle with
		//	MSG_TASK_COMPLETED = 64,
		//	MSG_TASK_COMPLETION_CONFIRMED = 65,
	}

#ifdef _MSC_VER
	//internal emulation
	void __CALLOUT NextMedTick()
	{
		_MedicineTimerTickCallback(MED_TICK_TIME);
	}
#endif

} //namespace
