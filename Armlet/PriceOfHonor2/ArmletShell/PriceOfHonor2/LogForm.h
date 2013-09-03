#pragma once
#include "Honor2FormBase.h"

typedef enum _LOG_KINDS {
	LogKindMessages,
	LogKindSystem,
	LogKindEvents,
	LogKindMedSymptoms,
	LogsCount
} LogKinds;

struct Log
{
	ImageHandle icon;
	char* title;
	char* subtitle;
	TextField* txtLogField; 
	LogKinds logKind;
};

class LogForm : public Honor2FormBase
{
	Log _logs[LogsCount];
	ubyte_t _currentLog;
	
	fresult CreateMenu(IMenu** o_mnu);
	TextField* _txtMsgBoxText;

	fresult ScrollUp(IMenuItem* Sender);
	fresult ScrollDown(IMenuItem* Sender);

	fresult CycleLogLeft(IMenuItem* Sender);
	fresult CycleLogRight(IMenuItem* Sender);

	virtual fresult OnBeforeShow( IForm* prevFrom, bool_t reActivation );

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic);

	fresult LogRecord(LogKinds log, char* text);
	fresult SwitchToLog(LogKinds log);

};