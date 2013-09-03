#pragma once
#include "Honor2FormBase.h"


class Honor2MainForm : public Honor2FormBase
{
	TextField* _txtStatus;

	fresult CreateMenu(IMenu** o_mnu);
	fresult OnKnockOut(IMenuItem* Sender);

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic);

	virtual ubyte_t FormControlsCount();

	virtual fresult OnBeforeShow( IForm* prevFrom, bool_t reActivation );

	fresult SetStatus(char* statusText);

	fresult Scroll(IMenuItem* sender);
};