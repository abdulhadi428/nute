#pragma once

class FormBase : public IForm
{
	ApplicationBase* _AppBase;
	ScatteredMenu _mnuInstance;

	fresult DefaultMenuHandler(IMenuItem* mi);
	MenuHandlerDelegate<FormBase> _autoDispatchMenuHandlerInstance;
protected:

	Repositories* _Repositories;
	Factories* _Factories;
	FormManager* _FormManager;

	Panel* _FormPanel;
	char* _Name;
	IMenu* _Menu;

	bool _IsDialog;

	MenuHandlerDelegate<FormBase>* _AutoDispatchMenuHandler;

	fresult BaseInit(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, ApplicationBase* app);

	//default menu handling
	virtual fresult OnMenu(IMenuItem* mi, bool_t* handled) {return SUCCESS;};
	virtual fresult OnMenuA(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuB(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuC(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuX(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuY(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuZ(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuL(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuE(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuR(IMenuItem* mi) {return SUCCESS;};
	virtual fresult OnMenuOther(IMenuItem* mi) {return SUCCESS;};


	fresult HCenterTextField(TextField* tf, HorizontalAlignmentStyles alignment, uword_t chars, ubyte_t areaWidth, ubyte_t offsetLeft);
	fresult HCenterConrol(IControl* ctrl, HorizontalAlignmentStyles alignment, ubyte_t requiredWidth, ubyte_t areaWidth, ubyte_t offsetLeft);

public:
	virtual char* GetName();
	virtual fresult DoLayout(){return SUCCESS;};	
	virtual fresult OnButtonEvent(ButtonState key, bool* found);
	virtual fresult OnAfterDialogShown(IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw);
	virtual fresult OnBeforeShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult);
	virtual fresult OnAfterShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult);
	virtual fresult OnLoad();
	bool_t IsDialog();

	fresult CloseForm(FormShowResults formShowResult);

	virtual fresult Draw();
};


