#pragma once

namespace Callouts {

	void ArmletMain(void);											//ARMLET InitializeShell

	//ARMLET EMULTION
	//input
	void OnButtonPress(int button_id);								//ARMLET emulation same 
	void OnButtonRelease(int button_id);							//ARMLET emulation same
																	//(OnButtonClick/OnButtonHold is only inside MiddleWareLib)
	void OnPillConnect(int pill_id, int charges);					//ARMLET emulation	OnPillConnect

	//SERVER EMULATION
	// OnRadioPacket / SendRadioPacket
	void SetPlayerName(char const*const name);						//SERVER emulation
	void SetRegenerationLevel(int level);							//SERVER emulation
	void SetLockAccess(int lock_id, bool bAllow);					//SERVER emulation
	void SendMessage(char const*const msg);							//SERVER emulation
	void Explosion(int room_id, int probability, int explosion_type);	//SERVER emulation
	//GATE EMULATION
	void NewTime(int hours, int minutes);
				
	void NextMedTick();												//for medicine emulation

} //namespace
