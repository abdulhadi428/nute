#pragma once

#define HEADER_HEIGHT 24


class Honor2FormBase : public FormBase
{
protected:
	Honor2App* _App;
	Honor2Logic* _Logic;

	Panel* _pnlPanelHeader;
	bool_t _ShowHeader;

	PictureBox* _pbxHeaderIcon;
	TextField* _txtHeaderTitle;
	TextField* _txtHeaderSubTitle;

	Size _FormClientSize;

	fresult BaseInit(bool_t showHeader, Repositories* reps, Factories* facts, char* name, FormManager* frmmngr, Honor2App* app, Honor2Logic* logic);
	virtual fresult OnBeforeShow( IForm* prevFrom, bool_t reActivation );

	//creates and itinializes 	_HeaderIcon, _HeaderTitle, _HeaderSubTitle;
	virtual fresult CreateHeader(ImageHandle headerIconHandle, char* headerTitleText, char* headerSubtitleText);
	virtual ubyte_t HeaderControlsCount();
	
public:


};