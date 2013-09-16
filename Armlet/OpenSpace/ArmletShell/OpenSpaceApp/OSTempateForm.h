#pragma once

class OSTemplateForm : public OpenSpaceFormBase
{
	fresult CreateMenu(IMenu** o_mnu);

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);
};