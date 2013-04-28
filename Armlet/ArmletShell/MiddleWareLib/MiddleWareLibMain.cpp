#include "ArmletApi.h"

namespace ArmletApi {
	
	bool InitializeShell() {
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
		; //TODO
	}

	//callout cure/pill
	void OnPillConnect(int cure_id, int charges)
	{
		//LowLevel::Log("Cure %d was connected, charges %d",cure_id, charges);
		WritePill(cure_id, charges-1);
		//LowLevel::Vibro();
		//LowLevel::SetCureName(1,"Спазмолитик");
		//LowLevel::SetCureName(9,"Абсорбент");
		//LowLevel::SetCureName(12,"Нанохирург");
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

#ifdef _MBCS
	//internal emulation (time)
	void NextMedTick()
	{
		//TODO
	}
#endif

} //namespace