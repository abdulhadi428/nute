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


					
#define MENU_IMAGE_WIDTH 24
#define MENU_IMAGE_HEIGHT 24
#define MENU_VERICAL_GAP 8

#define MENU_ITEM_LEFTSIDE_TOP_OFFSET 8
#define MENU_ITEM_RIGHTSIDE_TOP_OFFSET 30

#define MENU_ITEM_A_TOP MENU_ITEM_LEFTSIDE_TOP_OFFSET
#define	MENU_ITEM_A_LEFT 1
#define MENU_ITEM_B_TOP MENU_ITEM_A_TOP + MENU_IMAGE_HEIGHT+MENU_VERICAL_GAP
#define	MENU_ITEM_B_LEFT 1
#define MENU_ITEM_C_TOP MENU_ITEM_B_TOP + MENU_IMAGE_HEIGHT+MENU_VERICAL_GAP
#define	MENU_ITEM_C_LEFT 1
#define MENU_ITEM_L_TOP MENU_ITEM_C_TOP + MENU_IMAGE_HEIGHT+MENU_VERICAL_GAP
#define	MENU_ITEM_L_LEFT MENU_IMAGE_WIDTH +4

#define MENU_ITEM_X_TOP MENU_ITEM_RIGHTSIDE_TOP_OFFSET
#define	MENU_ITEM_X_LEFT SCREENX - MENU_IMAGE_WIDTH-1
#define MENU_ITEM_Y_TOP MENU_ITEM_X_TOP + MENU_IMAGE_HEIGHT+MENU_VERICAL_GAP
#define	MENU_ITEM_Y_LEFT SCREENX - MENU_IMAGE_WIDTH-1
#define MENU_ITEM_Z_TOP MENU_ITEM_Y_TOP + MENU_IMAGE_HEIGHT+MENU_VERICAL_GAP
#define	MENU_ITEM_Z_LEFT SCREENX - MENU_IMAGE_WIDTH-1
#define MENU_ITEM_R_TOP MENU_ITEM_C_TOP + MENU_IMAGE_HEIGHT+MENU_VERICAL_GAP
#define	MENU_ITEM_R_LEFT SCREENX - MENU_IMAGE_WIDTH*2-4

#define BATTERYSTATUS_TOP_PX 0
#define BATTERYSTATUS_LEFT_PX USERNAME_LEFT_PX + USERNAME_WIDTH_PX
#define BATTERYSTATUS_WIDTH_PX 18
#define BATTERYSTATUS_HEIGHT_PX 8

#define BATTERYSTATUS_BUFF_WIDTH 3
#define BATTERYSTATUS_BUFF_HEIGHT 1

#define MAINFORM_TEXT_GATEID_TOP 0
#define MAINFORM_TEXT_GATEID_LEFT  USERNAME_LEFT_PX + USERNAME_WIDTH_PX +BATTERYSTATUS_WIDTH_PX
#define MAINFORM_TEXT_GATEID_HIEGHT  8
#define MAINFORM_TEXT_GATEID_WIDTH 24

#define MAINFORM_TEXT_GATEID_BUFF_WIDTH 4
#define MAINFORM_TEXT_GATEID_BUFF_HEIGHT 1
					  
#define MAINFORM_TEXT_SIGNAL_TOP	MAINFORM_TEXT_GATEID_TOP+MAINFORM_TEXT_GATEID_HIEGHT
#define MAINFORM_TEXT_SIGNAL_LEFT	USERNAME_LEFT_PX + USERNAME_WIDTH_PX + ROOMID_WIDTH_PX
#define MAINFORM_TEXT_SIGNAL_HIEGHT 8
#define MAINFORM_TEXT_SIGNAL_WIDTH	24
					  
#define MAINFORM_TEXT_SIGNAL_BUFF_WIDTH 4
#define MAINFORM_TEXT_SIGNAL_BUFF_HEIGHT 1

#define ROOMID_TOP_PX 8
#define ROOMID_LEFT_PX BATTERYSTATUS_LEFT_PX
#define ROOMID_WIDTH_PX 18
#define ROOMID_HEIGHT_PX 8

#define ROOMID_BUFF_WIDTH 3
#define ROOMID_BUFF_HEIGHT 1

#define USERNAME_TOP_PX  0
#define USERNAME_LEFT_PX 14
#define USERNAME_WIDTH_PX  SCREENX - 2*MENU_IMAGE_WIDTH - 4 - BATTERYSTATUS_WIDTH_PX
#define USERNAME_HEIGHT_PX 16

#define USERNAME_BUFF_WIDTH 15
#define USERNAME_BUFF_HEIGHT 2

#define STATUSBAR_TOP_PX 0
#define STATUSBAR_LEFT_PX 2
#define STATUSBAR_HEIGHT_PX 24
#define STATUSBAR_WIDTH_PX SCREENX //- MENU_IMAGE_WIDTH - 4 - USERNAME_WIDTH_PX - BATTERYSTATUS_WIDTH_PX - MAINFORM_TEXT_SIGNAL_WIDTH

#define DIALOG_TOP_PX 16
#define DIALOG_LEFT_PX MENU_IMAGE_WIDTH+2
#define DIALOG_HEIGHT_PX SCREENX - DIALOG_TOP_PX
#define DIALOG_WIDTH_PX SCREENX - 2*MENU_IMAGE_WIDTH -4

#define MAINFORM_TOP 0
#define MAINFORM_LEFT 2
#define MAINFORM_HEIGHT SCREENY
#define MAINFORM_WIDTH SCREENX -4

#define MAINFORM_TEXT_MEDSTATUS_BUFF_WIDTH 18
#define MAINFORM_TEXT_MEDSTATUS_BUFF_PAGES 3
#define MAINFORM_TEXT_MEDSTATUS_BUFF_HEIGHT 14

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
#define MSGBOX_CONTENT_BUFF_HEIGHT 13*MSGBOX_CONTENT_BUFF_PAGES

#define MSGBOX_TOP 0
#define MSGBOX_LEFT 2
#define MSGBOX_HEIGHT SCREENY
#define MSGBOX_WIDTH SCREENX -4

#define WOUND_MNU_ITEM_HEIGHT 16
#define WOUND_MNU_ITEM_WIDTH  36
#define WOUND_MNU_TEXT_BUFF_HEIGHT 2
#define WOUND_MNU_TEXT_BUFF_WIDTH  6

#define WOUND_MNU_RIGHTCOL_TOP_OFFSET  10
#define WOUND_MNU_BOTTOMROW_LEFT_OFFSET  14

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


#define LOG_FORM_TEXT_TITLE_LEFT LOG_FORM_ICON_LEFT + LOG_FORM_ICON_WIDTH
#define LOG_FORM_TEXT_TITLE_TOP LOG_FORM_PANEL_HEADER_TOP
#define LOG_FORM_TEXT_TITLE_HEIGHT  8
#define LOG_FORM_TEXT_TITLE_WIDTH  SCREENX - LOG_FORM_ICON_WIDTH - LOG_FORM_ICON_LEFT - MENU_IMAGE_WIDTH
		
#define LOG_FORM_TEXT_TITLE_BUFF_HEIGHT 1
#define LOG_FORM_TEXT_TITLE_BUFF_WIDTH 18


#define LOG_FORM_TEXT_SUBTITLE_LEFT LOG_FORM_ICON_LEFT + LOG_FORM_ICON_WIDTH
#define LOG_FORM_TEXT_SUBTITLE_TOP LOG_FORM_TEXT_TITLE_HEIGHT
#define LOG_FORM_TEXT_SUBTITLE_HEIGHT  16
#define LOG_FORM_TEXT_SUBTITLE_WIDTH  SCREENX - LOG_FORM_ICON_WIDTH - LOG_FORM_ICON_LEFT - MENU_IMAGE_WIDTH
#define LOG_FORM_TEXT_SUBTITLE_BUFF_HEIGHT 1
#define LOG_FORM_TEXT_SUBTITLE_BUFF_WIDTH 18

#define LOG_FORM_PANEL_CONTENT_LEFT 0
#define LOG_FORM_PANEL_CONTENT_TOP LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_PANEL_CONTENT_HEIGHT  SCREENY -LOG_FORM_PANEL_HEADER_HEIGHT - MENU_IMAGE_HEIGHT
#define LOG_FORM_PANEL_CONTENT_WIDTH  SCREENX

#define LOG_FORM_TEXT_LOG_LEFT MENU_IMAGE_WIDTH +2
#define LOG_FORM_TEXT_LOG_TOP LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_TEXT_LOG_HEIGHT  SCREENY-LOG_FORM_PANEL_HEADER_HEIGHT
#define LOG_FORM_TEXT_LOG_WIDTH  SCREENX- 2*MENU_IMAGE_WIDTH -4

#define LOG_FORM_TEXT_LOG_SYSTEM_BUFF_PAGES 5
//TODO: bytelimit
#define LOG_FORM_TEXT_LOG_MESSAGE_BUFF_PAGES 19

#define LOG_FORM_TEXT_LOG_BUFF_HEIGHT 13
#define LOG_FORM_TEXT_LOG_BUFF_WIDTH 18
					  
				  
#define LOGS_COUNT 2
#define LOG_SYSTEM 0	  
#define LOG_MESSAGE 1	  

#define LOG_SYSTEM_NAME "Система"	  
#define LOG_MESSAGE_NAME "Сообщения"	  


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
			IControl* _pnlStatusBarControls[5];
				//txtUserName
				char _txtUserNameBuff[USERNAME_BUFF_WIDTH*USERNAME_BUFF_HEIGHT ];

				//txtBatteryStatus
				char _txtBatteryStatusBuff[BATTERYSTATUS_BUFF_WIDTH*BATTERYSTATUS_BUFF_HEIGHT];

				//txtRoomId
				char _txtRoomIdBuff[ROOMID_BUFF_WIDTH*ROOMID_BUFF_HEIGHT];
				
				//_strMainFormGateId
				char _strMainFormGateId[MAINFORM_TEXT_GATEID_BUFF_WIDTH*MAINFORM_TEXT_GATEID_BUFF_HEIGHT];

				//_strMainFormSignal
				char _strMainFormSignal[MAINFORM_TEXT_SIGNAL_BUFF_WIDTH*MAINFORM_TEXT_SIGNAL_BUFF_HEIGHT];

			//_txtMainFormMedStatus
			TextField _txtMainFormMedStatus;
			char _strMainFormMedStatusBuff[MAINFORM_TEXT_MEDSTATUS_BUFF_WIDTH*MAINFORM_TEXT_MEDSTATUS_BUFF_HEIGHT*MAINFORM_TEXT_MEDSTATUS_BUFF_PAGES];

		#pragma endregion
		#pragma region _mnuMainMenu
		//_mnuMainMenu;
		ScatteredMenu _mnuMainMenu;
		IMenuItem* _mnuMainMenuItems[5];

			//_miScrollUp
			MenuItem _miScrollUp;
			Image _imgMenuScrollUp;
			//_miScrollDown
			MenuItem _miScrollDown;
			Image _imgMenuScrollDown;
			//_miMedStatus
			MenuItem _miMedStatus;
			Image _imgMainFormMenuMedStatus;
			//_miNewShot
			MenuItem _miNewShot;
			Image _imgMenuNewShot;
			//_miShowLogs
			MenuItem _miShowLogs;
			Image _imgMenuShowLogs;
			//_miKnockOut
			MenuItem _miKnockOut;
			Image _imgMenuKnockOut;
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
			IControl* _pnlLogFormContentControls[2];
				//_txtLogFormSystemLog
				TextField _txtLogFormSystemLog;
				char _txtLogFormSystemLogBuff[LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_BUFF_WIDTH*LOG_FORM_TEXT_LOG_SYSTEM_BUFF_PAGES];
				//_txtLogFormMessageLog
				TextField _txtLogFormMessageLog;
				char _txtLogFormMessageLogBuff[LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_BUFF_WIDTH*LOG_FORM_TEXT_LOG_MESSAGE_BUFF_PAGES];
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
		TextField txtMainFormGateId;
		TextField txtMainFormSignal;
	//_pnlMainForm;
		fresult SetMedStatus(const char* text);


	Form _frmMsgBox;
	Form _frmWoundForm;	
	//_pnlWoundFrom
	TextField txtWoundResult;

	Form _frmLogForm;

public:
	fresult AppendLog(ubyte_t logId, const char* message);
	fresult Init();
	fresult Draw();
	fresult ShowForm(Form* form);
	fresult CloseForm();
	fresult MessageBoxShow(const char* caption, const char* text, ubyte_t pictureId);
	fresult MessageBoxClose();

	
	fresult virtual OnMainMnuScrollUp(IMenuItem* sender);
	fresult	virtual OnMainMnuScrollDown(IMenuItem* sender);
	fresult virtual OnMainMnuShot(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnMainMnuKnockout(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnMainMnuShowLogs(IMenuItem* sender);
	fresult virtual OnMsgBoxMnuOk		 (IMenuItem* sender);
	fresult virtual OnMsgBoxMnuScrollUp  (IMenuItem* sender);
	fresult virtual OnMsgBoxMnuScrollDown(IMenuItem* sender);

	fresult virtual OnWoundMnuHead(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuStomach(IMenuItem* sender){return GENERAL_ERROR;}
	fresult	virtual OnWoundMnuLeftArm(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuRightArm(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuChest(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuBack(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuLeftLeg(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuRightLeg(IMenuItem* sender){return GENERAL_ERROR;}
	fresult virtual OnWoundMnuOk(IMenuItem* sender);

	fresult virtual OnLogFormMnuScrollUp(IMenuItem* sender);
	fresult	virtual OnLogFormMnuScrollDown(IMenuItem* sender);
	fresult	virtual OnLogFormMnuPrev(IMenuItem* sender);	
	fresult virtual OnLogFormMnuNext(IMenuItem* sender);
	fresult virtual OnLogFormMnuBack(IMenuItem* sender);
	fresult virtual OnLogFormMnuCleanLog(IMenuItem* sender);

	void _SystemOnButtonClick(uword_t button);
	
};
