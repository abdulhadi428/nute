#include "OpenSpace.h"


fresult YNDialogForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	_dialogName = NULL;

	return SUCCESS;
}

fresult YNDialogForm::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 3;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_TRANSPARENT, &_FormPanel);
	ENSURESUCCESS(fres);

	//get stipes
	Panel* stripes = NULL;
	fres = GetStripesPanel(frmStripeMessageYNDialog, &stripes);
	ENSURESUCCESS(fres);
	if (stripes!=NULL)
	{
		fres = _FormPanel->AppendControl(stripes);
		ENSURESUCCESS(fres);
	}

	//get message title
	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	FAILIF(tff==NULL);

	Position posTitle;
	posTitle.Left = 0;
	posTitle.Top = caPos.Top+4;
	
	tff->CurrentTextFormatHandle = TF_MENU;
	fres = tff->GetTextBox(posTitle, 26, &_txtHeader);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtHeader);
	ENSURESUCCESS(fres);

	tff->CurrentTextFormatHandle = TF_MENUEVEN;
	Position posContent;
	posContent.Left = 2;
	posContent.Top = posTitle.Top+16;
	Size txSize;
	txSize.Width = 26;
	txSize.Height =7;
	fres = tff->GetTextBox(posContent, txSize, 1, &_txtContent);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtContent);
	ENSURESUCCESS(fres);
	
	tff->CurrentTextFormatHandle = tff->ResetDefaults();
	//get menu
	fres = CreateMenu(&_Menu);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult YNDialogForm::CreateMenu( IMenu** o_mnu )
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
	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->ImgHandle = small_arrow_right_red;
	mis->OverrideTextFormat = TF_MENU;
	mis->Text = "                     ";
	fres = _FormManager->GetCloseFormHandler(&mis->Handler, fsrOK);
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
	_miYes = (MenuItem*)mnu->GetItem(0);
	FAILIF(_miYes==NULL);
	_miCancel =(MenuItem*)mnu->GetItem(1);
	FAILIF(_miCancel==NULL);

	//set color to red
	TextFormat* yesTf = _miYes->GetTextField()->GetTextFormat();
	Color yesForeColor;
	fres = _Repositories->Colors->GetColorById(CL_OS_LIGHTRED, &yesForeColor);
	ENSURESUCCESS(fres);
	yesTf->FgColor = yesForeColor;

	*o_mnu = mnu;
	return SUCCESS;
}

char* YNDialogForm::GetDialogName()
{
	return _dialogName;
}

fresult YNDialogForm::ShowYNDialog(char* dialogName, char* formTitle, char* formSubtitle, char* msgTitle, char* msgContent, char* yesText)
{
	fresult fres;
	_TitleText = formTitle;
	_SubtitleText = formSubtitle;
	_dialogName = dialogName;
	
	uword_t headerLen = Length(msgTitle);
	fres = _txtHeader->SetText(msgTitle);
	ENSURESUCCESS(fres);

	Size sz = _App->GetClientAreaSize();

	fres = HCenterTextField(_txtHeader, CenterHorizontaly, headerLen, sz.Width, 0);
	ENSURESUCCESS(fres);

	fres = _txtContent->SetText(msgContent);
	ENSURESUCCESS(fres);

	TextField* tf = _miYes->GetTextField();
	if (tf!=NULL)
	{
		fres = tf->SetText(yesText);
		ENSURESUCCESS(fres);
		uword_t yesTextLen = Length(yesText);
		HCenterTextField(tf, ToRight, yesTextLen, sz.Width, 8);
	}

	fres = _FormManager->ShowForm(GetName());
	ENSURESUCCESS(fres);

	return SUCCESS;
}

//not supported by the form
fresult YNDialogForm::ShowYNDialog( char* name, char* title, char* text, ImageHandle icon )
{
	return GENERAL_ERROR;
}

fresult YNDialogForm::ShowMessage( char* name, char* title, char* text, ImageHandle icon )
{
	return GENERAL_ERROR;
}
