#pragma once
#include "IForm.h"

class FormManager;  

class FormOpenDelegate : IMenuHandler
{
	FormManager* _formManager;
	char* _formName;
public: 
	fresult Init (FormManager* frmmngr, char* formName);

	virtual fresult OnClick( IMenuItem* sender );

};

class FormCloseDelegate : IMenuHandler
{
	FormManager* _formManager;
	char* _formName;
	FormShowResults _result;
public: 
	fresult Init (FormManager* frmmngr, FormShowResults result);

	virtual fresult OnClick( IMenuItem* sender );

};

struct FormDescription
{
	IForm* FormObject;
	char* FormName;
	FormOpenDelegate OpenFormHandler;
};

class FormManager
{
	FormDescription* _formsRepository;

	FormCloseDelegate _formCloseDelegateInstance[FormShowResultsCount];

	ubyte_t _formsCount;
	ubyte_t _registeredForms;

	IForm** _formStack;
	ubyte_t _formStackLength;
	sbyte_t _shownIndex;
	
	FormDescription* allocFormsDescription(ubyte_t count);
	IForm** allocFormsArray(ubyte_t count);

	fresult ActivateForm(IForm* frm, IForm* prevForm, bool_t isReactivation, FormShowResults prevFormShowResult);

protected:
	
public:
	FormDescription* GetFormDescription( char* formName );
	IForm* GetForm(char* formName);

	IForm* GetCurrentForm();

	fresult Init(ubyte_t formsCount, ubyte_t stackLength);
	fresult RegisterForm(IForm* frm);
	
	fresult ShowForm(char* name);
	fresult CloseForm(IForm* frm, FormShowResults formShowResult);

	fresult GetOpenFormHandler(char* name, IMenuHandler** o_handler);
	fresult GetCloseFormHandler(IMenuHandler** o_handler);
	fresult GetCloseFormHandler(IMenuHandler** o_handler, FormShowResults result);
	fresult LayoutForms();
};


