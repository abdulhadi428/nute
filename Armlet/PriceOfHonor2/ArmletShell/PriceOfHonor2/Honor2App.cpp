#include "ThreeKShell.h"
#include "Honor2.h"

#define PriceOfHonor_VERSION "0.01"

fresult Honor2App::Init()
{
	fresult fres;

	fres = BaseInit();
	ENSURESUCCESS(fres);

	_Version = PriceOfHonor_VERSION ;
	Logic = &_logicInstance;
	fres = Logic->Init(this);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult Honor2App::InitImages( ImagesRepository* imgrep )
{
	fresult fres;
	fres= ApplicationBase::InitImages(imgrep);
	ENSURESUCCESS(fres);

	FillAppSpecificImages(imgrep);

	return SUCCESS;
}

fresult Honor2App::InitColors( ColorsRepository* clrrep )
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
	//голубой, заголовок 0xD9EF
	fres = clrrep->RegisterColor(CL_HEADER_BACKGROUND, 0xD9EF);
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
	
	//рыжий, фон (0xFF,230,230,230)
	fres = clrrep->RegisterColor(CL_MENU_BACKGROUND, 0xFF61);
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

fresult Honor2App::InitTextFormats( TextFormatsRepository* tfrep )
{
	return ApplicationBase::InitTextFormats(tfrep);
}

fresult Honor2App::GetFormManagerParams( ubyte_t* formsCount, ubyte_t* shownStackLength )
{
	*formsCount = 6;
	*shownStackLength = 5;
	
	return SUCCESS;
}

fresult Honor2App::CreateStatusBar()
{
	fresult fres;
	fres = _Honor2StatusBarInstance.Init(_Repositories, _Factories);
	ENSURESUCCESS(fres);
	
	_StatusBar = &_Honor2StatusBarInstance;
	AppStatusBar = &_Honor2StatusBarInstance;

	return SUCCESS;
}

fresult Honor2App::CreateForms()
{
	fresult fres;

	fres = _honor2FormsInstance.Init();
	ENSURESUCCESS(fres);

	Forms = &_honor2FormsInstance ;
	
	fres = _honor2MainFormInstance.Init(_Repositories, _Factories, Forms->Honor2MainFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_honor2MainFormInstance);
	ENSURESUCCESS(fres);
	
	fres = _newWoundFormInstance.Init(_Repositories, _Factories, Forms->NewWoundFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_newWoundFormInstance);
	ENSURESUCCESS(fres);

	fres = _msgBoxFormInstance.Init(_Repositories, _Factories, Forms->MsgBoxFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_msgBoxFormInstance);
	ENSURESUCCESS(fres);

	fres = _logFormInstance.Init(_Repositories, _Factories, Forms->LogFormName, _FormManager, this, Logic);
	ENSURESUCCESS(fres);
	fres = _FormManager->RegisterForm(&_logFormInstance);
	ENSURESUCCESS(fres);
	
	 _StartupFormName = Forms->Honor2MainFormName;

	return SUCCESS;
}

fresult Honor2App::MsgBoxShow( ImageHandle mgsBoxIcon, char* title, char* text )
{
	fresult fres;

	MsgBoxForm* frm = (MsgBoxForm* )_FormManager->GetForm(_honor2FormsInstance.MsgBoxFormName);
	FAILIF(frm==NULL);

	fres = frm->ShowMessage(title, mgsBoxIcon, text);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

void Honor2App::LogError( char* errorText )
{
	bool_t logged = FALSE;
	fresult fres;
	if (_FormManager != NULL)
	{
		LogForm* logFrm = (LogForm*)_FormManager->GetForm(_honor2FormsInstance.LogFormName);
		if (logFrm != NULL)
		{
			fres = logFrm->LogRecord(LogKindSystem, errorText);
			if (fres == SUCCESS)
			{
				logged = TRUE;
			}
		}
	}
	
	if (logged == FALSE)
	{
		ApplicationBase::LogError(errorText);
	}
}
