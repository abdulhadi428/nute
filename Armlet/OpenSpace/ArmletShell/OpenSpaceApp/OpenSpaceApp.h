#pragma once

#define CL_OS_LIGHTRED	CL_SYSTEM_LAST +1
#define CL_OS_DARKRED	CL_SYSTEM_LAST +2
#define CL_OS_ORANGE	CL_SYSTEM_LAST +3
#define CL_OS_GREEN		CL_SYSTEM_LAST +4
#define CL_OS_LIGHTGRAY	CL_SYSTEM_LAST +5

#define TF_MENU_RED_EVEN TF_SYSTEM_LAST +1
#define TF_MENU_RED		 TF_SYSTEM_LAST +2
#define TF_NORMAL_EVEN	 TF_SYSTEM_LAST +3
#define TF_REDTEXT		 TF_SYSTEM_LAST +4
#define TF_REDTEXT_EVEN	 TF_SYSTEM_LAST +5
#define TF_GRAYTEXT		 TF_SYSTEM_LAST +6
#define TF_GRAYTEXT_EVEN TF_SYSTEM_LAST +7

class OpenSpaceApp : public ApplicationBase
{
	OpenSpaceStatusBar _OSStatusBarInstance;
	OpenSpaceLogic _logicInstance;

	OpenSpaceForms _openSpaceFormsInstance;


	OSTemplateForm _OSTemplateFormIstance;
	YNDialogForm _YNDialogFormInstance;

	MedStressForm _MedStressFormInstance;
	MedChooseWoundForm _MedChooseWoundFormInstance;
	MedChooseTorsoWoundForm _MedChooseTorsoWoundFormInstance;
	MedStatusForm _MedStatusFormInstance;
	
	PlayMusicForm _PlayMusicFormInstance;
	BehaviorForm _BehavoirFormInstance; 

	MainForm _mainFormInstance;

public:
	OpenSpaceStatusBar* AppStatusBar; 
	OpenSpaceLogic* Logic;
	OpenSpaceForms* Forms;

	fresult Init();

	virtual fresult InitImages( ImagesRepository* imgrep );
	virtual fresult InitColors( ColorsRepository* clrrep );
	virtual fresult InitTextFormats(ColorsRepository* clrrep, TextFormatsRepository* tfrep );
	virtual fresult GetFormManagerParams( ubyte_t* formsCount, ubyte_t* shownStackLength );
	virtual fresult CreateStatusBar();
	virtual fresult CreateForms();

	fresult ShowYNDialogEx(char* dialogName, char* formTitle, char* formSubtitle, char* msgTitle, char* msgContent, char* yesText);
};