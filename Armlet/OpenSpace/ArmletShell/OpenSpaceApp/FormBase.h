#pragma once

class FormBase : public IForm
{
	ApplicationBase* _AppBase;
protected:

	Repositories* _Repositories;
	Factories* _Factories;
	FormManager* _FormManager;

	Panel* _FormPanel;
	char* _Name;
	IMenu** _Menus;
	ubyte_t _MenusCount;
 
    IMenu** AllocMenus(ubyte_t menusCount);
	fresult BaseInit(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, ApplicationBase* app);
	virtual fresult DoLayout(){return GENERAL_ERROR;};

public:
	virtual char* GetName();
	
	virtual fresult OnButtonEvent(ButtonState key);
	virtual fresult OnBeforeShow(IForm* prevFrom, bool_t reActivation);
	virtual fresult OnAfterShow(IForm* prevFrom, bool_t reActivation);
	virtual fresult OnLoad();

	virtual fresult Draw();
};


