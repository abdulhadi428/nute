 #include "ThreeKShell.h"
#include "To3KShell.h"
#include "Honor2.h"
#include "Honor2MainForm.h"

ALLOCATE_MENU_HANDLERS(Honor2MainForm,20);
DEFINE_MENU_HANDLER(Honor2MainForm);

//TODO: remove logic. it can't be inited yet!
fresult Honor2MainForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic )
{
	fresult fres;
	fres = BaseInit(FALSE,reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult Honor2MainForm::DoLayout()
{
	ubyte_t controlsCount = 2;

	fresult fres;

	Size caSz;
	caSz.Width = _App->GetDisplaySize().Width;
	caSz.Height = _App->GetDisplaySize().Height - _App->GetStatusBarSize().Height;
	_FormClientSize.data =  caSz.data;
	
	Position caPos;
	caPos.Top = _App->GetStatusBarSize().Height;
	caPos.Left = 0;

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_CONTROL_BACKGROUND, &_FormPanel);
	ENSURESUCCESS(fres);

	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	tff->CurrentWrap = TRUE;
	tff->CurrentFrames = 5;
	tff->CurrentLines = 12;
	
	Position pos;
	pos.Left = 26;
	pos.Top = caPos.Top+2;
	fres = tff->GetTextBox(pos, 18, &_txtStatus);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtStatus);
	ENSURESUCCESS(fres);

	tff->CurrentWrap = tff->DefaultWrap;
	tff->CurrentFrames = tff->DefaultFrames;
	tff->CurrentLines = tff->DefaultLines;
	tff->CurrentTextFormatHandle = tff->DefaultTextFormatHandle;


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

fresult Honor2MainForm::CreateMenu( IMenu** o_mnu )
{
	fresult fres;
	MenuFactory* mf = _Factories->GetMenuFactory();

	//ready factory
	mf->CurrentInterlaced = FALSE;
	mf->CurrentTextFormatHandle = TF_MENU;
	mf->CurrentEvenTextFormatHandle = TF_MENU;
	mf->ClearSettings();

	//Set up menu
	MenuItemSettings* mis;
	//ItemOriginA
	mis = &mf->Settings[ItemOriginA];
	mis->Empty = TRUE;

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->ImgHandle = BlueArrowUp;
	mis->Handler = CREATE_MENU_HANDLER(Honor2MainForm, Scroll);
	mis->Empty = FALSE;

	//ItemOriginÑ
	mis = &mf->Settings[ItemOriginC];
	mis->ImgHandle = BlueArrowDown;
	mis->Handler = CREATE_MENU_HANDLER(Honor2MainForm, Scroll);
	mis->Empty = FALSE;

	//ItemOriginX
	mis = &mf->Settings[ItemOriginX];
	mis->ImgHandle = OrangeHealth;
	fres = _FormManager->GetOpenFormHandler(_App->Forms->NewWoundFormName, &mis->Handler);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->ImgHandle = OrangeTarget;
	mis->Handler = CREATE_MENU_HANDLER(Honor2MainForm, OnKnockOut);
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->ImgHandle = OrangeDocument;
	fres = _FormManager->GetOpenFormHandler(_App->Forms->LogFormName, &mis->Handler);
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

	*o_mnu = mnu;

	return SUCCESS;

}

ubyte_t Honor2MainForm::FormControlsCount()
{
	return 1;
}

fresult Honor2MainForm::OnBeforeShow( IForm* prevFrom, bool_t reActivation )
{
	fresult fres;
	fres = Honor2FormBase::OnBeforeShow(prevFrom, reActivation);
	ENSURESUCCESS(fres);

	if (!_App->AppStatusBar->GetVisible())
	{
		_App->AppStatusBar->SetVisible(true);
	}
	fres = _App->AppStatusBar->Draw();
	ENSURESUCCESS(fres);
	return SUCCESS;
}


fresult Honor2MainForm::OnKnockOut( IMenuItem* Sender )
{
	fresult fres;

	fres = _App->Logic->OnKnockout();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult Honor2MainForm::SetStatus( char* statusText )
{
	fresult fres;

	if (_txtStatus!=NULL)
	{
		fres = _txtStatus->SetText(statusText);
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}

fresult Honor2MainForm::Scroll( IMenuItem* sender )
{
	fresult fres;
	MenuItemBase* mi = (MenuItemBase*)sender;

	if (mi->GetAccelerator()==BUTTON_A)
	{
		fres = _txtStatus->ScrollUp();
	}
	else
	{
		fres = _txtStatus->ScrollDown();
	}
	ENSURESUCCESS(fres);

	return SUCCESS;
}
