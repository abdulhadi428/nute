#pragma once

class MedChooseTorsoWoundForm : public OpenSpaceFormBase
{
	fresult CreateMenu(IMenu** o_mnu);
	MedWoundTarget _currentTarget;

	TextField* _txtTorsoTraumaCaption;
	TextField* _txtChoosePlaceCaption;
protected:
	fresult OnMenu( IMenuItem* mi, bool_t* handled );

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	virtual fresult OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw );
	virtual fresult OnBeforeShow( IForm* prevFrom, bool_t reActivation, FormShowResults results );

};