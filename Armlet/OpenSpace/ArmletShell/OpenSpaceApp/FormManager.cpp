#include "IDialogForm.h"
#include "IForm.h"
#include "ArmletAppSDK.h"

#include "FormManager.h"
#include "kl_allocator.h"

static Alloc_t<IForm*, 30> SIFormPtrArr;
static Alloc_t<FormDescription, 40> SFormDescriptionArr;

IForm** FormManager::allocFormsArray(ubyte_t count)
{
	return SIFormPtrArr.Allocate(count); // new IForm*[count];
}

FormDescription* FormManager::allocFormsDescription( ubyte_t count )
{
	return SFormDescriptionArr.Allocate(count); // new FormDescription[count];
}

fresult FormManager::Init( ubyte_t formsCount, ubyte_t stackLength )
{
	fresult fres;

	_formsRepository = allocFormsDescription(formsCount);
	FAILIF(_formsRepository == NULL);
	_formsCount = formsCount;
	_registeredForms = 0;

	_formStack = allocFormsArray(stackLength);
	FAILIF(_formStack == NULL);
	_formStackLength = stackLength;
	_shownIndex = -1;


	for (ubyte_t i = 0;i < FormShowResultsCount;i++)
	{
		fres = _formCloseDelegateInstance[i].Init(this, (FormShowResults)i);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult FormManager::RegisterForm( IForm* frm )
{
	fresult fres;
	FAILIF(_registeredForms == _formsCount);

	_formsRepository[_registeredForms].FormObject = frm;
	_formsRepository[_registeredForms].FormName = frm->GetName();
	fres = _formsRepository[_registeredForms].OpenFormHandler.Init(this, frm->GetName());
	ENSURESUCCESS(fres);
	_registeredForms++;

	return SUCCESS;
}

IForm* FormManager::GetForm( char* formName )
{
	IForm* foundFrm = NULL;
	FormDescription* frmDescr = NULL;

	frmDescr = GetFormDescription(formName);
	NULLIF(frmDescr == NULL);
	foundFrm = frmDescr->FormObject;
	return foundFrm;
}

FormDescription* FormManager::GetFormDescription( char* formName )
{
	FormDescription* foundFrmDescr = NULL;
	for (int i=0; i < _registeredForms; i++)
	{
		if (StringEquals(_formsRepository[i].FormName, formName))
		{
			foundFrmDescr = &_formsRepository[i];
			break;
		}
	}

	return foundFrmDescr;
}

fresult FormManager::ShowForm(char* name)
{
	fresult fres;
	FAILIF(! (_shownIndex+1 < _formStackLength));

	IForm* prevForm = NULL;
	if (_shownIndex != -1)
	{
		prevForm = _formStack[_shownIndex];
	}

	IForm* frm = GetForm(name);
	FAILIF(frm==NULL);

	_shownIndex++;

	_formStack[_shownIndex] = frm;
	_formStackLength++;

	fres = ActivateForm(frm, NULL, FALSE, fsrNone);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult FormManager::CloseForm( IForm* frm,  FormShowResults formShowResult)
{
	fresult fres;
	FAILIF(_shownIndex == -1);

	//can't close the main form
	FAILIF(_shownIndex == 0);

	//can close only last form in stack
	FAILIF(frm != _formStack[_shownIndex]);

	_formStack[_shownIndex] = NULL;
	_shownIndex--;

	IForm* prevForm = _formStack[_shownIndex];

	bool_t needRedrawPervFrom = TRUE;
	if (frm->IsDialog()==TRUE)
	{
		IDialogForm* dlg = NULL; // @KL dynamic_cast<IDialogForm*>(frm);
		FAILIF(dlg==NULL);
		char* dialogName = dlg->GetDialogName();
		fres = prevForm->OnAfterDialogShown(frm, dialogName, formShowResult, &needRedrawPervFrom);
		ENSURESUCCESS(fres);
	}

	if (needRedrawPervFrom)
	{
		fres = ActivateForm(prevForm, frm, TRUE, formShowResult);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

IForm* FormManager::GetCurrentForm()
{
	NULLIF(_shownIndex == -1);
	return _formStack[_shownIndex];
}

fresult FormManager::GetOpenFormHandler( char* name, IMenuHandler** o_handler )
{
	FormDescription* frmDescr = GetFormDescription(name);
	FAILIF (frmDescr==NULL);

	*o_handler = (IMenuHandler*)&frmDescr->OpenFormHandler;
	return SUCCESS;
}

fresult FormManager::LayoutForms()
{
	fresult fres;
	for (ubyte_t i=0;i<_registeredForms;i++)
	{
		fres = _formsRepository[i].FormObject->DoLayout();
		ENSURESUCCESS(fres);
	}
	for (ubyte_t i=0;i<_registeredForms;i++)
	{
		fres = _formsRepository[i].FormObject->OnLoad();
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult FormManager::GetCloseFormHandler( IMenuHandler** o_handler)
{
	return GetCloseFormHandler(o_handler, fsrNone);
}

fresult FormManager::GetCloseFormHandler( IMenuHandler** o_handler, FormShowResults result )
{
	FAILIF(!(result<FormShowResultsCount));

	*o_handler = (IMenuHandler*)&_formCloseDelegateInstance[result];
	return SUCCESS;
}

fresult FormManager::ActivateForm( IForm* frm, IForm* prevForm , bool_t isReactivation, FormShowResults prevFormShowResult)
{
	fresult fres;

	fres = frm->OnBeforeShown(prevForm, isReactivation, prevFormShowResult);
	ENSURESUCCESS(fres);

	//OnBeforeShown might open some form. If it's so
	if (_formStack[_shownIndex] != frm)
	{
		//new form is shown
		return SUCCESS;
	}

	fres = frm->Draw();
	ENSURESUCCESS(fres);

	fres = frm->OnAfterShown(prevForm, isReactivation, prevFormShowResult);
	ENSURESUCCESS(fres);

	return SUCCESS;

}

fresult FormOpenDelegate::OnClick( IMenuItem* sender )
{
	fresult fres;
	fres = _formManager->ShowForm(_formName);
	ENSURESUCCESS(fres);
	return SUCCESS;
}

fresult FormOpenDelegate::Init( FormManager* frmmngr, char* formName )
{
	_formManager = frmmngr;
	_formName = formName;

	return SUCCESS;
}

fresult FormCloseDelegate::Init( FormManager* frmmngr, FormShowResults result)
{
	_formManager = frmmngr;
	_result = result;
	return SUCCESS;
}

fresult FormCloseDelegate::OnClick( IMenuItem* sender )
{
	fresult fres;
	fres =_formManager->CloseForm(_formManager->GetCurrentForm(), _result);
	ENSURESUCCESS(fres);

	return SUCCESS;
}
