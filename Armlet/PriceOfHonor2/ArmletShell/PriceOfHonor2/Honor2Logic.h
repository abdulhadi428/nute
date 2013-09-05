#pragma once
#include "med.h"

#define FIRMWARE_VERSION "1.5"

typedef enum _LOG_KINDS {
	LogKindMedDiagnostics,
	LogKindEvents,
	LogKindMessages,
	LogKindSystem,
	LogKindErrors,
	LogsCount
} LogKinds;


#define MAX_LUSTRA_WAIT_TIME 120000
#define LUSTRA_POLL_TIME 3*1000
#define DOVIBRO_TIME 500
#define NOLUSTRA_LOG_INTERVAL 30*1000

#define BATTERYSTATUS_POLL_TIME 4*1000


class Honor2Logic
{
	Honor2App* _App;

	Honor2MainForm* _mainForm;
	LogForm* _logForm;

	uword_t _armletId;
	sword_t _lastKnownLustraId;
	sword_t _lastKnownRoomId;
	sword_t _lastKnownDiscoveryTime;
	sword_t _lastLoggedTime;

	sword_t GetRoomIdFromLustraId(sword_t lustraId);

protected:
	fresult ShowMessage(char* title, ImageHandle iconHandle, char* text, bool_t wakeUp);
	fresult AppendLog(LogKinds log, char* message);
	fresult ResetLog( LogKinds log, char* message );
	fresult SetMedStatus(char* mainStatus ,  char* diagnostics, char* medEvents);
	fresult ReportError(char* errorText);

	fresult SetRoom( sword_t room );
public:

	fresult Init(Honor2App* app);

	//UI logic entries
	char* OnNewWound(TARGET trg);
	fresult OnKnockout();


	//System callouts
	void OnPillConnected( sword_t cure_id, sword_t charges );

	//timers
	void OnLustraTimer();
	void OnMedTick();


	//network
	void SetRegenRate(ubyte_t regenRate);
	void SetPlayerName(char* playerName);
	void OnNewMessage(char* messageText);
	void OnExposion(sword_t roomId, ubyte_t probability, ubyte_t explosionType);

};