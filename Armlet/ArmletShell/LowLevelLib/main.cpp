#include "Internal.h"
#include "AppMain.h"

BUTTON_HANDLER* gClickButtonHandler = NULL;
BUTTON_HANDLER* gHoldButtonHandler = NULL;

void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler)
{
	gClickButtonHandler = ClickButtonHandler;
	gHoldButtonHandler = HoldButtonHandler;
}

void OnButtonClick(int button)
{
	if (button == BUTTON_R) {
		Clear(0x0009);
		return;
	}
	if (button == BUTTON_E) {
		Clear(0x0333);
		char* str = "HELLO , world!";
		DrawTextString(30,42,str,strlen(str),0,0);
		return;
	}
	if (gClickButtonHandler)
		(*gClickButtonHandler)(button);
}

void OnButtonHold(int button, int seconds)
{
	if (button==BUTTON_A) {
		Clear(BLUE);
		Show_Glyphs(2,0,16,16,2,2);
		return;
	}
	if (gClickButtonHandler)
		(*gClickButtonHandler)(button);
}

void __stdcall my_thread(void* param)
{
	char* str = "THREAD was started";
	DrawTextString(60,15,str,strlen(str),0,0);
	DrawTextString(15,75,str,strlen(str),0,0);
}

void LowLevelLibMain(void)
{
	Clear(GREEN);
	StartThread(my_thread,NULL);
	StartThread(AppMainThread,NULL);
	return;
}