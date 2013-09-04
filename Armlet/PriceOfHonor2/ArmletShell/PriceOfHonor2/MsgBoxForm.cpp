#include "ThreeKShell.h"
#include "To3KShell.h"
#include "Honor2.h"
#include "MsgBoxForm.h"

ALLOCATE_MENU_HANDLERS(MsgBoxForm,20);
DEFINE_MENU_HANDLER(MsgBoxForm);

fresult MsgBoxForm::CreateMenu( IMenu** o_mnu )
{
	fresult fres;
	MenuFactory* mf = _Factories->GetMenuFactory();
	MenuItemFactory* mif = _Factories->GetMenuItemFactory();

	//ready factory
	mf->CurrentInterlaced = FALSE;
	mf->CurrentTextFormatHandle = TF_MENU;
	mf->CurrentEvenTextFormatHandle = TF_MENU;
	mif->CurrentTextLines = 2;

	mf->ClearSettings();

	//Set up menu
	MenuItemSettings* mis;
	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = GreenOk;
	mis->Handler = CREATE_MENU_HANDLER(MsgBoxForm, HideMessageBox);
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
	mif->CurrentTextLines= mif->DefaultTextLines;
	*o_mnu = mnu;

	return SUCCESS;

}

fresult MsgBoxForm::DoLayout()
{
	fresult fres;
	ubyte_t controlsCount =2;

	Position pos;
	pos.Top = 0;
	pos.Left = 0;

	Size sz;
	sz = _App->GetDisplaySize();

	fres = _Factories->GetPanelFactory()->GetPanel(sz, pos, controlsCount, CL_CONTROL_BACKGROUND, &_FormPanel);
	ENSURESUCCESS(fres);

	//get header
	fres = CreateHeader(INVALID_HANDLE, NULL, NULL);
	ENSURESUCCESS(fres);
	_txtHeaderSubTitle->SetVisible(FALSE);

	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	tff->CurrentTextFormatHandle = TF_NORMAL;
	tff->CurrentFrames =2;
	tff->CurrentLines = 8;
	tff->CurrentWrap = TRUE;
	sz.Height = sz.Height - _pnlPanelHeader->GetSize().Height - 30;
	pos.Top = _pnlPanelHeader->GetSize().Height;
	
	fres = tff->GetTextBox(pos, sz, &_txtMsgBoxText);
	ENSURESUCCESS(fres);
	
	fres = _FormPanel->AppendControl(_txtMsgBoxText);
	ENSURESUCCESS(fres);

	tff->CurrentTextFormatHandle = tff->DefaultTextFormatHandle;
	tff->CurrentFrames =tff->DefaultFrames;
	tff->CurrentLines = tff->DefaultLines;
	tff->CurrentWrap = tff->DefaultWrap;

	_Menus = AllocMenus(1);
	FAILIF(_Menus==NULL);

	IMenu* mnu = NULL;
	fres = CreateMenu(&mnu);
	ENSURESUCCESS(fres);
	if (mnu!=NULL)
	{
		_Menus[0] = mnu;
	}

	return SUCCESS;
}

fresult MsgBoxForm::Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic )
{
	fresult fres;
	fres = BaseInit(TRUE,reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	_currentMessageIndex =-1;

	for (ubyte_t i=0; i < _MSGBOX_MAX_STACK;i++)
	{
		_messages->Empty = TRUE;
	}

	return SUCCESS;
}

fresult MsgBoxForm::ShowMessage( char* title, ImageHandle iconHandle, char* text )
{
	fresult fres;

	FAILIF(!(_currentMessageIndex+1 < _MSGBOX_MAX_STACK));

	_currentMessageIndex++;
	_messages[_currentMessageIndex].Empty = FALSE;
	_messages[_currentMessageIndex].icon = iconHandle;
	_messages[_currentMessageIndex].text = text;
	_messages[_currentMessageIndex].title = title;

	BitmapImage* bmp = _Repositories->Images->GetImageById(iconHandle);

	fres = SetMsgBoxData();
	ENSURESUCCESS(fres);

	if (_currentMessageIndex == 0)
	{
		fres = _FormManager->ShowForm(_App->Forms->MsgBoxFormName);
		ENSURESUCCESS(fres);
	}
	else
	{
		fres = Draw();
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult MsgBoxForm::SetMsgBoxData()
{
	fresult fres;
	BitmapImage* bmp = _Repositories->Images->GetImageById(_messages[_currentMessageIndex].icon);

	fres = _pbxHeaderIcon->SetImage(bmp);
	ENSURESUCCESS(fres);

	fres = _txtHeaderTitle->SetText(_messages[_currentMessageIndex].title);
	ENSURESUCCESS(fres);

	fres = _txtMsgBoxText->SetText(_messages[_currentMessageIndex].text);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MsgBoxForm::HideMessageBox( IMenuItem* sender )
{
	fresult fres;
	_messages[_currentMessageIndex].Empty = true;
	_currentMessageIndex--;
	if(_currentMessageIndex !=-1)
	{
		fres = SetMsgBoxData();
		ENSURESUCCESS(fres);
		fres = Draw();
		ENSURESUCCESS(fres);
	}
	else
	{
		fres = _FormManager->CloseForm(_FormManager->GetCurrentForm());
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}




