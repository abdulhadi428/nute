#include "OpenSpace.h"

fresult MedStatusForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	_TitleText = "Медицина/";
	_SubtitleText = "Состояние";

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MedStatusForm::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 2;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_MENU_BACKGROUND, &_FormPanel);
	ENSURESUCCESS(fres);

	//log text
	Position pos;
	pos.Left =caPos.Left + 6;
	pos.Top = caPos.Top;
	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	//TODO: override TF_NORMAL
	tff->CurrentTextFormatHandle=TF_MENU;
	tff->CurrentFrames = 2;

	Size tfSize;
	tfSize.Width = caSz.Width - 22;
	tfSize.Height = caSz.Height -16;

	fres = tff->GetTextBox(pos, tfSize, &_txtStatus);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtStatus);
	ENSURESUCCESS(fres);

	//get stipes
	Panel* stripes = NULL;
	fres = GetStripesPanel(frmStripeOneBot, &stripes);
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


fresult MedStatusForm::CreateMenu( IMenu** o_mnu )
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

	//ItemOriginX
	mis = &mf->Settings[ItemOriginX];
	mis->ImgHandle = scroll_up;
	//mis->Handler = _txtStatus.Get
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->ImgHandle = scroll_down;
	//mis->Handler = _txtStatus.Get
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

fresult MedStatusForm::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults results )
{
	fresult fres;

	fres = OpenSpaceFormBase::OnBeforeShown(prevFrom, reActivation, results);

	fres = _App->Logic->MedLogic->CreateMedStatus(_txtStatus);
	ENSURESUCCESS(fres);

	return SUCCESS;
}



