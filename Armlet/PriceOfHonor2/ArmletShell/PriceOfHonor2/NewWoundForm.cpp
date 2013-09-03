#include "ThreeKShell.h"
#include "To3KShell.h"
#include "Honor2.h"
#include "NewWoundForm.h"
#include "med.h"

fresult NewWoundForm::CreateMenu( IMenu** o_mnu )
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

	IMenuHandler* mh = CREATE_MENU_HANDLER(NewWoundForm, OnWound);

	//ItemOriginA
	mis = &mf->Settings[ItemOriginA];
	mis->Text = "Голова\n     ";
	mis->Handler = mh;
	mis->Empty = FALSE;

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->Text = "Живот\n     ";
	mis->Handler = mh;
	mis->Empty = FALSE;

	//ItemOriginС
	mis = &mf->Settings[ItemOriginC];
	mis->Text = "Левая\n рука ";
	mis->Handler = mh;
	mis->Empty = FALSE;

	//ItemOriginX
	mis = &mf->Settings[ItemOriginX];
	mis->Text = "Грудь\n     ";
	mis->ScreenOrigin.Top = mis->ScreenOrigin.Top -5;
	mis->Handler = mh;
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->Text = "Спина\n     ";
	mis->ScreenOrigin.Top = mis->ScreenOrigin.Top -5;
	mis->Handler = mh;
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->Text = "Правая\n рука";
	mis->ScreenOrigin.Top = mis->ScreenOrigin.Top -5;
	mis->Handler = mh;
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginL];
	mis->Text = "Левая\n нога";
	mis->Handler = mh;
	mis->Empty = FALSE;

	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = GreenOk;
	fres = _FormManager->GetCloseFormHandler(&mis->Handler);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginR];
	mis->Text = "Правая\n нога";
	mis->Handler = mh;
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

fresult NewWoundForm::DoLayout()
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
	fres = CreateHeader(INVALID_HANDLE, "Укажи место ранения", NULL);
	ENSURESUCCESS(fres);
	_txtHeaderSubTitle->SetVisible(FALSE);
	_pbxHeaderIcon->SetVisible(FALSE);
	
	Size szHeaderSize;
	szHeaderSize.Width = sz.Width;
	szHeaderSize.Height = 14;
	fres = _pnlPanelHeader->SetSize(szHeaderSize);
	ENSURESUCCESS(fres);

	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	sz.Width = 78;
	sz.Height =80;
	pos.Top = 16;
	pos.Left = 42;
	tff->CurrentWrap = TRUE;
	fres = tff->GetTextBox(pos, sz, &_txtWoundText);
	ENSURESUCCESS(fres);
	fres = _FormPanel->AppendControl(_txtWoundText);
	ENSURESUCCESS(fres);
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

fresult NewWoundForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic )
{
	fresult fres;
	fres = BaseInit(FALSE,reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult NewWoundForm::OnWound( IMenuItem* sender )
{
	fresult fres;

	MenuItemBase* mi = (MenuItemBase*)sender;

	
	TARGET trg;
	switch (mi->GetAccelerator())
	{
	case BUTTON_A:
		trg = HeadTarget;
		break;
	case BUTTON_B:
		trg = AbdomenTarget;
		break;
	case BUTTON_C:
		trg = LeftArmTarget;
		break;
	case BUTTON_X:
		trg = ChestTarget;
		break;
	case BUTTON_Y:
		trg = BackTarget;
		break;
	case BUTTON_Z:
		trg = RightArmTarget;
		break;
	case BUTTON_L:
		trg = LeftLegTarget;
		break;
	case BUTTON_R:
		trg = RightLegTarget;
		break;
	default:
		return GENERAL_ERROR;
	}

	char* text = _Logic->OnNewWound(trg);
	fres = _txtWoundText->AppendText("\n");
	fres = _txtWoundText->AppendText(text);
	ENSURESUCCESS(fres);
	fres = _txtWoundText->Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

