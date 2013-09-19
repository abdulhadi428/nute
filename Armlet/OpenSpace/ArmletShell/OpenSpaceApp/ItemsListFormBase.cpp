#include "OpenSpace.h"


fresult ItemsListFormBase::BaseInit( char** itemsTexts, ubyte_t startIndex, ubyte_t itemsCount, Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;

	_currentIndex = startIndex;
	_itemsCount = itemsCount;
	_itemsTexts = itemsTexts;

	fres = OpenSpaceFormBase::BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult ItemsListFormBase::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 1;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_TRANSPARENT, &_FormPanel);
	ENSURESUCCESS(fres);


	TextField* tf;
	Position pos;
	pos.Left =5;
	pos.Top = 50;
	TextFieldFactory* tff = _Factories->GetTextFieldFactory();

	tff->CurrentTextFormatHandle=TF_PARROT;

	Size tftxSize;
	tftxSize.Width = 14;
	tftxSize.Height = 6;

	fres = tff->GetTextBox(pos, tftxSize, 1, &tf);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(tf);
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

fresult ItemsListFormBase::CreateMenu( IMenu** o_mnu )
{
	fresult fres;
	MenuFactory* mf = _Factories->GetMenuFactory();

	//ready factory
	mf->CurrentInterlaced = true;
	mf->CurrentTextFormatHandle = TF_MENU;
	mf->CurrentEvenTextFormatHandle = TF_MENUEVEN;
	mf->ClearSettings();


	//Set up menu
	MenuItemSettings* settings = GetMenuSettings();
	FAILIF(settings==NULL);
	MenuItemSettings* mis;
	
	//ItemOriginE
	mis = &settings[ItemOriginE];
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

fresult ItemsListFormBase::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	fresult fres;

	fres = OpenSpaceFormBase::OnBeforeShown(prevFrom, reActivation, formShowResult);
	ENSURESUCCESS(fres);

	fres = SetItem(_currentIndex);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult ItemsListFormBase::SetItem( ubyte_t index )
{
	fresult fres;
	FAILIF(index <0 || index >= _itemsCount);
	_currentIndex = index;

	fres = _txtItemContent->SetText(_itemsTexts[_currentIndex]);
	ENSURESUCCESS(fres);

	fres = _App->RedrawIfActive(this);
	ENSURESUCCESS(fres);
	
	return SUCCESS;
}

fresult ItemsListFormBase::SetItemHeader( char* headerText, HorizontalAlignmentStyles alignment, ubyte_t leftOffset)
{
	fresult fres;

	if (_txtItemHeader != NULL)
	{
		fres = HCenterTextField(_txtItemHeader, alignment, Length(headerText), _App->GetClientAreaSize().Width, leftOffset);
		ENSURESUCCESS(fres);
		fres = _txtItemHeader->SetText(headerText);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult ItemsListFormBase::MoveToNext()
{
	return SUCCESS;
}

fresult ItemsListFormBase::MoveToPrev()
{
	return SUCCESS;
}

MenuItemSettings* ItemsListFormBase::GetMenuSettings()
{
	MenuFactory* mf = _Factories->GetMenuFactory();
	
	return mf->Settings;
}
