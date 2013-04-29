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
	
	bool InitializeShell() {
		Clear(RED);
		//StartThread(my_thread,NULL);
		StartThread(AppMainThread,NULL);
		return true; //TODO
	}

	void CriticalError(char* error)
	{
		//TODO
	}

	//callout for  button
	void OnButtonPress(int button_id)
	{
		; //TODO
	}

	//callout for button
	void OnButtonRelease(int button_id)
	{
		if (gClickButtonHandler)
			(*gClickButtonHandler)((ubyte_t)button_id);
	}

	//callout cure/pill
	void OnPillConnect(int cure_id, int charges)
	{
		//Log("Cure %d was connected, charges %d",cure_id, charges);
		Log("Cure was connected, with charges");
		WritePill(cure_id, charges-1);
		DoVibro(500);
		ArmletApi::SetCureName(1,"Спазмолитик");
		ArmletApi::SetCureName(9,"Абсорбент");
		ArmletApi::SetCureName(12,"Нанохирург");
		if (charges == 0) {
			; //TODO
		} else {
			; //TODO
		}
	}

	//callout for radio transport
	void OnRadioPacket(unsigned char* packet, int len)
	{
		; //TODO
	}
} //namespace

namespace LowLevel {

#ifdef _MBCS
	//internal emulation (time)
	void NextMedTick()
	{
		//TODO
	}
#endif

} //namespace