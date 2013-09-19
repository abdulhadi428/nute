#pragma once

class MedStatusForm : public OpenSpaceFormBase
{
	fresult CreateMenu(IMenu** o_mnu);
	TextField* _txtStatus;

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	virtual fresult OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult );

	virtual fresult OnMenuX( IMenuItem* mi );

	virtual fresult OnMenuZ( IMenuItem* mi );

};