#include "ArmletApi.h"
#include "ArmletShell.h"
#include "MiddleInterface.h"
#include "AppMain.h"
#include "Osanve.h"
//#include <stdio.h>

//TODO TO BUTTONS and to app
BUTTON_HANDLER* gClickButtonHandler = NULL;
BUTTON_HANDLER* gHoldButtonHandler = NULL;

// Заполняет глобальные указатели на обработчики кнопок переданными значениями
void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler)
{
	gClickButtonHandler = ClickButtonHandler;
	gHoldButtonHandler = HoldButtonHandler;
}

namespace ArmletApi {
	int tick = 0;
	// обработчик таймера начальной задержки
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

	// обработчик включения браслета
	bool __CALLOUT InitializeShell() {
		// Заставка
		Clear(LIGHT|BLUE);
		FILE log;
		char tmp[16];
		const char* str = "FIRST AGE: NORTH COURAGE";
		char* s = "test log" ;
		uword_t a = GetArmletId();
		DrawTextString(6,60,str,Length(str),WHITE,BLACK);
		// журнал	
		OpenFile(&log, "log.txt", true);
		//sprintf( tmp, "Log, armlet#%d", a);
		WriteFile(&log, s, Length(s));

		GetBatteryLevel();
		SetScreenBrightness(100);
//		char buff[200]; Some debug...
//		int res = snprintf(buff,sizeof(buff),"test %d",0);
		DrawPixel(1,1,WHITE);
		DoVibroAndBeep(500);

		RequestTimer(TimerProc,100); // пауза и запуск
		int up = GetUpTime();
		int rnd = GetRandom(50);
		//SetCureName is checked in AppMainThread
		//WritePill is checked in AppMainThread
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
	{ // В Первой эпохе нам ничего не нужно
		//Log("Cure %d was connected, charges %d",cure_id, charges);
		// DoVibroAndBeep(500);
		//UI CALL:
		// _OnPillConnected(cure_id, charges);
	}

	//__CALLOUT for radio transport Прием пакета из радиоканала
	void __CALLOUT OnRadioPacket(unsigned char* packet, int len)
	{
		if (len < sizeof(PACKET_LEN)) {
			CriticalError("PROTOCOL: len < PACKET_LEN");
			return;
		}
		if( *(packet+1) & PACKET_TYPE_ATACK)
		{ // пакет атаки
		  ATACK_PACKET* ap = (ATACK_PACKET*)packet;
		  if( ap->dst != Player.userId) // не нас, пронесло...
			  return;
		  if(*(packet+1) & FORCE_HEAL) // упс, прилетело
			  _OnHealPacket(ap->src, ap->atack);
		  else
			  _OnAtackPacket(ap->src, ap->atack, GET_CONS(ap->flags));
		} else { // пакет осанве
			OSANVE_PACKET* op = (OSANVE_PACKET*)packet;
			_OnOsanvePacket( op->src, op->force, op->maxForce, op->flags&OSANVE_MASK);
		}
	}

#ifdef _MSC_VER
	//internal emulation
	void __CALLOUT NextMedTick()
	{
		OnForceTick();
	}
#endif

} //namespace
