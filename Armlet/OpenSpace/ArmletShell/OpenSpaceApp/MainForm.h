#pragma once

class MainForm : public OpenSpaceFormBase
{
	fresult CreateInfoBar(Panel** o_mainStatus);
	fresult CreateMenu(IMenu** o_mnu);


public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);
};