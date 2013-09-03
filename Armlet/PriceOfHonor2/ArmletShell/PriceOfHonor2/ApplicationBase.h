#pragma  once

#define DOVIBRO_TIME 500
#define BATTERYSTATUS_POLL_TIME 4*1000


class ApplicationBase
{
private:
	
	fresult InitRepositories(Repositories* repositories);
	fresult InitFactories(Factories* factories);

	//Repositories private static alloc. Don't use them, access via _Repositories
	ColorsRepository _colorsInstance;
	FontsRepository _fontsInstance;
	TextFormatsRepository _textFormatsInstance;
	ImagesRepository _imagesInstance;

	//Factories private static alloc. Don't use them, access via _Factories
	PanelFactory _pnlFactoryInstance;
	PictureBoxFactory _pbxFactoryInstance;
	ImageListFactory _imglFactoryInstance;
	TextFieldFactory _tfFactoryInstance;
	MenuItemFactory _miFactoryInstance;
	MenuFactory _mnuFactoryInstance;
	FormManager _fmngrFormManagerInstance;

	GraphicRenderer _renderer;

protected:
	IRender* _Render;

	//override reasonably
	virtual Repositories* AllocRepositories(); 
	virtual Factories* AllocFactories(); 
	
	char* _StartupFormName;
	Repositories* _Repositories;
	Factories* _Factories;
	FormManager* _FormManager;
	
	StatusBarBase* _StatusBar;
	char* _Version;
	
	fresult BaseInit();
	
	virtual fresult InitImages(ImagesRepository* imgrep);
	virtual fresult InitFonts(FontsRepository* fntrep);
	virtual fresult InitColors(ColorsRepository* clrrep);
	virtual fresult InitTextFormats(TextFormatsRepository* tfrep);
	virtual fresult GetFormManagerParams(ubyte_t* formsCount, ubyte_t* shownStackLength);
	virtual fresult CreateStatusBar();
	virtual fresult CreateForms();

public:
	virtual char* GetVersion();
	
	virtual Size GetDisplaySize();
	virtual Size GetStatusBarSize();
	virtual Size GetClientAreaSize();
	virtual Position GetClientAreaPos();

	virtual fresult Start();
	virtual fresult OnButtonEvent(ButtonState buttonState);
	virtual bool_t OnSystemTimer();
	
	virtual void LogError(char* errorText);

	//api access
	void DoVibroAndBeep();
};