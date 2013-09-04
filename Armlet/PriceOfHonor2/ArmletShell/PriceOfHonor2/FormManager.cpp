#include "IForm.h"
#include "ArmletShell.h"

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

	fres = _formCloseDelegateInstance.Init(this);
	ENSURESUCCESS(fres);

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

	fres = frm->OnBeforeShow(prevForm, FALSE);
	ENSURESUCCESS(fres);

	fres = frm->Draw();
	ENSURESUCCESS(fres);

	fres = frm->OnAfterShow(prevForm, FALSE);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult FormManager::CloseForm( IForm* frm)
{
	fresult fres;
	FAILIF(_shownIndex == -1);

	//can't close the main form
	FAILIF(_shownIndex == 0);

	//can close only last form in stack
	FAILIF(frm != _formStack[_shownIndex]);

	_formStack[_shownIndex] = NULL;
	_shownIndex--;

	IForm* frmToShow = _formStack[_shownIndex];

	fres = frmToShow->OnBeforeShow(frm, TRUE);
	ENSURESUCCESS(fres);

	fres = frmToShow->Draw();
	ENSURESUCCESS(fres);

	fres = frmToShow->OnAfterShow(frm, TRUE);
	ENSURESUCCESS(fres);

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

fresult FormManager::GetCloseFormHandler( IMenuHandler** o_handler )
{
	*o_handler = (IMenuHandler*)&_formCloseDelegateInstance;
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


fresult FormCloseDelegate::Init( FormManager* frmmngr )
{
	_formManager = frmmngr;
	return SUCCESS;
}

fresult FormCloseDelegate::OnClick( IMenuItem* sender )
{
	fresult fres;
	fres =_formManager->CloseForm(_formManager->GetCurrentForm());
	ENSURESUCCESS(fres);

	return SUCCESS;
}
