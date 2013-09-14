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


public:
	virtual char* GetName();
	virtual fresult DoLayout(){return SUCCESS;};	
	virtual fresult OnButtonEvent(ButtonState key);
	virtual fresult OnBeforeShow(IForm* prevFrom, bool_t reActivation, FormShowResults results);
	virtual fresult OnAfterShow(IForm* prevFrom, bool_t reActivation, FormShowResults results);
	virtual fresult OnLoad();

	virtual fresult Draw();
};


