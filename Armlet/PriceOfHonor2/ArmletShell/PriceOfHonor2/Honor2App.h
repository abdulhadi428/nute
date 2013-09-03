#pragma once

#define CL_OS_LIGHTRED	CL_SYSTEM_LAST +1
#define CL_OS_DARKRED	CL_SYSTEM_LAST +2
#define CL_OS_ORANGE	CL_SYSTEM_LAST +3
#define CL_OS_GREEN		CL_SYSTEM_LAST +4
#define CL_OS_LIGHTGRAY	CL_SYSTEM_LAST +5


class Honor2App : public ApplicationBase
{
	Honor2StatusBar _Honor2StatusBarInstance;
	Honor2Logic _logicInstance;

	Honor2Forms _honor2FormsInstance;

	Honor2MainForm _honor2MainFormInstance;
	NewWoundForm _newWoundFormInstance;
	MsgBoxForm _msgBoxFormInstance;
	LogForm _logFormInstance;

public:
	Honor2StatusBar* AppStatusBar; 
	Honor2Logic* Logic;
	Honor2Forms* Forms;

	fresult Init();

	virtual fresult InitImages( ImagesRepository* imgrep );
	virtual fresult InitColors( ColorsRepository* clrrep );
	virtual fresult InitTextFormats( TextFormatsRepository* tfrep );
	virtual fresult GetFormManagerParams( ubyte_t* formsCount, ubyte_t* shownStackLength );
	virtual fresult CreateStatusBar();
	virtual fresult CreateForms();

	fresult MsgBoxShow(ImageHandle mgsBoxIcon, char* title, char* text);
};