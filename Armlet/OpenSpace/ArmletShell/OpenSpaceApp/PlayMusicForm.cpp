#include "OpenSpace.h"

#define MENU_CAPTION_HELPLESSNESS  "Слушать свою\nбеспомощность"
#define MENU_CAPTION_EGO "Слушать свое\nэго"
#define MENU_CAPTION_CRUELTY "Слушать свою\nжестокость"

#define AUDIO_READY_DIALOG_NAME "AudioReady"

fresult PlayMusicForm::Init( Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, OpenSpaceApp* app, OpenSpaceLogic* logic )
{
	fresult fres;
	_TitleText = "Поведение/";
	_SubtitleText = "Музыка";

	_playingStress = StressNone;
	_triedPlayStress = StressNone;
	_audioReady = FALSE;

	fres = BaseInit(sbdmTitles, reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::DoLayout()
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


fresult PlayMusicForm::CreateMenu( IMenu** o_mnu )
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
	mis->ImgHandle = small_arrow_left;
	mis->Text = MENU_CAPTION_HELPLESSNESS;
	mis->OverrideTextFormat = TF_MENUEVEN;
	mis->ScreenOrigin.Top +=8;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginY
	mis = &mf->Settings[ItemOriginY];
	mis->ImgHandle = small_arrow_right;
	mis->Text = MENU_CAPTION_EGO;
	mis->OverrideTextFormat = TF_MENU;
	mis->ScreenOrigin.Top -=8;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginC
	mis = &mf->Settings[ItemOriginC];
	mis->ImgHandle = small_arrow_left;
	mis->Text = MENU_CAPTION_CRUELTY;
	mis->OverrideTextFormat = TF_MENUEVEN;
	mis->ScreenOrigin.Top +=8;
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;


	//ItemOriginL
	mis = &mf->Settings[ItemOriginL];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "тише";
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginE
	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "назад";
	mis->Handler = _AutoDispatchMenuHandler;
	mis->Empty = FALSE;

	//ItemOriginL
	mis = &mf->Settings[ItemOriginR];
	mis->ImgHandle = small_arrow_bottom;
	mis->Text = "громче";
	mis->Handler = _AutoDispatchMenuHandler;
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
	for (ubyte_t i=0; i<mnu->GetItemsCount();i++)
	{
		MenuItem* mi = (MenuItem*)(mnu->GetItem(i));
		char* miName = mi->GetName();
		if (StringEquals(miName, "A") == TRUE)
		{
			_miHelplessness = mi;
			continue;
		}
		if (StringEquals(miName, "Y") == TRUE)
		{
			_miEgo = mi;
			continue;
		}
		if (StringEquals(miName, "C") == TRUE)
		{
			_miCruelty = mi;
			continue;
		}
		if (StringEquals(miName, "L") == TRUE)
		{
			_miVolumeDown = mi;
			continue;
		}
		if (StringEquals(miName, "R") == TRUE)
		{
			_miVolumeUp = mi;
			continue;
		}
	}

	*o_mnu = mnu;
	return SUCCESS;
}

fresult PlayMusicForm::PlayMusic( StressType stress )
{
	fresult fres;

	//Stop current
	if (_playingStress != StressNone)
	{
		fres = _App->MP3Stop();
		ENSURESUCCESS(fres);
	}

	//restore current playing caption (if any)
	fres = RestoreCaptionAfterStop();

	_playingStress = stress;

	//Set new caption
	TextField* txtHelplessness = _miHelplessness->GetTextField();
	TextField* txtEgo = _miEgo->GetTextField();
	TextField* txtCruelty = _miCruelty->GetTextField();
	TextField* txtCurrent = NULL;

	//restore menu name;
	TextFormatHandle tfh;
	TextFormat* tf = NULL;
	char* stopText = NULL;

	switch (stress)
	{
	case StressHelplessness:
		txtCurrent = txtHelplessness;
		tfh = TF_MENU_RED_EVEN;
		stopText = "стоп";
		break;
	case StressEgo:
		txtCurrent = txtEgo;
		tfh = TF_MENU_RED;
		stopText = "        стоп";
		break;
	case StressCruelty:
		txtCurrent = txtCruelty;
		tfh = TF_MENU_RED_EVEN;
		stopText = "стоп";
		break;
	default:
		txtCurrent=NULL;
	}

	if (txtCurrent!=NULL)
	{
		fres = _Repositories->TextFormats->GetTextFormat(tfh, &tf);
		ENSURESUCCESS(fres);

		txtCurrent->SetTextFormat(tf);

		fres = txtCurrent->SetText(stopText);
		ENSURESUCCESS(fres);
	}

	fres = UpdateMenu();
	ENSURESUCCESS(fres);

	//Play Music
	char* fileName = _App->Logic->getStressMP3FileName(stress);

	fres = _App->MP3PlayFile(fileName);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::StopPlayingMusic()
{
	fresult fres;

	fres = RestoreCaptionAfterStop();
	ENSURESUCCESS(fres);

	_playingStress = StressNone;

	fres = UpdateMenu();
	ENSURESUCCESS(fres);

	fres = _App->MP3Stop();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::UpdateMenu()
{
	fresult fres;

	if (_playingStress == StressNone)
	{
		fres = _miVolumeUp->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
		fres = _miVolumeUp->SetVisible(FALSE);
		ENSURESUCCESS(fres);

		fres = _miVolumeDown->SetEnabled(FALSE);
		ENSURESUCCESS(fres);
		fres = _miVolumeDown->SetVisible(FALSE);
		ENSURESUCCESS(fres);
	}
	else
	{
		fres = _miVolumeUp->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
		fres = _miVolumeUp->SetVisible(TRUE);
		ENSURESUCCESS(fres);

		fres = _miVolumeDown->SetEnabled(TRUE);
		ENSURESUCCESS(fres);
		fres = _miVolumeDown->SetVisible(TRUE);
		ENSURESUCCESS(fres);
	}

	fres = _Menu->Draw();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::OnMenuA( IMenuItem* mi )
{
	fresult fres;

	fres = HandleMenuHit(StressHelplessness);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::OnMenuC( IMenuItem* mi )
{
	fresult fres;

	fres = HandleMenuHit(StressCruelty);
	ENSURESUCCESS(fres);
	
	return SUCCESS;
}

fresult PlayMusicForm::OnMenuY( IMenuItem* mi )
{
	fresult fres;

	fres = HandleMenuHit(StressEgo);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::OnMenuL( IMenuItem* mi )
{
	return _App->MP3VolumeDown();
}

fresult PlayMusicForm::OnMenuR( IMenuItem* mi )
{
	return _App->MP3VolumeUp();
}

fresult PlayMusicForm::RestoreCaptionAfterStop()
{
	fresult fres;

	TextField* txtHelplessness = _miHelplessness->GetTextField();
	TextField* txtEgo = _miEgo->GetTextField();
	TextField* txtCruelty = _miCruelty->GetTextField();
	TextField* txtCurrent = NULL;

	//restore menu name;
	TextFormatHandle tfh;
	TextFormat* tf = NULL;
	char* miName = NULL;

	switch (_playingStress)
	{
	case StressHelplessness:
		txtCurrent = txtHelplessness;
		miName = MENU_CAPTION_HELPLESSNESS;
		tfh = TF_MENUEVEN;
		break;
	case StressEgo:
		txtCurrent = txtEgo;
		miName = MENU_CAPTION_EGO;
		tfh = TF_MENU;
		break;
	case StressCruelty:
		txtCurrent = txtCruelty;
		miName = MENU_CAPTION_CRUELTY;
		tfh = TF_MENUEVEN;
		break;
	default:
		txtCurrent=NULL;
	}

	if (txtCurrent!=NULL)
	{
		fres = _Repositories->TextFormats->GetTextFormat(tfh, &tf);
		ENSURESUCCESS(fres);

		txtCurrent->SetTextFormat(tf);

		fres = txtCurrent->SetText(miName);
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult PlayMusicForm::HandleMenuHit(StressType stress)
{
	fresult fres;

	if (_playingStress != stress)
	{
		if (_audioReady == TRUE)
		{
			fres = PlayMusic(stress);
			ENSURESUCCESS(fres);
		}
		else
		{
			_triedPlayStress = stress;
			fres = _App->ShowYNDialogEx(AUDIO_READY_DIALOG_NAME, "Поведение/", "Музыка", NULL, "\n  Убедитесь, что наушники \n    или другие динамики\nподключены к аудио-разъему\n        браслета!", "Да, все готово!");
			ENSURESUCCESS(fres);
		}
	}
	else
	{
		fres = StopPlayingMusic();
		ENSURESUCCESS(fres);
	}

	return SUCCESS;	
}

fresult PlayMusicForm::OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw )
{
	fresult fres;

	if (StringEquals(dialogName, AUDIO_READY_DIALOG_NAME))
	{
		if (results == fsrOK)
		{
			_audioReady = TRUE;

			fres = PlayMusic(_triedPlayStress);
			ENSURESUCCESS(fres);
		}
	}

	return SUCCESS;
}

fresult PlayMusicForm::OnBeforeShown( IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult )
{
	fresult fres;
	if (reActivation==FALSE)
	{
		_playingStress = StressNone;
		_triedPlayStress = StressNone;
		_audioReady = FALSE;
	}

	fres = OpenSpaceFormBase::OnBeforeShown(prevFrom, reActivation, formShowResult);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult PlayMusicForm::OnMenuE( IMenuItem* mi )
{
	fresult fres;
	if (_playingStress!=StressNone)
	{
		fres = StopPlayingMusic();
		ENSURESUCCESS(fres);
	}

	fres = CloseForm(fsrNone);
	ENSURESUCCESS(fres);

	return SUCCESS;
}
