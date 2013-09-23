#include "ThreeKShell.h"
#include "To3KShell.h"
#include "OpenSpace.h"

#define OpenSpaceApp_VERSION "0.01"

fresult OpenSpaceApp::Init()
{
	fresult fres;

	fres = BaseInit();
	ENSURESUCCESS(fres);

	_Version = OpenSpaceApp_VERSION;
	Logic = &_logicInstance;
	fres = Logic->Init();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult OpenSpaceApp::InitImages( ImagesRepository* imgrep )
{
	fresult fres;
	fres= ApplicationBase::InitImages(imgrep);
	ENSURESUCCESS(fres);

	FillAppSpecificImages(imgrep);

	return SUCCESS;
}

fresult OpenSpaceApp::InitColors( ColorsRepository* clrrep )
{
	fresult fres;
	fres = ApplicationBase::InitColors(clrrep);
	ENSURESUCCESS(fres);


/*
светло-серый, фон (0xFF,230,230,230)
темно-серый, полосы (0xFF,199,199,199)
желтый, заголовок (0xFF,255,204,0)
черный, текст, линия (0xFF,0,0,0)
серый, текст (0xFF,102,102,102)
красный, текст (0xFF,255,0,0)
темно-красный, значения стрессов (0xFF,171,0,0)
оранжевый, значения стрессов (0xFF,255,102,0)
зеленый, текст, значения стрессов (0xFF,0,153,51)
*/
	//Override colors
	//желтый, заголовок (0xFF,255,204,0)	
	fres = clrrep->Register24bitARGBColor(CL_HEADER_BACKGROUND, 0xFF,255,204,0);
	ENSURESUCCESS(fres);

	//черный, текст, линия (0xFF,0,0,0)	
	fres = clrrep->Register24bitARGBColor(CL_HEADER_TEXT_FOREGROUND, 0xFF,0,0,0);
	ENSURESUCCESS(fres);

	//черный, текст, линия (0xFF,0,0,0)	
	fres = clrrep->Register24bitARGBColor(CL_DEFAULT_TEXT_FOREGROUND, 0xFF,0,0,0);
	ENSURESUCCESS(fres);
	
	//черный, текст, линия (0xFF,0,0,0)	
	fres = clrrep->Register24bitARGBColor(CL_MENU_TEXT_FOREGROUND, 0xFF,0,0,0);
	ENSURESUCCESS(fres);
	
	//светло-серый, фон (0xFF,230,230,230)
	fres = clrrep->Register24bitARGBColor(CL_MENU_BACKGROUND, 0xFF,230,230,230);
	ENSURESUCCESS(fres);
					
	//черный, текст, линия (0xFF,0,0,0)	
	fres = clrrep->Register24bitARGBColor(CL_MENU_EVEN_TEXT_FOREGROUND, 0xFF,0,0,0);
	ENSURESUCCESS(fres);

	//темно-серый, полосы (0xFF,199,199,199)
	fres = clrrep->Register24bitARGBColor(CL_MENU_EVEN_BACKGROUND, 0xFF,199,199,199);
	ENSURESUCCESS(fres);

	//серый, текст (0xFF,102,102,102)
	fres = clrrep->Register24bitARGBColor(CL_OS_LIGHTGRAY	, 0xFF,102,102,102);
	ENSURESUCCESS(fres);

	//красный, текст (0xFF,255,0,0)
	fres = clrrep->Register24bitARGBColor(CL_OS_LIGHTRED	, 0xFF,255,0,0);
	ENSURESUCCESS(fres);	

	//темно-красный, значения стрессов (0xFF,171,0,0)
	fres = clrrep->Register24bitARGBColor(CL_OS_DARKRED	, 0xFF,171,0,0);
	ENSURESUCCESS(fres);	

	//оранжевый, значения стрессов (0xFF,255,102,0)
	fres = clrrep->Register24bitARGBColor(CL_OS_ORANGE	, 0xFF,255,102,0);
	ENSURESUCCESS(fres);	
	
	//зеленый, текст, значения стрессов (0xFF,0,153,51)
	fres = clrrep->Register24bitARGBColor(CL_OS_GREEN	, 0xFF,0,153,51);
	ENSURESUCCESS(fres);			
		
	return SUCCESS;
}

fresult OpenSpaceApp::InitTextFormats( ColorsRepository* clrrep, TextFormatsRepository* tfrep )
{
	fresult fres;
	fres = ApplicationBase::InitTextFormats(clrrep, tfrep);
	ENSURESUCCESS(fres);

	TextFormatsRepository* tfr = tfrep;
	ColorsRepository* clr = clrrep;

	ColorHandle clMenuEvenBg;
	fres = clr->GetColorById(CL_MENU_EVEN_BACKGROUND, &clMenuEvenBg);
	ENSURESUCCESS(fres);

	ColorHandle clMenuBg;
	fres = clr->GetColorById(CL_MENU_BACKGROUND, &clMenuBg);
	ENSURESUCCESS(fres);

	ColorHandle clLightRed;
	fres = clr->GetColorById(CL_OS_LIGHTRED, &clLightRed);
	ENSURESUCCESS(fres);

	ColorHandle clBlack;
	fres = clr->GetColorById(CL_BLACK, &clBlack);
	ENSURESUCCESS(fres);

	ColorHandle clLightGray;
	fres = clr->GetColorById(CL_OS_LIGHTGRAY, &clLightGray);
	ENSURESUCCESS(fres);

	TextFormat* tfm = NULL;

	fres = tfr->GetTextFormat(TF_NORMAL, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clBlack;
	tfm->BgColor = clMenuBg;

	fres = tfr->GetTextFormat(TF_NORMAL_EVEN, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clBlack;
	tfm->BgColor = clMenuEvenBg;
	
	fres = tfr->GetTextFormat(TF_MENU_RED, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clLightRed;
	tfm->BgColor = clMenuBg;

	fres = tfr->GetTextFormat(TF_MENU_RED_EVEN, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clLightRed;
	tfm->BgColor = clMenuEvenBg;

	fres = tfr->GetTextFormat(TF_REDTEXT, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clLightRed;
	tfm->BgColor = clMenuBg;
	
	fres = tfr->GetTextFormat(TF_REDTEXT_EVEN, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clLightRed;
	tfm->BgColor = clMenuEvenBg;

	fres = tfr->GetTextFormat(TF_GRAYTEXT, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clLightGray;
	tfm->BgColor = clMenuBg;

	fres = tfr->GetTextFormat(TF_GRAYTEXT_EVEN, &tfm);
	ENSURESUCCESS(fres);
	tfm->FgColor = clLightGray;
	tfm->BgColor = clMenuEvenBg;

	return SUCCESS;
}

fresult OpenSpaceApp::GetFormManagerParams( ubyte_t* formsCount, ubyte_t* shownStackLength )
{
	*formsCount =10;
	*shownStackLength = 5;
	
	return SUCCESS;
}

fresult OpenSpaceApp::CreateStatusBar()
{
	fresult fres;
	Size statusBarSize;
	Size clSz = GetClientAreaSize();

	statusBarSize.Height = 16;
	statusBarSize.Width = clSz.Width;

	fres = _OSStatusBarInstance.Init(statusBarSize, _Repositories, _Factories);
	ENSURESUCCESS(fres);
	
	_StatusBar = &_OSStatusBarInstance;
	AppStatusBar = &_OSStatusBarInstance;

	return SUCCESS;
}

fresult OpenSpaceApp::CreateForms()
{
	fresult fres;

	fres = _openSpaceFormsInstance.Init();
	ENSURESUCCESS(fres);

	Forms = &_openSpaceFormsInstance;

	//TestForm
	//fres = _OSTemplateFormIstance.Init(_Repositories, _Factories, Forms->OSTemplateFormName, _FormManager, this, Logic);
	//ENSURESUCCESS(fres);
	//fres = _FormManager->RegisterForm(&_OSTemplateFormIstance);
	//ENSURESUCCESS(fres);
	//_StartupFormName = Forms->OSTemplateFormName;

	//YNDialogForm
	fres = _YNDialogFormInstance.Init(_Repositories, _Factories, Forms->YNDialogFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_YNDialogFormInstance);
	ENSURESUCCESS(fres);

	//MedStatusFormName
	fres = _MedStatusFormInstance.Init(_Repositories, _Factories, Forms->MedStatusFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_MedStatusFormInstance);
	ENSURESUCCESS(fres);


	//MedChooseTorsoWoundForm
	fres = _MedChooseTorsoWoundFormInstance.Init(_Repositories, _Factories, Forms->MedChooseTorsoWoundFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_MedChooseTorsoWoundFormInstance);
	ENSURESUCCESS(fres);

	//MedChooseWoundForm
	fres = _MedChooseWoundFormInstance.Init(_Repositories, _Factories, Forms->MedChooseWoundFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_MedChooseWoundFormInstance);
	ENSURESUCCESS(fres);
	
	//med form
	fres = _MedStressFormInstance.Init(_Repositories, _Factories, Forms->MedStressFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_MedStressFormInstance);
	ENSURESUCCESS(fres);

	
	//PlayMusicForm form
	fres = _PlayMusicFormInstance.Init(_Repositories, _Factories, Forms->PlayMusicFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_PlayMusicFormInstance);
	ENSURESUCCESS(fres);

	//Behavior form
	fres = _BehavoirFormInstance.Init(_Repositories, _Factories, Forms->BehavoirFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_BehavoirFormInstance);
	ENSURESUCCESS(fres);


	//main form
	fres = _mainFormInstance.Init(_Repositories, _Factories, Forms->MainFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_mainFormInstance);
	ENSURESUCCESS(fres);

	_StartupFormName = Forms->MainFormName;

	return SUCCESS;
}

fresult OpenSpaceApp::ShowYNDialogEx(char* dialogName, char* formTitle, char* formSubtitle, char* msgTitle, char* msgContent, char* yesText )
{
	fresult fres;
	YNDialogForm* frm = (YNDialogForm*)_FormManager->GetForm(Forms->YNDialogFormName);
	FAILIF(frm==NULL);

	fres = frm->ShowYNDialog(dialogName, formTitle, formSubtitle, msgTitle, msgContent, yesText);
	ENSURESUCCESS(fres);

	return SUCCESS;
}


