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
	gHoldButtonHandler = HoldButtonHandler;
}

namespace ArmletApi {

	bool __CALLBACK TimerProc(int elapsed)
	{
		return false;
	}

	bool __CALLOUT InitializeShell() {
		int i = 0;
		Clear(RED);
		GetBatteryLevel();
		SetScreenBrightness(100);
		char buff[200];
		int res = snprintf(buff,sizeof(buff),"test %d",0);
		DrawPixel(1,1,0xFFF);
		Color area[100];
		for (i=0; i<100; i++) area[i] = 0xF0F;
		DrawArea(5,5,area,100,10,10);
		DoVibroAndBeep(500);
		WritePill(10,15);
		ubyte_t l = GetLustraId();
		SendRadioPacket(NULL,0);
		uword_t a = GetArmletId();
		FILE log;
		OpenFile(&log, "log.txt", true);
		char* s = "meta log test";
		WriteFile(&log, s, Length(s));
		OpenFile(&log, "zlog.txt", true);
		WriteFile(&log, (char*)area, 100);
		for (i=0; i<100; i++) area[i] = 0x0;
		ReadFile(&log, (char*)area, 100);
		AppendFile(&log, (char*)area, 100);
		DrawArea(5,5,area,100,10,10);
		RequestTimer(TimerProc,1000);
		int up = GetUpTime();
		int rnd = GetRandom(50);
		SetCureName(4,"Relo");
		AppMainThread(NULL);
		//StartThread(my_thread,NULL);
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
		Log("Cure was connected, with charges");
		WritePill(cure_id, charges-1);
		DoVibroAndBeep(500);
//		ArmletApi::SetCureName(1,"Спазмолитик");
//		ArmletApi::SetCureName(9,"Абсорбент");
//		ArmletApi::SetCureName(12,"Нанохирург");
		if (charges == 0) {
			; //TODO
		} else {
			; //TODO
		}
	}

	//__CALLOUT for radio transport
	void __CALLOUT OnRadioPacket(unsigned char* packet, int len)
	{
		; //TODO
	}

#ifdef _MSC_VER
	//internal emulation
	void __CALLOUT NextMedTick()
	{
		//TODO
	}
#endif

} //namespace
