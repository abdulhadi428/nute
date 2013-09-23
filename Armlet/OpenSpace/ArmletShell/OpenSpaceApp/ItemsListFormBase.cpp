#include "OpenSpace.h"


fresult ItemsListFormBase::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 5;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_TRANSPARENT, &_FormPanel);
	ENSURESUCCESS(fres);

	//get stipes
	Panel* stripes = NULL;
	fres = GetStripesPanel(frmStripeOneTopOneBot, &stripes);
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

	tff->CurrentTextFormatHandle = TF_GRAYTEXT;
	fres = tff->GetTextBox(posTitle, 26, &_txtItemHeader);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtItemHeader);
	ENSURESUCCESS(fres);

	tff->CurrentTextFormatHandle = TF_NORMAL_EVEN;
	Position posContent;
	Size szContent;
	//- for scroll menu
	fres = GetContentDimensions(&posContent, &szContent);
	ENSURESUCCESS(fres);

	szContent.Width = szContent.Width - 13;
	fres = tff->GetTextBox(posContent, szContent, &_txtItemContent);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtItemContent);
	ENSURESUCCESS(fres);

	tff->ResetDefaults();
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
	MenuItemSettings* settings = mf->Settings;
	MenuItemSettings* mis;

	//ItemOriginX
	mis = &settings[ItemOriginX];
	mis->ImgHandle = scroll_up;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &settings[ItemOriginZ];
	mis->ImgHandle = scroll_down;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginL
	mis = &settings[ItemOriginL];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = _prevText;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginE
	mis = &settings[ItemOriginE];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "назад";
	fres = _FormManager->GetCloseFormHandler(&mis->Handler, fsrCancel);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;
	
	//ItemOriginR
	mis = &settings[ItemOriginR];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = _nextText;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;
	//call derived update settings
	UpdateMenuSettings(settings);

	//create mnu
	IMenu* mnu = NULL;
	fres = mf->GetMenu(ABCXYZLER, &mnu);
	ENSURESUCCESS(fres);

	for (ubyte_t i=0; i<mnu->GetItemsCount();i++)
	{
		MenuItem* mi = (MenuItem*)(mnu->GetItem(i));
		if (StringEquals(mi->GetName(), "L") == TRUE)
		{
			_mnuBack = mi;
		}
		if (StringEquals(mi->GetName(), "R") == TRUE)
		{
			_mnuNext = mi;
		}
	}

	//Clean up
	mf->ClearSettings();
	mf->DefaultInterlaced=mf->CurrentInterlaced;
	mf->DefaultTextFormatHandle=mf->CurrentTextFormatHandle;
	mf->DefaultEvenTextFormatHandle=mf->CurrentEvenTextFormatHandle;

	//Get menu Items

	*o_mnu = mnu;
	return SUCCESS;
}

fresult ItemsListFormBase::SetItemText(char* itemText)
{
	fresult fres;

	fres = _txtItemContent->SetText(itemText);
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

fresult ItemsListFormBase::UpdateMenuSettings(MenuItemSettings* ms)
{
	return SUCCESS;
}

fresult ItemsListFormBase::OnMenuL( IMenuItem* mi )
{
	fresult fres;
	sbyte_t index;
	index = GetPrevItemIndex(_currentItemIndex);
	
	if (index != -1)
	{
		fres = SetActiveItem(index);
		ENSURESUCCESS(fres);
	}
	else
	{
		//should't be here
		fres = SetActiveItem(_currentItemIndex);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult ItemsListFormBase::OnMenuR( IMenuItem* mi )
{
	fresult fres;
	sbyte_t index;
	index = GetNextItemIndex(_currentItemIndex);

	if (index != -1)
	{
		fres = SetActiveItem(index);
		ENSURESUCCESS(fres);
	}
	else
	{
		//should't be here
		fres = SetActiveItem(_currentItemIndex);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

sbyte_t ItemsListFormBase::GetNextItemIndex(sbyte_t index)
{
	if (index+1>=_currentItemIndex)
	{
		return -1;
	}

	return index++;
}

sbyte_t ItemsListFormBase::GetPrevItemIndex(sbyte_t index)
{
	if (index-1 < 0)
	{
		return -1;
	}

	return index--;
}

fresult ItemsListFormBase::SetActiveItem( sbyte_t index )
{
	fresult fres;

	_currentItemIndex = index;

	sbyte_t prevIndex = GetPrevItemIndex(index);
	
	if (prevIndex == -1)
	{
		fres = _mnuBack->SetVisible(FALSE);
		ENSURESUCCESS(fres);
		fres = _mnuBack->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
	}
	else
	{
		fres = _mnuBack->SetVisible(TRUE);
		ENSURESUCCESS(fres);
		fres = _mnuBack->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
	}
	
	sbyte_t nextIndex = GetNextItemIndex(index);
	if (nextIndex == -1)
	{
		fres = _mnuNext->SetVisible(FALSE);
		ENSURESUCCESS(fres);
		fres = _mnuNext->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
	}
	else
	{
		fres = _mnuNext->SetVisible(TRUE);
		ENSURESUCCESS(fres);
		fres = _mnuNext->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;

}

fresult ItemsListFormBase::OnMenuX( IMenuItem* mi )
{
	return _txtItemContent->ScrollUp();
}

fresult ItemsListFormBase::OnMenuZ( IMenuItem* mi )
{
	return _txtItemContent->ScrollDown();
}
