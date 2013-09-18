#pragma once

#define CL_OS_LIGHTRED	CL_SYSTEM_LAST +1
#define CL_OS_DARKRED	CL_SYSTEM_LAST +2
#define CL_OS_ORANGE	CL_SYSTEM_LAST +3
#define CL_OS_GREEN		CL_SYSTEM_LAST +4
#define CL_OS_LIGHTGRAY	CL_SYSTEM_LAST +5

class OpenSpaceApp : public ApplicationBase
{
	OpenSpaceStatusBar _OSStatusBarInstance;
	OpenSpaceLogic _logicInstance;

	OpenSpaceForms _openSpaceFormsInstance;

	MainForm _mainFormInstance;
	MedStressForm _MedStressFormInstance;
	YNDialogForm _YNDialogFormInstance;
	MedChooseWoundForm _MedChooseWoundFormInstance;
	MedChooseTorsoWoundForm _MedChooseTorsoWoundFormInstance;
	MedStatusForm _MedStatusFormInstance;
	OSTemplateForm _OSTemplateFormIstance;


public:
	OpenSpaceStatusBar* AppStatusBar; 
	OpenSpaceLogic* Logic;
	OpenSpaceForms* Forms;

	fresult Init();

	virtual fresult InitImages( ImagesRepository* imgrep );
	virtual fresult InitColors( ColorsRepository* clrrep );
	virtual fresult InitTextFormats( TextFormatsRepository* tfrep );
	virtual fresult GetFormManagerParams( ubyte_t* formsCount, ubyte_t* shownStackLength );
	virtual fresult CreateStatusBar();
	virtual fresult CreateForms();

	fresult ShowYNDialogEx(char* dialogName, char* formTitle, char* formSubtitle, char* msgTitle, char* msgContent, char* yesText);
};