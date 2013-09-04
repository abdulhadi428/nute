#pragma once
#include "Honor2FormBase.h"


struct MessageBoxContent
{
	char* title;
	char* text;
	ImageHandle icon;
	bool_t Empty;
};

#define _MSGBOX_MAX_STACK 5

class MsgBoxForm : public Honor2FormBase
{
	
	MessageBoxContent _messages[_MSGBOX_MAX_STACK]; 
	sbyte_t _currentMessageIndex;

	fresult CreateMenu(IMenu** o_mnu);
	TextField* _txtMsgBoxText;

	fresult SetMsgBoxData();
	fresult HideMessageBox(IMenuItem* sender);

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic);

	fresult ShowMessage( char* title, ImageHandle iconHandle, char* text );
	MessageBoxContent* GetCurrentMessage();
};