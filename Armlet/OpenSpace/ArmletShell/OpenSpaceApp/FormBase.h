#pragma once

class FormBase : public IForm
{
	ApplicationBase* _AppBase;
	ScatteredMenu _mnuInstance;
protected:

	Repositories* _Repositories;
	Factories* _Factories;
	FormManager* _FormManager;

	Panel* _FormPanel;
	char* _Name;
	IMenu* _Menu;

	bool _IsDialog;
 
	fresult BaseInit(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, ApplicationBase* app);

public:
	virtual char* GetName();
	virtual fresult DoLayout(){return SUCCESS;};	
	virtual fresult OnButtonEvent(ButtonState key, bool* found);
	virtual fresult OnAfterDialogShown(IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw);
	virtual fresult OnBeforeShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult);
	virtual fresult OnAfterShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult);
	virtual fresult OnLoad();
	bool_t IsDialog();

	virtual fresult Draw();
};


