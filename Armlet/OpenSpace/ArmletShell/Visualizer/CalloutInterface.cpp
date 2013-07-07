#include "stdafx.h"
#include "ArmletApi.h"
#include "..\KernelDll\KernelApi.h"
#include "ServerProtocol.h"

#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"	//_vsnprintf
#include "stdarg.h"	//va_...
#include "string.h"	//strlen
#include "malloc.h" //malloc

namespace Callouts {
	short NoncheId = 1000;

	void ArmletMain(void)
	{		
		if (!ArmletApi::InitializeShell()) {
			ArmletApi::CriticalError("InitializeShell failed");
		}
	}

	//ARMLET emulation same
	void OnButtonPress(int button_id)
	{
		ArmletApi::OnButtonPress(button_id);
	}

	//ARMLET emulation same
	void OnButtonRelease(int button_id)
	{
		ArmletApi::OnButtonRelease(button_id);
	}

	//ARMLET emulation	OnPillConnect
	void OnPillConnect(int cure_id, int charges)
	{
		ArmletApi::OnPillConnect(cure_id, charges);
	}

	//SERVER EMULATION
	void SetPlayerName(char const*const name)
	{
		int size = strlen(name);
		int full_size = sizeof(SRV_STRING) + size;	//-1 for already [1]
													//+1 for \0

		SRV_STRING* packet = (SRV_STRING*)malloc(full_size);
		packet->header.msg_type = MSG_SET_PLAYER_NAME;
		packet->header.srv_msg_num = NoncheId++;
		memcpy(packet->string,name, size+1);
		//TODO timer/list helper
		ArmletApi::OnRadioPacket((unsigned char*)packet,full_size);
		memset(packet,0,full_size);
		free(packet);
	}

	//SERVER EMULATION
	void SetRegenerationLevel(int level)
	{
		int full_size = sizeof(SRV_SET_PLAYER_REGENERATION);

		SRV_SET_PLAYER_REGENERATION* packet = (SRV_SET_PLAYER_REGENERATION*)malloc(full_size);
		packet->header.msg_type = MSG_SET_PLAYER_REGENERATION;
		packet->header.srv_msg_num = NoncheId++;
		packet->level = (unsigned char)level;
		//TODO timer/list helper
		ArmletApi::OnRadioPacket((unsigned char*)packet,full_size);
		memset(packet,0,full_size);
		free(packet);
	}

	//SERVER EMULATION
	void SetLockAccess(int lock_id, bool bAllow)
	{
		UNREFERENCED_PARAMETER(lock_id);
		UNREFERENCED_PARAMETER(bAllow);
		//FUTURE
	}

	//SERVER EMULATION
	void SendMessage(char const*const msg)
	{
		int size = strlen(msg);
		int full_size = sizeof(SRV_STRING) + size;	//-1 for already [1]
													//+1 for \0

		SRV_STRING* packet = (SRV_STRING*)malloc(full_size);
		packet->header.msg_type = MSG_SHOW_MESSAGE;
		packet->header.srv_msg_num = NoncheId++;
		memcpy(packet->string,msg, size+1);
		//TODO timer/list helper
		ArmletApi::OnRadioPacket((unsigned char*)packet,full_size);
		memset(packet,0,full_size);
		free(packet);
	}

	//SERVER EMULATION
	void Explosion(int room_id)
	{
		int full_size = sizeof(SRV_ROOM_HIT);

		SRV_ROOM_HIT* packet = (SRV_ROOM_HIT*)malloc(full_size);
		packet->header.msg_type = MSG_ROOM_HIT;
		packet->header.srv_msg_num = NoncheId++;
		packet->room_id = (unsigned char)room_id;
		//TODO timer/list helper
		ArmletApi::OnRadioPacket((unsigned char*)packet,full_size);
		memset(packet,0,full_size);
		free(packet);
	}

	//SPECIAL PLATFORM DEPENDENT
	void NextMedTick()
	{
		ArmletApi::NextMedTick();
	}

} //namespace
