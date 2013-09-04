#include "ThreeKShell.h"
#include "To3KShell.h"
#include "Honor2.h"
#include "LogForm.h"

fresult LogForm::CreateMenu( IMenu** o_mnu )
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
	//ItemOriginA

	//ItemOriginB
	mis = &mf->Settings[ItemOriginB];
	mis->ImgHandle = BlueArrowUp;
	mis->Handler = CREATE_MENU_HANDLER(LogForm, ScrollUp);
	mis->Empty = FALSE;

	//ItemOrigin�
	mis = &mf->Settings[ItemOriginC];
	mis->ImgHandle = BlueArrowDown;
	mis->Handler = CREATE_MENU_HANDLER(LogForm, ScrollDown);
	mis->Empty = FALSE;

	//ItemOriginL
	mis = &mf->Settings[ItemOriginL];
	mis->ImgHandle = BlueArrowLeft;
	mis->Handler = CREATE_MENU_HANDLER(LogForm, CycleLogLeft);
	mis->Empty = FALSE;

	//ItemOriginR
	mis = &mf->Settings[ItemOriginR];
	mis->ImgHandle = BlueArrowRight;
	mis->Handler = CREATE_MENU_HANDLER(LogForm, CycleLogRight);
	mis->Empty = FALSE;

	//ItemOriginE
	mis = &mf->Settings[ItemOriginE];
	mis->ImgHandle = GreenOk;
	fres = _FormManager->GetCloseFormHandler(&mis->Handler);
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

fresult LogForm::DoLayout()
{
	fresult fres;
	ubyte_t controlsCount =LogsCount+1;

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

	TextFieldFactory* tff = _Factories->GetTextFieldFactory();
	tff->CurrentTextFormatHandle = TF_NORMAL;
	tff->CurrentFrames =5;
	tff->CurrentLines = 10 ;
	tff->CurrentWrap = TRUE;

	pos.Top = 24;
	pos.Left = 25;

	sz.Height = 80;
	sz.Width = 132;

	for (ubyte_t i=0; i< LogsCount;i++)
	{
		fres = tff->GetTextBox(pos, sz, &(_logs[i].txtLogField));
		ENSURESUCCESS(fres);
		_logs[i].txtLogField->SetVisible(FALSE);
		_FormPanel->AppendControl(_logs[i].txtLogField);
		ENSURESUCCESS(fres);
	}

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

const char* _logTitle = "��� �������";
const char* _logMsgTitle = "���������";
const char* _logSymptomsTitle = "��������";
const char* _logTimeEventsTitle = "�������";
const char* _logSystemTitle = "�������";


fresult LogForm::Init(Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic )
{
	fresult fres;
	fres = BaseInit(TRUE,reps,facts, name, frmmngr, app, logic);
	ENSURESUCCESS(fres);

	_logs[LogKindMessages].icon = BlueMail;
	_logs[LogKindMessages].title = (char*)_logTitle;
	_logs[LogKindMessages].subtitle = (char*)_logMsgTitle;
	_logs[LogKindMessages].logKind = LogKindMessages;

	_logs[LogKindMedSymptoms].icon = BlueHealth;
	_logs[LogKindMedSymptoms].title = (char*)_logTitle;
	_logs[LogKindMedSymptoms].subtitle = (char*)_logSymptomsTitle;
	_logs[LogKindMedSymptoms].logKind = LogKindMedSymptoms;

	_logs[LogKindSystem].icon = BlueRss;
	_logs[LogKindSystem].title = (char*)_logTitle;
	_logs[LogKindSystem].subtitle = (char*)_logSystemTitle;
	_logs[LogKindSystem].logKind = LogKindSystem;

	_logs[LogKindEvents].icon = BlueInfo;
	_logs[LogKindEvents].title = (char*)_logTitle;
	_logs[LogKindEvents].subtitle = (char*)	_logTimeEventsTitle;
	_logs[LogKindEvents].logKind = LogKindSystem;

	_currentLog =0;

	return SUCCESS;
}

fresult LogForm::LogRecord( LogKinds log, char* text )
{
	fresult fres;
	FAILIF(! (log< LogsCount));

	if (_logs[log].txtLogField !=NULL)
	{
		fres = _logs[log].txtLogField->AppendText("\n");
		ENSURESUCCESS(fres);
		_logs[log].txtLogField->AppendText(text);
		ENSURESUCCESS(fres);
	}

	//check if form is active
	if (ArmletShell::StringEquals(_FormManager->GetCurrentForm()->GetName(), GetName()))
	{
		//if current log is active
		if (log == _currentLog)
		{
			if (_logs[log].txtLogField != NULL)
			{
				fres = _logs[log].txtLogField->Draw();
				ENSURESUCCESS(fres);
			}
		}
	}

	return SUCCESS;
}

fresult LogForm::ScrollUp( IMenuItem* Sender )
{
	fresult fres;
	if (_logs[_currentLog].txtLogField !=NULL)
	{
		fres = _logs[_currentLog].txtLogField->ScrollUp();
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}

fresult LogForm::ScrollDown( IMenuItem* Sender )
{
	fresult fres;
	if (_logs[_currentLog].txtLogField !=NULL)
	{
		fres = _logs[_currentLog].txtLogField->ScrollDown();
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}

fresult LogForm::CycleLogLeft( IMenuItem* Sender )
{
	fresult fres;
	
	if (_currentLog != -1)
	{
		ubyte_t newLog;
		if (_currentLog != 0)
		{
			newLog = _currentLog-1;
		}
		else
		{
			newLog =LogsCount-1;
		}
		fres = SwitchToLog((LogKinds)newLog);
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}

fresult LogForm::CycleLogRight( IMenuItem* Sender )
{
	fresult fres;
	if (_currentLog != -1)
	{
		ubyte_t newLog;
		if (_currentLog != LogsCount-1)
		{
			newLog = _currentLog+1;
		}
		else
		{
			newLog = 0;
		}
		fres = SwitchToLog((LogKinds)newLog);
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}

fresult LogForm::SwitchToLog( LogKinds log )
{
	fresult fres;

	FAILIF(log < LogKindMessages || log >= LogsCount);

	//check if form is active
	BitmapImage* bmp = _Repositories->Images->GetImageById(_logs[log].icon);
		fres = _pbxHeaderIcon->SetImage(bmp);
	ENSURESUCCESS(fres);

	fres = _txtHeaderTitle->SetText(_logs[log].title);
	ENSURESUCCESS(fres);

	fres = _txtHeaderSubTitle->SetText(_logs[log].subtitle);
	ENSURESUCCESS(fres);

	for (ubyte_t i=0;i< LogsCount;i++)
	{
		TextField* tf = _logs[i].txtLogField;
		if (tf !=NULL)
		{
			if (i==_currentLog)
			{
				tf->SetVisible(TRUE);
			}
			else
			{
				tf->SetVisible(FALSE);
			}
		}
	}

	if (StringEquals(_FormManager->GetCurrentForm()->GetName(), GetName()))
	{
		fres = Draw();
		ENSURESUCCESS(fres);
	}

	_currentLog = log;
	return SUCCESS;
}

fresult LogForm::OnBeforeShow( IForm* prevFrom, bool_t reActivation )
{
	fresult fres;
	fres = Honor2FormBase::OnBeforeShow(prevFrom, reActivation);
	ENSURESUCCESS(fres);

	fres = SwitchToLog(LogKindMessages);

	return SUCCESS;
}