#include "ArmletApi.h"
#include "ArmletShell.h"
#include "ThreeKShell.h"
#include "To3KShell.h"

#define DEFAULT_SHOWNFORMSSTACK_LENGTH 10

Repositories* ApplicationBase::AllocRepositories()
{
	return new Repositories();
}

Factories* ApplicationBase::AllocFactories()
{
	return new Factories();
}

fresult ApplicationBase::InitImages( ImagesRepository* imgrep )
{
	return SUCCESS;
}

fresult ApplicationBase::InitFonts( FontsRepository* fntrep )
{
	return SUCCESS;
}

fresult ApplicationBase::InitColors( ColorsRepository* clrrep )
{
	return SUCCESS;
}

fresult ApplicationBase::InitTextFormats( TextFormatsRepository* tfrep )
{
	return SUCCESS;
}

fresult ApplicationBase::InitRepositories( Repositories* repositories )
{
	fresult fres;

	_Version = "0.0";

	fres = _colorsInstance.Init();
	ENSURESUCCESS(fres);
	fres = InitColors(&_colorsInstance);
	ENSURESUCCESS(fres);

	fres = _fontsInstance.Init();
	ENSURESUCCESS(fres);
	fres = InitFonts(&_fontsInstance);
	ENSURESUCCESS(fres);

	fres = _textFormatsInstance.Init(&_colorsInstance, &_fontsInstance);
	ENSURESUCCESS(fres);
	fres = InitTextFormats(&_textFormatsInstance);
	ENSURESUCCESS(fres);

	fres = _imagesInstance.Init();
	ENSURESUCCESS(fres);
	fres = InitImages(&_imagesInstance);
	ENSURESUCCESS(fres);

	fres = _Repositories->Init(&_colorsInstance, &_fontsInstance, &_textFormatsInstance, &_imagesInstance);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult ApplicationBase::InitFactories( Factories* factories )
{
	fresult fres;

	fres = _pnlFactoryInstance.Init(_Render, _Repositories);
	ENSURESUCCESS(fres);
	
	fres = _pbxFactoryInstance.Init(_Render, _Repositories->Images);
	ENSURESUCCESS(fres);

	fres = _imglFactoryInstance.Init(_Render, _Repositories->Images);
	ENSURESUCCESS(fres);

	fres = _tfFactoryInstance.Init(_Render, _Repositories);
	ENSURESUCCESS(fres);

	fres = _miFactoryInstance.Init(_Render, _Repositories, &_tfFactoryInstance, &_pbxFactoryInstance, &_pnlFactoryInstance);
	ENSURESUCCESS(fres);

	fres = _mnuFactoryInstance.Init(_Render, _Repositories, &_tfFactoryInstance, &_pbxFactoryInstance, &_pnlFactoryInstance, &_miFactoryInstance);	
	ENSURESUCCESS(fres);

	fres = _Factories->Init(&_pnlFactoryInstance, &_pbxFactoryInstance, &_imglFactoryInstance, &_tfFactoryInstance, &_miFactoryInstance, &_mnuFactoryInstance);
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult ApplicationBase::BaseInit()
{
	fresult fres;

	_Render = &_renderer;
	_StartupFormName = NULL;

	_Repositories = AllocRepositories();
	FAILIF(_Repositories==NULL);
	fres = InitRepositories(_Repositories);
	ENSURESUCCESS(fres);

	_Factories = AllocFactories();
	FAILIF(_Factories==NULL);
	fres = InitFactories(_Factories);
	ENSURESUCCESS(fres);

	_FormManager = &_fmngrFormManagerInstance;
	
	ubyte_t formsCount = 0;
	ubyte_t shownStackLength = DEFAULT_SHOWNFORMSSTACK_LENGTH;

	fres = GetFormManagerParams(&formsCount, &shownStackLength);
	ENSURESUCCESS(fres);

	fres = _fmngrFormManagerInstance.Init(formsCount, shownStackLength);
	ENSURESUCCESS(fres);

	fres = CreateStatusBar();
	ENSURESUCCESS(fres);

	fres = CreateForms();
	ENSURESUCCESS(fres);

	fres = _fmngrFormManagerInstance.LayoutForms();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult ApplicationBase::CreateForms()
{
	return SUCCESS;
}

fresult ApplicationBase::GetFormManagerParams( ubyte_t* formsCount, ubyte_t* shownStackLength )
{
	*formsCount = 0;
	*shownStackLength = DEFAULT_SHOWNFORMSSTACK_LENGTH;

	return SUCCESS;
}

fresult ApplicationBase::Start()
{
	fresult fres;

	//clear screen
	Position zeroPos;
	zeroPos.data = 0;
	_renderer.DrawRect(zeroPos,GetDisplaySize(),WHITE);

	if (_StartupFormName!=NULL)
	{
		fres = _FormManager->ShowForm(_StartupFormName);
		ENSURESUCCESS(fres);
	}

	if (_StatusBar!=NULL)
	{
		fres = _StatusBar->Draw();
		ENSURESUCCESS(fres);
	}

	return SUCCESS;
}

fresult ApplicationBase::OnButtonEvent( ButtonState buttonState )
{
	fresult fres;
	IForm* frm = _FormManager->GetCurrentForm();
	FAILIF(frm==NULL);
	fres =  frm->OnButtonEvent(buttonState);
	ENSURESUCCESS(fres);
	
	return SUCCESS;
}

Size ApplicationBase::GetDisplaySize()
{
	Size sz;
	sz.Width = SCREENX;
	sz.Height = SCREENY;
	return sz;
}

Size ApplicationBase::GetStatusBarSize()
{
	Size sz;
	sz.data = 0;
	if (_StatusBar!=NULL)
	{
		sz.data = _StatusBar->GetSize().data;
	}
	return sz;
}

Size ApplicationBase::GetClientAreaSize()
{
	Size sz;
	Size statusBarSize;
	Size displaySize;
	statusBarSize = GetStatusBarSize();
	displaySize = GetDisplaySize();

	sz.Width = displaySize.Width;
	sz.Height = displaySize.Height - statusBarSize.Height;

	return sz;
}

fresult ApplicationBase::CreateStatusBar()
{
	_StatusBar = NULL;
	return SUCCESS;
}

char* ApplicationBase::GetVersion()
{
	return _Version;
}

Position ApplicationBase::GetClientAreaPos()
{
	Position pos;
	Size statusBarSize;
	statusBarSize = GetStatusBarSize();

	pos.Left =0;
	pos.Top = statusBarSize.Height;

	return pos;
}
