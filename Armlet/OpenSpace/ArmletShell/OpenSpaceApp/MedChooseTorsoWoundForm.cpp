#include "OpenSpace.h"

#define NEW_WOUND_DIALOG_NAME "ConfirmNewWound"

fresult MedChooseTorsoWoundForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	_TitleText = "Медицина/";
	_SubtitleText = "Добавить\nпроблему";

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MedChooseTorsoWoundForm::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 3;

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

	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	FAILIF(tff==NULL);

	uword_t captionLen;
	char* caption;
	Position pos;

	// _txtTorsoTraumaCaption
	tff->CurrentTextFormatHandle = TF_MENU;
	caption = "Травма корпуса";
	captionLen = Length(caption);

	pos.Left =0;
	pos.Top = caPos.Top+4;
	fres = tff->GetTextBox(pos, caption, &_txtTorsoTraumaCaption);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtTorsoTraumaCaption);
	ENSURESUCCESS(fres);
	fres =HCenterTextField(_txtTorsoTraumaCaption, CenterHorizontaly, captionLen, caSz.Width, 0);
	ENSURESUCCESS(fres);

	// _txtChoosePlaceCaption
	pos.Left =0;
	pos.Top += 16;
	tff->CurrentTextFormatHandle = TF_GRAYTEXT_EVEN;
	tff->DefaultWrap = FALSE;
	caption = "Выберите место";
	captionLen = Length(caption);

	fres = tff->GetTextBox(pos, caption, &_txtChoosePlaceCaption);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtChoosePlaceCaption);
	ENSURESUCCESS(fres);
	fres =HCenterTextField(_txtChoosePlaceCaption, CenterHorizontaly, captionLen, caSz.Width, 0);
	ENSURESUCCESS(fres);

	tff->ResetDefaults();

	//get menu
	fres = CreateMenu(&_Menu);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MedChooseTorsoWoundForm::CreateMenu( IMenu** o_mnu )
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

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->Text = "Верх корпуса";
	mis->ImgHandle = small_arrow_left;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->Text = "Низ корпуса";
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

fresult MedChooseTorsoWoundForm::OnMenu( IMenuItem* mi, bool_t* handled )
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

			case ItemOriginB:
				_currentTarget = mwtUpTorso;
				hitTargetName = "Травма верх корпуса";
				found = TRUE;
				break;
			case ItemOriginY:
				_currentTarget = mwtUpTorso;
				hitTargetName = "Травма низа корпуса";
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

fresult MedChooseTorsoWoundForm::OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw )
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

fresult MedChooseTorsoWoundForm::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults results )
{
	fresult fres;
	fres = OpenSpaceFormBase::OnBeforeShown(prevFrom, reActivation, results);
	ENSURESUCCESS(fres);

	if (reActivation == FALSE)
	{
		_currentTarget = mwtUnknownTarget;
	}

	return SUCCESS;
}

