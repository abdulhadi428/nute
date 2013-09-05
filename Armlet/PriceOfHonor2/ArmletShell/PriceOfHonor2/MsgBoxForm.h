#pragma once
#include "Honor2FormBase.h"

#define MAX_MSG_LEN 141

struct MessageBoxContent
{
	char* title;
	char text[MAX_MSG_LEN];
	ImageHandle icon;
	bool_t YesNoDialog;
	bool_t Empty;
};

#define _MSGBOX_MAX_STACK 5

class MsgBoxForm : public Honor2FormBase
{
	
	MessageBoxContent _messages[_MSGBOX_MAX_STACK]; 
	sbyte_t _currentMessageIndex;

	fresult CreateMenu(IMenu** o_mnu);
	TextField* _txtMsgBoxText;

	IMenuItem* _miYes;
	IMenuItem* _miOk;
	IMenuItem* _miCancel;


	fresult SetMsgBoxData();

	fresult CloseMessageBox(FormShowResults result);
	fresult CloseMessageBoxWithOk(IMenuItem* sender);
	fresult CloseMessageBoxWithCancel(IMenuItem* sender);

	
public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic);

	fresult ShowMessage( char* title, ImageHandle iconHandle, char* text );
	fresult ShowMessage(bool_t yesNoDialog, char* title, ImageHandle iconHandle, char* text);
	fresult ShowMessageYesNo( char* title, ImageHandle iconHandle, char* text );
	MessageBoxContent* GetCurrentMessage();
	MessageBoxContent* GetLastYesNoDialog();

};