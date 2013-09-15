#include "ThreeKShell.h"
#include "To3KShell.h"

fresult FormBase::BaseInit( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, ApplicationBase* app )
{
	FAILIF(reps==NULL);
	FAILIF(facts==NULL);
	FAILIF(name==NULL);
	FAILIF(frmmngr==NULL);
	FAILIF(app==NULL);

	_Repositories = reps;
	_Factories = facts;
	_FormManager = frmmngr;
	_Name = name;
	_AppBase = app;
	
	_FormPanel = NULL;
	_Menu = &_mnuInstance;
	_IsDialog = FALSE;


	return SUCCESS;
}

char* FormBase::GetName()
{
	return _Name;
}

fresult FormBase::OnButtonEvent( ButtonState key, bool* found)
{
	return _Menu->ProcessButton(key, found);
}

fresult FormBase::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	return SUCCESS;
}

fresult FormBase::OnAfterShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	return SUCCESS;
}

fresult FormBase::OnLoad()
{
	return SUCCESS;
}

fresult FormBase::Draw()
{
	fresult fres;

	if (_FormPanel!=NULL)
	{
		fres = _FormPanel->Draw();
		ENSURESUCCESS(fres);
	}

	fres = _Menu->Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

bool_t FormBase::IsDialog()
{
	return _IsDialog;
}

fresult FormBase::OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw )
{
	*needRedraw = TRUE;
	return SUCCESS;
}


