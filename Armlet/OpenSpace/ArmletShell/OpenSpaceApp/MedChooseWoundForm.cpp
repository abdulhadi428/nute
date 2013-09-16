#include "OpenSpace.h"

#define NEW_WOUND_DIALOG_NAME "ConfirmNewWound"

fresult MedChooseWoundForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	_TitleText = "Медицина/";
	_SubtitleText = "Добавить\nпроблему";

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MedChooseWoundForm::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 1;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_TRANSPARENT, &_FormPanel);
	ENSURESUCCESS(fres);

	//get stipes
	Panel* stripes = NULL;
	fres = GetStripesPanel(frmStripeSevenNarrow, &stripes);
	ENSURESUCCESS(fres);
	if (stripes!=NULL)
	{
		fres = _FormPanel->AppendControl(stripes);
		ENSURESUCCESS(fres);
	}

	//get menu
	fres = CreateMenu(&_Menu);
	ENSURESUCCESS(fres);

	return SUCCESS;
}


fresult MedChooseWoundForm::CreateMenu( IMenu** o_mnu )
{
	fresult fres;
	MenuFactory* mf = _Factories->GetMenuFactory();

	//ready factory
	mf->CurrentInterlaced = true;
	mf->CurrentTextFormatHandle = TF_MENU;
	mf->CurrentEvenTextFormatHandle = TF_MENUEVEN;
	mf->ClearSettings();

	//Set up menu
	MenuItemSettings* mis;


	//ItemOriginA
	mis = &mf->Settings[ItemOriginA];
	mis->Text = "Травма головы";
	mis->ImgHandle = small_arrow_left;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->Text = "Травма\nконечности";
	mis->ImgHandle = small_arrow_left;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginС
	mis = &mf->Settings[ItemOriginC];
	mis->Text = "Травма\nкорпуса";
	mis->ImgHandle = small_arrow_left;
	fres = _FormManager->GetOpenFormHandler(_App->Forms->MedChooseTorsoWoundFormName,&mis->Handler);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	//ItemOriginX
	mis = &mf->Settings[ItemOriginX];
	mis->Text = "Избит";
	mis->ImgHandle = small_arrow_right;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->Text = "Оглушен";
	mis->ImgHandle = small_arrow_right;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->Text = "При смерти";
	mis->ImgHandle = small_arrow_right;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;


	//ItemOriginE
	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "назад";
	fres = _FormManager->GetCloseFormHandler(&mis->Handler, fsrCancel);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	//create mnu
	IMenu* mnu = NULL;
	fres = mf->GetMenu(ABCXYZLER, &mnu);
	ENSURESUCCESS(fres);

	//Clean up
	mf->ClearSettings();
	mf->DefaultInterlaced=mf->CurrentInterlaced;
	mf->DefaultTextFormatHandle=mf->CurrentTextFormatHandle;
	mf->DefaultEvenTextFormatHandle=mf->CurrentEvenTextFormatHandle;

	//Get menu Items

	*o_mnu = mnu;
	return SUCCESS;
}

fresult MedChooseWoundForm::OnMenu( IMenuItem* mi, bool_t* handled )
{
	fresult fres;

	_currentTarget =  mwtUnknownTarget;
	char* hitTargetName;

	//search menu handler
	char* miName = mi->GetName();
	MenuFactory* mf = _Factories->GetMenuFactory();
	FAILIF(mf==NULL);
	MenuItemSettings* mis = mf->Settings;
	bool_t found = FALSE;

	for(ubyte_t i=0; i<MenuItemOriginsLast;i++)
	{
		if (StringEquals(miName, mis[i].Name) == TRUE)
		{
			switch (i)
			{
			case ItemOriginA:
				_currentTarget = mwtHeadTarget;
				hitTargetName = "Травма головы";
				found = TRUE;
				break;
			case ItemOriginB:
				_currentTarget = mwtLimbTarget;
				hitTargetName = "Травма конечности";
				found = TRUE;
				break;
			case ItemOriginX:
				_currentTarget = mwtBeatenTarget;
				hitTargetName = "Избит";
				found = TRUE;
				break;
			case ItemOriginY:
				_currentTarget = mwtStunnedTarget;
				hitTargetName = "Оглушен";
				found = TRUE;
				break;
			case ItemOriginZ:
				_currentTarget = mwtDyingTarget;
				hitTargetName = "При смерти";
				found = TRUE;
				break;
			default:
				found = FALSE;
			}
			break;
		}
	}
	if (found != TRUE)
	{
		return GENERAL_ERROR;
	}

	char* woundDescription = NULL;
	woundDescription = _App->Logic->MedLogic->GetWoundTargetText(_currentTarget);

	fres = _App->ShowYNDialogEx(NEW_WOUND_DIALOG_NAME, _TitleText, "добавить\nпроблему", hitTargetName, woundDescription, "добавить проблему");
	ENSURESUCCESS(fres);

	*handled = TRUE;

	return SUCCESS;
}

fresult MedChooseWoundForm::OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw )
{
	fresult fres;
	if (StringEquals(NEW_WOUND_DIALOG_NAME, dialogName))
	{
		if (results == fsrOK)
		{
			fres = _App->Logic->MedLogic->AddNewWound(_currentTarget);
			ENSURESUCCESS(fres);
		}
	}
	
	return SUCCESS;
}

fresult MedChooseWoundForm::OnBeforeShow( IForm* prevFrom, bool_t reActivation, FormShowResults results )
{
	if (reActivation == FALSE)
	{
		_currentTarget = mwtUnknownTarget;
	}

	return SUCCESS;
}


