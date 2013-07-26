#pragma once

class MainForm : public OpenSpaceFormBase
{
	virtual fresult DoLayout();
	fresult CreateInfoBar(Panel** o_mainStatus);
	fresult CreateMenu(IMenu** o_mnu);


public:
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	virtual fresult OnLoad();
};