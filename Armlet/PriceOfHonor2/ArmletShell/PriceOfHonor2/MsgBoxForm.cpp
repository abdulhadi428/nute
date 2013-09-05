#include "ThreeKShell.h"
#include "To3KShell.h"
#include "Honor2.h"
#include "MsgBoxForm.h"

ALLOCATE_MENU_HANDLERS(MsgBoxForm,20);
DEFINE_MENU_HANDLER(MsgBoxForm);

fresult MsgBoxForm::Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic )
{
	fresult fres;
	fres = BaseInit(TRUE,reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	_currentMessageIndex =-1;

	for (ubyte_t i=0; i < _MSGBOX_MAX_STACK;i++)
	{
		_messages->Empty = TRUE;
		StrPad(_messages->text, 0, 0, MAX_MSG_LEN);
		_messages->YesNoDialog = FALSE;
	}
	
	return SUCCESS;
}

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

	mis = &mf->Settings[ItemOriginL];
	mis->ImgHandle = GreenOk;
	mis->Handler = CREATE_MENU_HANDLER(MsgBoxForm, CloseMessageBoxWithOk);
	mis->Empty = FALSE;

	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = GreenOk;
	mis->Handler = CREATE_MENU_HANDLER(MsgBoxForm, CloseMessageBoxWithOk);
	mis->Empty = FALSE;

	mis = &mf->Settings[ItemOriginR];
	mis->ImgHandle = RedCancel;
	mis->Handler = CREATE_MENU_HANDLER(MsgBoxForm, CloseMessageBoxWithCancel);
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

	//get all buttons
	_miYes = mnu->GetItem(0);
	FAILIF(_miYes==NULL);
	_miOk = mnu->GetItem(1);
	FAILIF(_miOk==NULL);
	_miCancel = mnu->GetItem(2);
	FAILIF(_miCancel==NULL);

	//hide all buttons		
	fres = _miYes->SetVisible(FALSE);
	ENSURESUCCESS(fres);
	fres = _miOk->SetVisible(FALSE);
	ENSURESUCCESS(fres);
	fres = _miCancel->SetVisible(FALSE);
	ENSURESUCCESS(fres);


	fres = _miYes->SetEnabled(FALSE);
	ENSURESUCCESS(fres);
	fres = _miOk->SetEnabled(FALSE);
	ENSURESUCCESS(fres);
	fres = _miCancel->SetEnabled(FALSE);
	ENSURESUCCESS(fres);

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
	tff->CurrentFrames =1;
	tff->CurrentLines = 6;
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


fresult MsgBoxForm::ShowMessage(bool_t yesNoDialog, char* title, ImageHandle iconHandle, char* text)
{
	fresult fres;

	FAILIF(!(_currentMessageIndex+1 < _MSGBOX_MAX_STACK));

	//yesno may be only the first in stack
	FAILIF(_currentMessageIndex!=-1 && yesNoDialog == TRUE);

	_currentMessageIndex++;
	_messages[_currentMessageIndex].Empty = FALSE;
	_messages[_currentMessageIndex].icon = iconHandle;
	_messages[_currentMessageIndex].YesNoDialog = yesNoDialog;
	fres = StrCopy(_messages[_currentMessageIndex].text,0, text, MAX_MSG_LEN);
	ENSURESUCCESS(fres);
	_messages[_currentMessageIndex].title = title;

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

fresult MsgBoxForm::ShowMessageYesNo(char* title, ImageHandle iconHandle, char* text )
{
	return ShowMessage(TRUE, title, iconHandle, text);
}

fresult MsgBoxForm::ShowMessage( char* title, ImageHandle iconHandle, char* text )
{
	return ShowMessage(FALSE, title, iconHandle, text);
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

	if (_messages[_currentMessageIndex].YesNoDialog ==  TRUE)
	{
		//hide all buttons		
		fres = _miYes->SetVisible(TRUE);
		ENSURESUCCESS(fres);
		fres = _miOk->SetVisible(FALSE);
		ENSURESUCCESS(fres);
		fres = _miCancel->SetVisible(TRUE);
		ENSURESUCCESS(fres);

		fres = _miYes->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
		fres = _miOk->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
		fres = _miCancel->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
	}
	else
	{
		//hide all buttons		
		fres = _miYes->SetVisible(FALSE);
		ENSURESUCCESS(fres);
		fres = _miOk->SetVisible(TRUE);
		ENSURESUCCESS(fres);
		fres = _miCancel->SetVisible(FALSE);
		ENSURESUCCESS(fres);

		//hide all buttons		
		fres = _miYes->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
		fres = _miOk->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
		fres = _miCancel->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult MsgBoxForm::CloseMessageBox( FormShowResults result )
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
		fres = _FormManager->CloseForm(_FormManager->GetCurrentForm(), result);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult MsgBoxForm::CloseMessageBoxWithOk( IMenuItem* sender )
{
	return CloseMessageBox(fsrOK);
}

fresult MsgBoxForm::CloseMessageBoxWithCancel( IMenuItem* sender )
{
	return CloseMessageBox(fsrCancel);
}

MessageBoxContent* MsgBoxForm::GetCurrentMessage()
{
	if (_currentMessageIndex == -1)
	{
		return NULL;
	}
	else
	{
		return &_messages[_currentMessageIndex];
	}

}

MessageBoxContent* MsgBoxForm::GetLastYesNoDialog()
{
	if (_currentMessageIndex != -1)
	{
		return NULL;
	}
	else
	{
		//dialog may be only first (0-index) message
		if (_messages[0].YesNoDialog != TRUE)
		{
			return NULL;
		}

		return &_messages[0];
	}
}



