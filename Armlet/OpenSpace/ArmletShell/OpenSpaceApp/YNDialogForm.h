#pragma once


class YNDialogForm : public OpenSpaceFormBase, public IDialogForm
{
	fresult CreateMenu(IMenu** o_mnu);

	TextField* _txtHeader;
	TextField* _txtContent;

	MenuItem* _miYes;
	MenuItem* _miCancel;

	char* _dialogName;

public:
	virtual fresult DoLayout();
	fresult Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic);

	fresult ShowYNDialog(char* dialogName, char* formTitle, char* formSubtitle, char* msgTitle, char* msgContent, char* yesText);

	virtual bool_t IsDialog(){return TRUE;};

	virtual char* GetDialogName();
	virtual fresult ShowYNDialog( char* name, char* title, char* text, ImageHandle icon );
	virtual fresult ShowMessage( char* name, char* title, char* text, ImageHandle icon );

};

