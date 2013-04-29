#include "DownInterface.h"
#include "ArmletApi.h"

namespace LowLevel {

	void SetPlayerName(char const*const name)
	{
		//TODO
	}

	void SetRegenerationLevel(int level)
	{
		//TODO
	}

	void SetLockAccess(int lock_id, bool bAllow)
	{
		//TODO
	}

	void SendMessage(char const*const msg)
	{
		//TODO
	}

	void Explosion(int room_id)
	{
		//TODO
	}

} //namespace

namespace ArmletApi {

	void __SYSCALL SendRadioPacket(unsigned char* packet, int len) {
		; //TODO implement
	}

} //namespace

/*
//SERVER EMULATION
	// OnRadioPacket / SendRadioPacket
	void SetPlayerName(char const*const name);						//SERVER emulation
	void SetRegenerationLevel(int level);							//SERVER emulation
	void SetLockAccess(int lock_id, bool bAllow);					//SERVER emulation
	void SendMessage(char const*const msg);							//SERVER emulation
	void Explosion(int room_id);									//SERVER emulation
*/
