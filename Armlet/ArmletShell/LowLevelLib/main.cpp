#include "Internal.h"
#include "AppMain.h"
#include "ArmletApi.h"

BUTTON_HANDLER* gClickButtonHandler = NULL;
BUTTON_HANDLER* gHoldButtonHandler = NULL;

void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler)
{
	gClickButtonHandler = ClickButtonHandler;
	gHoldButtonHandler = HoldButtonHandler;
}

void OnButtonClick(int button)
{
	/*if (button == BUTTON_R) {
		Clear(0x0009);
		return;
	}
	if (button == BUTTON_E) {
		Clear(0x0333);
		char* str = "HELLO , world!";
		DrawTextString(30,42,str,strlen(str),0,0);
		return;
	}*/
	if (gClickButtonHandler)
		(*gClickButtonHandler)(button);
}

void OnButtonHold(int button, int seconds)
{
	/*if (button==BUTTON_A) {
		Clear(BLUE);
		Show_Glyphs(2,0,16,16,2,2);
		return;
	}
	if (button==BUTTON_B) {
		Clear(BLUE);
		Show_Glyphs(2,0,16,16,2,1);
		return;
	}
	if (button==BUTTON_C) {
		Clear(BLUE);
		Show_Glyphs(1,0,8,8,1,1);
		return;
	}*/
	if (gHoldButtonHandler)
		(*gHoldButtonHandler)(button);
}

void __stdcall my_thread(void* param)
{
	char* str = "THREAD was started";
	//DrawTextString(60,15,str,strlen(str),0,0);
	//DrawTextString(15,75,str,strlen(str),0,0);
}

#define MAX_TIMERS 30
typedef struct _TIMER {
	bool bUsed;
	int curr;
	int period;
	TIMER_PROC* routine;
} TIMER;
TIMER gTimers[MAX_TIMERS];

void LowLevelLibMain(void)
{
	memset(gTimers,0,sizeof(gTimers));
	Clear(GREEN);
	StartThread(my_thread,NULL);
	StartThread(AppMainThread,NULL);
	return;
}

bool RequestTimer(int period, TIMER_PROC routine)
{
	if (period < 100)
		return false;
	for (int i=0;i<MAX_TIMERS;i++) {
		if (gTimers[i].bUsed) {
			continue;
		}
		//todo interlocked
		gTimers[i].bUsed = true;
		gTimers[i].curr = period;
		gTimers[i].period = period;
		gTimers[i].routine = routine;
		return true;
	}
	return false;
}

void OnElapsed50msec()
{
	for (int i=0;i<MAX_TIMERS;i++) {
		if (!gTimers[i].bUsed) {
			continue;
		}
		gTimers[i].curr -= 50;
		if (gTimers[i].curr <= 20) {
			if (gTimers[i].routine()) {
				gTimers[i].curr += gTimers[i].period;
			} else {
				gTimers[i].bUsed = false;
			}
		}
	}
}