#include "OpenSpace.h"
#include "MedMainForm.h"


fresult MedMainForm::DoLayout()
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
		_FormPanel->SetControl(stripes, 0);
	}

	//get menu
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

fresult MedMainForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	fres = BaseInit(reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MedMainForm::CreateMenu( IMenu** o_mnu )
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
	mis->ImgHandle = big_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->Text = "Hard&Soft";
	mis->ImgHandle = big_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOrigin�
	mis = &mf->Settings[ItemOriginC];
	mis->ImgHandle = big_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginX
	mis = &mf->Settings[ItemOriginX];
	mis->ImgHandle = big_arrow_right;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->ImgHandle = big_arrow_right;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginZ
	mis = &mf->Settings[ItemOriginZ];
	mis->ImgHandle = big_arrow_right;
	mis->Handler = NULL;
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

	_TitleText = NULL;
	_SubtitleText = NULL;

	*o_mnu = mnu;

	return SUCCESS;

}