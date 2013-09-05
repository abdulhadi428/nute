#pragma once
#include "Honor2FormBase.h"


class NewWoundForm : public Honor2FormBase
{
	fresult CreateMenu(IMenu** o_mnu);
	TextField* _txtWoundText;

	fresult OnWound(IMenuItem* sender);
	bool_t _woundSelected;
public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic);
	virtual fresult OnBeforeShow( IForm* prevFrom, bool_t reActivation, FormShowResults results );

};