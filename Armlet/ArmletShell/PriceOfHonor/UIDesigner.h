#pragma once
#include "GraphicRenderer.h"
#include "TextField.h"
#include "Image.h"
#include "MenuItem.h"
#include "Panel.h"
#include "ScatteredMenu.h"
using namespace ThreeKShell;
#include "Form.h"
//#include "MenuDelegate.h"
//#include "project.h"


#define BUTTON_HOLD_OFFSET 256

#define BUTTON_HOLD_A	BUTTON_HOLD_OFFSET  + BUTTON_A
#define BUTTON_HOLD_B	BUTTON_HOLD_OFFSET	+ BUTTON_B
#define BUTTON_HOLD_C	BUTTON_HOLD_OFFSET	+ BUTTON_C
						
#define BUTTON_HOLD_X	BUTTON_HOLD_OFFSET	+ BUTTON_X
#define BUTTON_HOLD_Y	BUTTON_HOLD_OFFSET	+ BUTTON_Y
#define BUTTON_HOLD_Z	BUTTON_HOLD_OFFSET  + BUTTON_Z
						
#define BUTTON_HOLD_L	BUTTON_HOLD_OFFSET	+ BUTTON_L
#define BUTTON_HOLD_E	BUTTON_HOLD_OFFSET	+ BUTTON_E
#define BUTTON_HOLD_R	BUTTON_HOLD_OFFSET	+ BUTTON_R
						
#define MENU_IMAGE_WIDTH 24
#define MENU_IMAGE_HEIGHT 24

#define BATTERYSTATUS_TOP_PX 0
#define BATTERYSTATUS_LEFT_PX SCREENX - MENU_IMAGE_WIDTH - BATTERYSTATUS_WIDTH_PX +4
#define BATTERYSTATUS_WIDTH_PX 24
#define BATTERYSTATUS_HEIGHT_PX 8

#define BATTERYSTATUS_BUFF_WIDTH 3
#define BATTERYSTATUS_BUFF_HEIGHT 1

#define ROOMID_TOP_PX 8
#define ROOMID_LEFT_PX SCREENX - MENU_IMAGE_WIDTH - BATTERYSTATUS_WIDTH_PX +4
#define ROOMID_WIDTH_PX 24
#define ROOMID_HEIGHT_PX 8

#define ROOMID_BUFF_WIDTH 3
#define ROOMID_BUFF_HEIGHT 1


#define USERNAME_TOP_PX  0
#define USERNAME_LEFT_PX MENU_IMAGE_WIDTH +2
#define USERNAME_WIDTH_PX  SCREENX - BATTERYSTATUS_WIDTH_PX - 2*MENU_IMAGE_WIDTH +4
#define USERNAME_HEIGHT_PX 16

#define USERNAME_BUFF_WIDTH 15
#define USERNAME_BUFF_HEIGHT 2

#define STATUSBAR_TOP_PX 0
#define STATUSBAR_LEFT_PX MENU_IMAGE_WIDTH

#define STATUSBAR_HEIGHT_PX 16
#define STATUSBAR_WIDTH_PX SCREENX - MENU_IMAGE_WIDTH*2


#define DIALOG_TOP_PX 16
#define DIALOG_LEFT_PX MENU_IMAGE_WIDTH+2
#define DIALOG_HEIGHT_PX 112
#define DIALOG_WIDTH_PX SCREENX - 2*MENU_IMAGE_WIDTH -4

#define MAINFORM_TOP 0
#define MAINFORM_LEFT 2
#define MAINFORM_HEIGHT SCREENY
#define MAINFORM_WIDTH SCREENX -4

#define STATUS_BUFF_WIDTH 18
#define STATUS_BUFF_PAGES 3
#define STATUS_BUFF_HEIGHT 13*STATUS_BUFF_PAGES

#define MSGBOX_ICON_HEIGHT 24
#define MSGBOX_ICON_WIDTH 24

#define MSGBOX_ICON_TOP 0
#define MSGBOX_ICON_LEFT 0+1

#define MSGBOX_TITLE_TOP 0
#define MSGBOX_TITLE_LEFT MSGBOX_ICON_LEFT+MSGBOX_ICON_WIDTH+1
#define MSGBOX_TITLE_WIDTH SCREENX - 2*MENU_IMAGE_WIDTH +2
#define MSGBOX_TITLE_HEIGHT 24
#define MSGBOX_TITLE_BUFF_WIDTH 18
#define MSGBOX_TITLE_BUFF_HEIGHT 3

#define MSGBOX_CONTENT_TOP MENU_IMAGE_HEIGHT
#define MSGBOX_CONTENT_LEFT 2
#define MSGBOX_CONTENT_HEIGHT SCREENY - MSGBOX_ICON_HEIGHT
#define MSGBOX_CONTENT_WIDTH SCREENX-4
#define MSGBOX_CONTENT_BUFF_WIDTH 26
#define MSGBOX_CONTENT_BUFF_PAGES 3
#define MSGBOX_CONTENT_BUFF_HEIGHT 13*STATUS_BUFF_PAGES

#define MSGBOX_TOP 0
#define MSGBOX_LEFT 2
#define MSGBOX_HEIGHT SCREENY
#define MSGBOX_WIDTH SCREENX -4

#define WOUND_MNU_ITEM_HEIGHT 16
#define WOUND_MNU_ITEM_WIDTH  36
#define WOUND_MNU_TEXT_BUFF_HEIGHT 2
#define WOUND_MNU_TEXT_BUFF_WIDTH  6

#define WOUND_FORM_HEIGHT SCREENY
#define WOUND_FORM_WIDTH  SCREENX

#define WOUND_FORM_TITLE_TOP 0
#define WOUND_FORM_TITLE_LEFT  18+2
#define WOUND_FORM_TITLE_HEIGHT 14
#define WOUND_FORM_TITLE_WIDTH  114
#define WOUND_FORM_TITLE_TEXT_BUFF_HEIGHT 2
#define WOUND_FORM_TITLE_TEXT_BUFF_WIDTH  20

#define WOUND_RESULT_TOP 16
#define WOUND_RESULT_LEFT 37
#define WOUND_RESULT_WIDTH 86
#define WOUND_RESULT_HEIGHT 88
#define WOUND_RESULT_TEXT_BUFF_HEIGHT 10
#define WOUND_RESULT_TEXT_BUFF_WIDTH  14

#define LOG_FORM_LEFT 0
#define LOG_FORM_TOP 0
#define LOG_FORM_HEIGHT SCREENY
#define LOG_FORM_WIDTH SCREENX 

#define LOG_FORM_PANEL_HEADER_LEFT 0
#define LOG_FORM_PANEL_HEADER_TOP 0
#define LOG_FORM_PANEL_HEADER_HEIGHT  24
#define LOG_FORM_PANEL_HEADER_WIDTH  SCREENX

#define LOG_FORM_ICON_LEFT 20
#define LOG_FORM_ICON_TOP 0
#define LOG_FORM_ICON_HEIGHT  MENU_IMAGE_HEIGHT
#define LOG_FORM_ICON_WIDTH  MENU_IMAGE_WIDTH


#define LOG_FORM_TEXT_TITLE_LEFT LOG_FORM_ICON_LEFT + LOG_FORM_ICON_WIDTH +2
#define LOG_FORM_TEXT_TITLE_TOP LOG_FORM_PANEL_HEADER_TOP
#define LOG_FORM_TEXT_TITLE_HEIGHT  8
#define LOG_FORM_TEXT_TITLE_WIDTH  SCREENX - LOG_FORM_ICON_WIDTH - LOG_FORM_ICON_LEFT - MENU_IMAGE_WIDTH
		
#define LOG_FORM_TEXT_TITLE_BUFF_HEIGHT 1
#define LOG_FORM_TEXT_TITLE_BUFF_WIDTH 18


#define LOG_FORM_TEXT_SUBTITLE_LEFT LOG_FORM_ICON_LEFT + LOG_FORM_ICON_WIDTH +2
#define LOG_FORM_TEXT_SUBTITLE_TOP LOG_FORM_TEXT_TITLE_HEIGHT
#define LOG_FORM_TEXT_SUBTITLE_HEIGHT  8
#define LOG_FORM_TEXT_SUBTITLE_WIDTH  SCREENX - LOG_FORM_ICON_WIDTH - LOG_FORM_ICON_LEFT - MENU_IMAGE_WIDTH
#define LOG_FORM_TEXT_SUBTITLE_BUFF_HEIGHT 1
#define LOG_FORM_TEXT_SUBTITLE_BUFF_WIDTH 18



#define LOG_FORM_PANEL_CONTENT_LEFT 0
#define LOG_FORM_PANEL_CONTENT_TOP LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_PANEL_CONTENT_HEIGHT  SCREENY -LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_PANEL_CONTENT_WIDTH  SCREENX

#define LOG_FORM_TEXT_LOG_LEFT MENU_IMAGE_WIDTH +2
#define LOG_FORM_TEXT_LOG_TOP LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_TEXT_LOG_HEIGHT  SCREENY-LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_TEXT_LOG_WIDTH  SCREENX- 2*MENU_IMAGE_WIDTH -4
#define LOG_FORM_TEXT_LOG_BUFF_PAGES 1
#define LOG_FORM_TEXT_LOG_BUFF_HEIGHT 13*LOG_FORM_TEXT_LOG_BUFF_PAGES
#define LOG_FORM_TEXT_LOG_BUFF_WIDTH 18
					  
				  
#define LOGS_COUNT 3
#define LOG_EVENT 0	  
#define LOG_MED 1	  
#define LOG_SYMPTOM 2

#define LOG_EVENT_NAME "Дневник"	  
#define LOG_MED_NAME "Медицина"	  
#define LOG_SYMPTOM_NAME "Симптомы"


class UIDesigner	 
{					 
					 
private:
	GraphicRenderer _renderer;
	
	#pragma region Initializers
	fresult InitTextField(TextField* tf, Size tfControlSize, Position tfControlPosition, ubyte_t textFormat, bool_t wrap, char* buff, Size buffSize, const char* text);
	fresult InitImage(Image* img, Size imgControlSize, Position imgControlPosition, ubyte_t pictureId);
	fresult InitMenuItem(MenuItem* mi,TextField* tf, Size tfControlSize, Position tfControlPosition, ubyte_t textFormat, char* buff, Size buffSize, const char* text, ubyte_t selTextFormat, Image* img, Size imgControlSize, Position imgControlPosition, sbyte_t image_id, Panel* pnl, int handler, ButtonState buttonAccel);
	#pragma endregion

	////////////////////////////////////////////////////////
	#pragma region frmMain
	//frmMain
		#pragma region _pnlMainForm
		//_pnlMainForm
		Panel _pnlMainForm;

		//_pnlMainForm
		IControl* _pnlMainFormControls[2];
			//_pnlStatusBar
			Panel _pnlStatusBar;
			IControl* _pnlStatusBarControls[3];
				//txtUserName
				char _txtUserNameBuff[USERNAME_BUFF_WIDTH*USERNAME_BUFF_HEIGHT ];

				//txtBatteryStatus
				char _txtBatteryStatusBuff[BATTERYSTATUS_BUFF_WIDTH*BATTERYSTATUS_BUFF_HEIGHT];

				//txtRoomId
				char _txtRoomIdBuff[ROOMID_BUFF_WIDTH*ROOMID_BUFF_HEIGHT];

				//txtMainLog               3 pages
				char _txtMainLog[STATUS_BUFF_WIDTH*STATUS_BUFF_HEIGHT];
		#pragma endregion
		#pragma region _mnuMainMenu
		//_mnuMainMenu;
		ScatteredMenu _mnuMainMenu;
		IMenuItem* _mnuMainMenuItems[8];
			//miHelp
			MenuItem _miHelp;
			Image _imgMenuHelp;
			//miScrollUp
			MenuItem _miScrollUp;
			Image _imgMenuScrollUp;
			//miScrollDown
			MenuItem _miScrollDown;
			Image _imgMenuScrollDown;
			//miCycleLeft
			MenuItem _miCycleLeft;
			Image _imgMenuCycleLeft; 
			//miNewShot
			MenuItem _miNewShot;
			Image _imgMenuNewShot;
			//miKnockOut
			MenuItem _miKnockOut;
			Image _imgMenuKnockOut;
			//miOpenLock
			MenuItem _miOpenLock;
			Image _imgMenuOpenLock;
			//miCycleRight
			MenuItem _miCycleRight;
			Image _imgMenuCycleRight;
			//miShowLogs
			MenuItem _miShowLogs;
			Image _imgMenuShowLogs;
	#pragma endregion
		fresult InitMainForm();
		fresult InitStatusBar();
		fresult InitMainFormMnu();

	#pragma endregion 

	////////////////////////////////////////////////////////
	#pragma region frmWoundForm
	//frmWoundForm
		#pragma region _pnlWoundForm
		//_pnlWoundForm
		Panel _pnlWoundForm;
		IControl* _pnlWoundFormControls[2];
		//_txtWoundFormTitle
		TextField _txtWoundFormTitle;
		char _txtWoundFormTitleTextBuff[WOUND_FORM_TITLE_TEXT_BUFF_HEIGHT*WOUND_FORM_TITLE_TEXT_BUFF_WIDTH];
		//_txtWoundResult
		char _txtWoundResultTextBuff[WOUND_RESULT_TEXT_BUFF_HEIGHT*WOUND_RESULT_TEXT_BUFF_WIDTH];

		#pragma endregion
		#pragma region _mnuWoundMenu
		//_mnuWoundMenu
		ScatteredMenu _mnuWoundMenu;
		IMenuItem* _mnuWoundMenuItems[9];
			//miWoundHead
			MenuItem _miWoundHead;
				TextField _txtMnuWoundHead;
				char _txtMnuWoundHeadTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundStomach
			MenuItem _miWoundStomach;
				TextField _txtMnuWoundStomach;
				char _txtMnuWoundStomachTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundLeftArm
			MenuItem _miWoundLeftArm;
				TextField _txtMnuWoundLeftArm;
				char _txtMnuWoundLeftArmTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundRightArm
			MenuItem _miWoundRightArm;
				TextField _txtMnuWoundRightArm;
				char _txtMnuWoundRightArmTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundChest
			MenuItem _miWoundChest;
				TextField _txtMnuWoundChest;
				char _txtMnuWoundChestTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundBack
			MenuItem _miWoundBack;
				TextField _txtMnuWoundBack;
				char _txtMnuWoundBackTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundLeftLeg
			MenuItem _miWoundLeftLeg;
				TextField _txtMnuWoundLeftLeg;
				char _txtMnuWoundLeftLegTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundRightLeg
			MenuItem _miWoundRightLeg;
				TextField _txtMnuWoundRightLeg;
				char _txtMnuWoundRightLegTextBuff[WOUND_MNU_TEXT_BUFF_HEIGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundRightLeg
			MenuItem _miWoundOk;
				Image _imgMnuWoundOk;
		#pragma endregion
		fresult InitWoundForm();
		fresult InitWoundFormMenu();
	#pragma endregion
	
	////////////////////////////////////////////////////////
	#pragma region frmMsgBox
	//frmMsgBox
		#pragma region _pnlMsgBox
		//_pnlMsgBox
		Panel _pnlMsgBox;
		IControl* _pnlMsgBoxControls[3];
			//_txtMessageBoxTitle
			TextField _txtMessageBoxTitle;
			char _txtMessageBoxTitleBuff[MSGBOX_TITLE_BUFF_WIDTH*MSGBOX_TITLE_BUFF_HEIGHT];
			//_txtMessageBoxContent
			TextField _txtMessageBoxContent;
			char _txtMessageBoxContentBuff[MSGBOX_CONTENT_BUFF_WIDTH*MSGBOX_CONTENT_BUFF_HEIGHT];
			// _imgMessageBoxIcon
			Image _imgMessageBoxIcon;
		#pragma endregion
		#pragma region _mnuMsgBoxMenu
		//_mnuMsgBoxMenu
		ScatteredMenu _mnuMsgBoxMenu;
			IMenuItem* _mnuMsgBoxMenuItems[3];
				//_miMsgBoxMnuScrollUp
				MenuItem _miMsgBoxMnuScrollUp;
				//_miMsgBoxMnuScrollDown
				MenuItem _miMsgBoxMnuScrollDown;
				//_miMsgBoxMnuOk
				MenuItem _miMsgBoxMnuOk;
					Image _imgMsgBoxMnuOk;
		#pragma endregion
		fresult InitMsgBoxForm();
	#pragma endregion

	////////////////////////////////////////////////////////
	#pragma region frmLogForm
	//frmLogForm
		#pragma region _pnlLogForm
		Panel _pnlLogForm;
		IControl* _pnlLogFormControls[2];
			//_pnlLogFormHeader
			Panel _pnlLogFormHeader;
			IControl* _pnlLogFormHeaderControls[3];
				//_txtLogFormTitle
				TextField _txtLogFormTitle;
				char _txtLogFormTitleBuff[LOG_FORM_TEXT_TITLE_BUFF_HEIGHT*LOG_FORM_TEXT_TITLE_BUFF_WIDTH];
				TextField _txtLogFormSubTitle;
				char _txtLogFormSubTitleBuff[LOG_FORM_TEXT_SUBTITLE_BUFF_HEIGHT*LOG_FORM_TEXT_SUBTITLE_BUFF_WIDTH];
				//_imgLogFormIcon
				Image _imgLogFormIcon;
			//_pnlLogFormContent
			Panel _pnlLogFormContent;
			IControl* _pnlLogFormContentControls[3];
				//_txtLogFormEventLog
				char _txtLogFormEventLogBuff[LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_BUFF_WIDTH];
				//_txtLogFormMedLog
				char _txtLogFormMedLogBuff[LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_BUFF_WIDTH];
				//_txtLogFormSymptomLog
				char _txtLogFormSymptomLogBuff[LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_BUFF_WIDTH];
		#pragma endregion
		#pragma region _mnuLogFormMenu
		//_mnuLogFormMenu
		ScatteredMenu _mnuLogFormMenu;
			IMenuItem* _mnuLogFormMenuItems[6];
			//_miLogFormMnuScrollUp
			MenuItem _miLogFormMnuScrollUp;
			Image _imgLogFormMnuScrollUp;
			//_miLogFormMnuScrollDown
			MenuItem _miLogFormMnuScrollDown;
			Image _imgLogFormMnuScrollDown;
			//_miLogFormMnuPrev
			MenuItem _miLogFormMnuPrev;
			Image _imgLogFormMnuPrev;
			//_miLogFormMnuNext
			MenuItem _miLogFormMnuNext;
			Image _imgLogFormMnuNext;
			//_miLogFormMnuClose
			MenuItem _miLogFormMnuBack;
			Image _imgLogFormMnuBack;
			//_miLogFormCleanLog
			MenuItem _miLogFormCleanLog;
			Image _imgLogFormClean;
#pragma endregion
		fresult InitLogForm();
		fresult InitLogFormMenu();

		ubyte_t _currentLog;
		TextField* _logs[LOGS_COUNT];
		char* _logNames[LOGS_COUNT];

		fresult SetActiveLog(ubyte_t logId);

#pragma endregion

		////////////////////////////////////////////////////////

	
	
	//Called if corresponding menuItem is not found
	fresult virtual OnButtonPressed(ButtonState button);

protected:
	Form* currentForm;

	Form _frmMainForm;
	//_pnlStatusBar;
		TextField txtUserName;
		TextField txtBatteryStatus;
		TextField txtRoomId;
	//_pnlMainForm;
		TextField txtMainLog;
	Form _frmMsgBox;
	Form _frmWoundForm;	
	//_pnlWoundFrom
	TextField txtWoundResult;

	Form _frmLogForm;
	//_txtLogFormEventLog
	TextField txtLogFormEventLog;
	//_txtLogFormMedLog
	TextField txtLogFormMedLog;
	//_txtLogFormSymptomLog
	TextField txtLogFormSymptomLog;
	fresult AppendLog(ubyte_t logId, const char* message);

public:
	fresult Init();
	fresult Draw();
	fresult ShowForm(Form* form);
	fresult CloseForm();
	fresult MessageBoxShow(const char* caption, const char* text, ubyte_t pictureId);
	fresult MessageBoxClose();

	fresult virtual OnMainMnuHelp(IMenuItem* sender)=0;
	fresult virtual OnMainMnuScrollUp(IMenuItem* sender)=0;
	fresult	virtual OnMainMnuScrollDown(IMenuItem* sender)=0;
	fresult virtual OnMainMnuCycleLeft(IMenuItem* sender)=0;
	fresult virtual OnMainMnuShot(IMenuItem* sender)=0;
	fresult virtual OnMainMnuKnockout(IMenuItem* sender)=0;
	fresult virtual OnMainMnuOpenLock(IMenuItem* sender)=0;
	fresult virtual OnMainMnuCycleRight(IMenuItem* sender)=0;
	fresult virtual OnMainMnuShowLogs(IMenuItem* sender);

	fresult virtual OnMsgBoxMnuOk		 (IMenuItem* sender);
	fresult virtual OnMsgBoxMnuScrollUp  (IMenuItem* sender);
	fresult virtual OnMsgBoxMnuScrollDown(IMenuItem* sender);

	fresult virtual OnWoundMnuHead(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuStomach(IMenuItem* sender)=0;
	fresult	virtual OnWoundMnuLeftArm(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuRightArm(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuChest(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuBack(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuLeftLeg(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuRightLeg(IMenuItem* sender)=0;
	fresult virtual OnWoundMnuOk(IMenuItem* sender);

	fresult virtual OnLogFormMnuScrollUp(IMenuItem* sender);
	fresult	virtual OnLogFormMnuScrollDown(IMenuItem* sender);
	fresult	virtual OnLogFormMnuPrev(IMenuItem* sender);	
	fresult virtual OnLogFormMnuNext(IMenuItem* sender);
	fresult virtual OnLogFormMnuBack(IMenuItem* sender);
	fresult virtual OnLogFormMnuCleanLog(IMenuItem* sender);
	

	void _SystemOnButtonClick(uword_t button);
	void OnNetworkData();

};
