/*#pragma once
#include "UIDesigner.h"
#include "ArmletShell.h"
#include "Med.h"

#define MAX_LUSTRA_WAIT_TIME 120000
#define LUSTRA_POLL_TIME 3*1000
#define DOVIBRO_TIME 500
#define NOLUSTRA_LOG_INTERVAL 30*1000

#define BATTERYSTATUS_POLL_TIME 4*1000


class UserInterface : public UIDesigner
{

	uword_t _armletId;
	sword_t _lastKnownLustraId;
	sword_t _lastKnownRoomId;
	sword_t _lastKnownDiscoveryTime;
	sword_t _lastLoggedTime;

	sword_t GetRoomIdFromLustraId(sword_t lustraId);
	
	fresult AddNewWound(TARGET bodyPart);
	fresult RedrawIfForeground(Form_DEPRECATED* form);

	void LogError(char* message);
	
	bool_t _woundSelected;
public: 
	fresult Init();

	void OnPillConnected(sword_t cure_id, sword_t charges);
	void SetPlayerName(char* name);
	void SetRoom(sword_t room);
	fresult SetBatteryLevel(ubyte_t batteryLevel);
	fresult SetRadioStatus(sword_t gateId, sword_t level);
	void OnExplosion(sword_t room);
	void OnServerMessage(char* msg);
	void SetRegenerationRate(sword_t regenRate);
	bool OnLustraTimer();
	void DoMedTick();
	bool OnBatteryTimer();
	void Int2Str(uword_t value, char* buff, ubyte_t buffSize);


	#pragma	region Hanldlers
	fresult virtual OnMainMnuShot(IMenuItem* sender);
	fresult virtual OnMainMnuKnockout(IMenuItem* sender);

	fresult virtual OnButtonPressed(ButtonState button);

	fresult virtual OnWoundMnuHead( IMenuItem* sender );
	fresult virtual OnWoundMnuStomach( IMenuItem* sender );
	fresult virtual OnWoundMnuLeftArm( IMenuItem* sender );
	fresult virtual OnWoundMnuRightArm( IMenuItem* sender );
	fresult virtual OnWoundMnuChest( IMenuItem* sender );
	fresult virtual OnWoundMnuBack( IMenuItem* sender );
	fresult virtual OnWoundMnuLeftLeg( IMenuItem* sender );
	fresult virtual OnWoundMnuRightLeg( IMenuItem* sender );
#pragma	endregion

};*/