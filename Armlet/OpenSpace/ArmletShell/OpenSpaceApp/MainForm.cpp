#include "ThreeKShell.h"
#include "To3KShell.h"
#include "OpenSpace.h"
#include "MainForm.h"

fresult MainForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	fres = BaseInit(sbdmClock, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MainForm::DoLayout()
{
	ubyte_t controlsCount = 2;
	
	fresult fres;

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

	//get statusbar
	Panel* status = NULL;
	fres = CreateInfoBar(&status);
	ENSURESUCCESS(fres);
	if (status!=NULL)
	{
		fres = _FormPanel->AppendControl(status);
		ENSURESUCCESS(fres);
	}

	fres = CreateMenu(&_Menu);
	ENSURESUCCESS(fres);
	
	return SUCCESS;
}

fresult MainForm::CreateInfoBar( Panel** o_mainStatus )
{
	fresult fres;
	Panel* pnlStatus;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	Position pnlPos;
	Size pnlSize;
	pnlSize.Height = STRIPE_HEIGHT;
	pnlSize.Width = caSz.Width;


	pnlPos.Left=0;
	pnlPos.Top = caPos.Top + 6*STRIPE_HEIGHT;

	fres = _Factories->GetPanelFactory()->GetPanel(pnlSize, pnlPos, 0, CL_MENU_BACKGROUND, &pnlStatus);
	ENSURESUCCESS(fres);

	*o_mainStatus = pnlStatus;
	return SUCCESS;
}

fresult MainForm::CreateMenu( IMenu** o_mnu )
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
	mis->Text = "Инфо";
	mis->ImgHandle = small_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->Text = "Hard&Soft";
	mis->ImgHandle = small_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginС
	mis = &mf->Settings[ItemOriginC];
	mis->Text = "Психотерапия";
	mis->ImgHandle = small_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginX
	mis = &mf->Settings[ItemOriginX];
	mis->Text = "Медицина";
	mis->ImgHandle = small_arrow_right;
	fres = _FormManager->GetOpenFormHandler(_App->Forms->MedMainFormName, &mis->Handler);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->Text = "Поведение";
	mis->ImgHandle = small_arrow_right;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->Text = "Память";
	mis->ImgHandle = small_arrow_right;
	mis->Handler = NULL;
	mis->Empty = FALSE;
	
	//create mnu
	IMenu* mnu = NULL;
	fres = mf->GetMenu(ACY, &mnu);
	ENSURESUCCESS(fres);

	//Clean up
	mf->ClearSettings();
	mf->DefaultInterlaced=mf->CurrentInterlaced;
	mf->DefaultTextFormatHandle=mf->CurrentTextFormatHandle;
	mf->DefaultEvenTextFormatHandle=mf->CurrentEvenTextFormatHandle;

	_TitleText = NULL;
	_SubtitleText = NULL;

	*o_mnu = mnu;

	return SUCCESS;
}

