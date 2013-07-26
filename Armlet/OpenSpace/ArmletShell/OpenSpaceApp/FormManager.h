#pragma once
#include "IForm.h"

struct FormDescription;

class FormManager
{
	FormDescription* _formsRepository;
	ubyte_t _formsCount;
	ubyte_t _registeredForms;

	IForm** _formStack;
	ubyte_t _formStackLength;
	sbyte_t _shownIndex;
	
	FormDescription* allocFormsDescription(ubyte_t count);
	IForm** allocFormsArray(ubyte_t count);

protected:
	

public:
	IForm* GetForm(char* formName);
	IForm* GetCurrentForm();

	fresult Init(ubyte_t formsCount, ubyte_t stackLength);
	fresult RegisterForm(IForm* frm);
	
	fresult ShowForm(char* name);
	fresult CloseForm(IForm* frm);

	fresult GetOpenFormHandler(char* name, IMenuHandler** handler);

};

class FormOpenDelegate : IMenuHandler
{
	FormManager* _formManager;
	char* _formName;
public: 
	fresult Init (FormManager* frmmngr, char* formName);

	virtual fresult OnClick( IMenuItem* sender );

};

struct FormDescription
{
	IForm* FormObject;
	char* FormName;
	FormOpenDelegate OpenFormHandler;
};

