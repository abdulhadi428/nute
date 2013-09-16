#include "OpenSpace.h"

fresult MedStressForm::DoLayout()
{
	fresult fres;

	ubyte_t controlsCount = 1;

	Size caSz = _App->GetClientAreaSize();
	Position caPos = _App->GetClientAreaPos();

	fres = _Factories->GetPanelFactory()->GetPanel(caSz, caPos, controlsCount, CL_TRANSPARENT, &_FormPanel);
	ENSURESUCCESS(fres);

	//get stipes
	Panel* stripes = NULL;
	fres = GetStripesPanel(frmStripeThreeWide, &stripes);
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

fresult MedStressForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	_TitleText = "Медицина/";
	_SubtitleText = "Стрессы";

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MedStressForm::CreateMenu( IMenu** o_mnu )
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
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->ImgHandle = big_arrow_left;
	mis->Handler = NULL;
	mis->Empty = FALSE;

	//ItemOriginС
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

	//ItemOriginL
	mis = &mf->Settings[ItemOriginL];
	mis->ImgHandle = small_arrow_bottom;
	//TODO: почему то не влезает 10 символов
	mis->Text = "+рана";
	fres = _FormManager->GetOpenFormHandler(_App->Forms->MedChooseWoundFormName,&mis->Handler);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;


	//ItemOriginE
	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "назад";
	fres = _FormManager->GetCloseFormHandler(&mis->Handler, fsrNone);
	ENSURESUCCESS(fres);
	mis->Empty = FALSE;

	//ItemOriginR
	mis = &mf->Settings[ItemOriginR];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "состояние";
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

	*o_mnu = mnu;

	return SUCCESS;

}

fresult MedStressForm::OnMenuA( IMenuItem* mi )
{
	//TODO: stub
	return  _App->ShowYNDialogEx("SomeDLG", "Медицина/", "Херь", "Хрень", "случилась нелепая\nрозовая хрень!", "Я за хрень!");
}
