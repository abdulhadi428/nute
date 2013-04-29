#pragma once
#include "GraphicRenderer.h"
#include "TextField.h"
#include "Image.h"
#include "MenuItem.h"
#include "Panel.h"
#include "ScatteredMenu.h"
using namespace ThreeKShell;
#include "Form.h"
#include "Icons.h"
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
#define BATTERYSTATUS_HEIGHT_PX 16

#define BATTERYSTATUS_BUFF_WIDTH 3
#define BATTERYSTATUS_BUFF_HEIGHT 2

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
#define MSGBOX_TITLE_WIDTH SCREENX - MSGBOX_TITLE_LEFT - MENU_IMAGE_WIDTH +2
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

#define WOUND_MNU_ITEM_HIEGHT 16
#define WOUND_MNU_ITEM_WIDTH  36
#define WOUND_MNU_TEXT_BUFF_HIEGHT 2
#define WOUND_MNU_TEXT_BUFF_WIDTH  6

#define WOUND_FORM_HIEGHT SCREENY
#define WOUND_FORM_WIDTH  SCREENX

#define WOUND_FORM_TITLE_TOP 0
#define WOUND_FORM_TITLE_LEFT  18+2
#define WOUND_FORM_TITLE_HIEGHT 8
#define WOUND_FORM_TITLE_WIDTH  138 +2
#define WOUND_FORM_TITLE_TEXT_BUFF_HIEGHT 2
#define WOUND_FORM_TITLE_TEXT_BUFF_WIDTH  20

#define WOUND_RESULT_TOP 16
#define WOUND_RESULT_LEFT 37
#define WOUND_RESULT_WIDTH 86
#define WOUND_RESULT_HEIGHT 88
#define WOUND_RESULT_TEXT_BUFF_HIEGHT 10
#define WOUND_RESULT_TEXT_BUFF_WIDTH  14


class UIDesigner
{

private:
	GraphicRenderer _renderer;
	
	#pragma region Initializers
	fresult InitTextField(TextField* tf, Size tfControlSize, Position tfControlPosition, TextFormat* textFormat, bool_t wrap, char* buff, Size buffSize, const char* text);
	fresult InitImage(Image* img, Size imgControlSize, Position imgControlPosition, ubyte_t pictureId);
	fresult InitMenuItem(MenuItem* mi,TextField* tf, Size tfControlSize, Position tfControlPosition, TextFormat* textFormat, char* buff, Size buffSize, const char* text, TextFormat* selTextFormat, Image* img, Size imgControlSize, Position imgControlPosition, sbyte_t image_id, Panel* pnl, int handler, ButtonState buttonAccel);
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
			IControl* _pnlStatusBarControls[2];
				//txtUserName
				char _txtUserNameBuff[USERNAME_BUFF_WIDTH*USERNAME_BUFF_HEIGHT ];
				TextFormat _txtUserNameFormat;

				//txtBatteryStatus
				char _txtBatteryStatusBuff[BATTERYSTATUS_BUFF_WIDTH*BATTERYSTATUS_BUFF_HEIGHT];
				TextFormat _txtBatteryStatusFormat;

			//txtStatus               3 pages
			char _txtStatusBuff[STATUS_BUFF_WIDTH*STATUS_BUFF_HEIGHT];
			TextFormat _txtStatusFormat;
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
		TextFormat _txtWoundFormTitleFormat;
		char _txtWoundFormTitleTextBuff[WOUND_FORM_TITLE_TEXT_BUFF_HIEGHT*WOUND_FORM_TITLE_TEXT_BUFF_WIDTH];
		//_txtWoundResult
		TextFormat _txtWoundResultFormat;
		char _txtWoundResultTextBuff[WOUND_RESULT_TEXT_BUFF_HIEGHT*WOUND_RESULT_TEXT_BUFF_WIDTH];

		#pragma endregion
		#pragma region _mnuWoundMenu
		//_mnuWoundMenu
		ScatteredMenu _mnuWoundMenu;
		TextFormat _mnuWoundFormMenuFormat;
		TextFormat _mnuWoundFormMenuFormatSelected;
		IMenuItem* _mnuWoundMenuItems[9];
			//miWoundHead
			MenuItem _miWoundHead;
				TextField _txtMnuWoundHead;
				char _txtMnuWoundHeadTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundStomach
			MenuItem _miWoundStomach;
				TextField _txtMnuWoundStomach;
				char _txtMnuWoundStomachTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundLeftArm
			MenuItem _miWoundLeftArm;
				TextField _txtMnuWoundLeftArm;
				char _txtMnuWoundLeftArmTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundRightArm
			MenuItem _miWoundRightArm;
				TextField _txtMnuWoundRightArm;
				char _txtMnuWoundRightArmTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundChest
			MenuItem _miWoundChest;
				TextField _txtMnuWoundChest;
				char _txtMnuWoundChestTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundBack
			MenuItem _miWoundBack;
				TextField _txtMnuWoundBack;
				char _txtMnuWoundBackTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundLeftLeg
			MenuItem _miWoundLeftLeg;
				TextField _txtMnuWoundLeftLeg;
				char _txtMnuWoundLeftLegTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
			//miWoundRightLeg
			MenuItem _miWoundRightLeg;
				TextField _txtMnuWoundRightLeg;
				char _txtMnuWoundRightLegTextBuff[WOUND_MNU_TEXT_BUFF_HIEGHT*WOUND_MNU_TEXT_BUFF_WIDTH];
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
			TextFormat _txtMessageBoxTitleFormat;
			//_txtMessageBoxContent
			TextField _txtMessageBoxContent;
			char _txtMessageBoxContentBuff[MSGBOX_CONTENT_BUFF_WIDTH*MSGBOX_CONTENT_BUFF_HEIGHT];
			TextFormat _txtMessageBoxContentFormat;
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

	Form* _currentForm;
	
	//Called if corresponding menuItem is not found
	fresult virtual OnButtonPressed(ButtonState button);

protected:
	Form _frmMainForm;
	//_pnlStatusBar;
		TextField txtUserName;
		TextField txtBatteryStatus;
	//_pnlMainForm;
		TextField txtStatus;
	Form _frmMsgBox;
	Form _frmWoundForm;	
	//_pnlWoundFrom
	TextField txtWoundResult;
	
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

	void _SystemOnButtonClick(uword_t button);
	void OnNetworkData();

};
