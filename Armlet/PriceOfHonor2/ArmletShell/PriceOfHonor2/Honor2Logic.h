#pragma once
#include "med.h"

typedef enum _LOG_KINDS {
	LogKindMessages,
	LogKindSystem,
	LogKindEvents,
	LogKindMedSymptoms,
	LogsCount
} LogKinds;


class Honor2Logic
{
	Honor2App* _App;

protected:
	fresult ShowMessage(char* title, ImageHandle iconHandle, char* text);
	fresult AppendLog(LogKinds log, char* message);
	fresult AppendMainStatus(char* mainStatus);
	fresult ReportError(char* errorText);

public:

	fresult Init(Honor2App* app);

	//UI logic entries
	char* OnNewWound(TARGET trg);
	fresult OnKnockout();


	//System callouts
	fresult OnPillConnected(CURE_ID pillId);
	fresult OnExposion();
	fresult OnRoomPresenceDetected(sbyte_t roomId);
	fresult OnMedTick();


	//network
	fresult SetRegenRate(ubyte_t regenRate);
	fresult SetPlayerName(char* playerName);
	fresult OnNewMessage(char* messageText);

};