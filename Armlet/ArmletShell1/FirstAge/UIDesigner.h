// Roman
// UIDesigner отвечает за функционирование элементов GUI
#pragma once
#include "GraphicRenderer.h"
#include "TextField.h"
#include "Image.h"
#include "MenuItem.h"
#include "Panel.h"
#include "ScatteredMenu.h"
#include "Osanve.h"
#include "conf.h"
using namespace ThreeKShell;
#include "Form.h"
					
#define MENU_IMAGE_WIDTH 24
#define MENU_IMAGE_HEIGHT 24
#define MENU_VERTICAL_GAP 8

#define MENU_ITEM_LEFTSIDE_TOP_OFFSET 8
#define MENU_ITEM_RIGHTSIDE_TOP_OFFSET 30

#define MENU_ITEM_A_TOP MENU_ITEM_LEFTSIDE_TOP_OFFSET
#define	MENU_ITEM_A_LEFT 1

#define MENU_ITEM_B_TOP MENU_ITEM_A_TOP + MENU_IMAGE_HEIGHT+MENU_VERTICAL_GAP
#define	MENU_ITEM_B_LEFT 1

#define MENU_ITEM_C_TOP MENU_ITEM_B_TOP + MENU_IMAGE_HEIGHT+MENU_VERTICAL_GAP
#define	MENU_ITEM_C_LEFT 1

#define MENU_ITEM_L_TOP SCREENY - MENU_VERTICAL_GAP
#define	MENU_ITEM_L_LEFT 1

#define MENU_ITEM_X_TOP MENU_ITEM_RIGHTSIDE_TOP_OFFSET
#define	MENU_ITEM_X_LEFT SCREENX - MENU_IMAGE_WIDTH-1

#define MENU_ITEM_Y_TOP MENU_ITEM_X_TOP + MENU_IMAGE_HEIGHT+MENU_VERTICAL_GAP
#define	MENU_ITEM_Y_LEFT SCREENX - MENU_IMAGE_WIDTH-1

#define MENU_ITEM_Z_TOP MENU_ITEM_Y_TOP + MENU_IMAGE_HEIGHT+MENU_VERTICAL_GAP
#define	MENU_ITEM_Z_LEFT SCREENX - MENU_IMAGE_WIDTH-1

#define MENU_ITEM_R_TOP SCREENY - MENU_VERTICAL_GAP
#define	MENU_ITEM_R_LEFT (SCREENX/3)*2

#define MENU_ITEM_E_TOP MENU_ITEM_R_TOP
#define	MENU_ITEM_E_LEFT SCREENX/3


#define BATTERYSTATUS_WIDTH_PX 18
#define BATTERYSTATUS_HEIGHT_PX 8
#define BATTERYSTATUS_TOP_PX 0
#define BATTERYSTATUS_LEFT_PX SCREENX - BATTERYSTATUS_WIDTH_PX

#define BATTERYSTATUS_BUFF_WIDTH 3
#define BATTERYSTATUS_BUFF_HEIGHT 1

#define FORCESTATUS_WIDTH_PX 48
#define FORCESTATUS_HEIGHT_PX 8
#define FORCESTATUS_TOP_PX 0
#define FORCESTATUS_LEFT_PX SCREENX - BATTERYSTATUS_WIDTH_PX - FORCESTATUS_WIDTH_PX

#define FORCESTATUS_BUFF_WIDTH 8
#define FORCESTATUS_BUFF_HEIGHT 1

#define TIMESTATUS_WIDTH_PX 36
#define TIMESTATUS_HEIGHT_PX 8
#define TIMESTATUS_TOP_PX 0
#define TIMESTATUS_LEFT_PX 0
        
#define TIMESTATUS_BUFF_WIDTH 6
#define TIMESTATUS_BUFF_HEIGHT 1

#define USERNAME_TOP_PX  0
#define USERNAME_LEFT_PX 0
#define USERNAME_WIDTH_PX  SCREENX - FORCESTATUS_WIDTH_PX - BATTERYSTATUS_WIDTH_PX
#define USERNAME_HEIGHT_PX 8

#define USERNAME_BUFF_WIDTH 15
#define USERNAME_BUFF_HEIGHT 1


#define STATUSBAR_TOP_PX 0
#define STATUSBAR_LEFT_PX 0
#define STATUSBAR_HEIGHT_PX 16
#define STATUSBAR_WIDTH_PX SCREENX //- MENU_IMAGE_WIDTH - 4 - USERNAME_WIDTH_PX - BATTERYSTATUS_WIDTH_PX - MAINFORM_TEXT_SIGNAL_WIDTH

#define DIALOG_TOP_PX 16
#define DIALOG_LEFT_PX 0
#define DIALOG_HEIGHT_PX SCREENY - DIALOG_TOP_PX
#define DIALOG_WIDTH_PX SCREENX - 2*DIALOG_LEFT_PX

#define MAINFORM_TOP 0
#define MAINFORM_LEFT 0
#define MAINFORM_HEIGHT SCREENY
#define MAINFORM_WIDTH SCREENX


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
#define MSGBOX_CONTENT_BUFF_HEIGHT 12

#define MSGBOX_TOP 0
#define MSGBOX_LEFT 2
#define MSGBOX_HEIGHT SCREENY
#define MSGBOX_WIDTH SCREENX -4


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

#define MENU_OSANVE_L "Бой!"
#define MENU_OSANVE_E "Лист"  
#define MENU_OSANVE_R "Выбор"

#define MENU_BATTLE_L "Осанве"
#define MENU_BATTLE_E "Лист"  
#define MENU_BATTLE_R "Выбор"

#define MENU_FIGHT_X "+5"
#define MENU_FIGHT_Y "Атака!"  
#define MENU_FIGHT_Z "-5"
#define MENU_FIGHT_R "Отступ."
#define MENU_FIGHT_E "Защита"
#define MENU_FIGHT_L "Лечить"

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
			IControl* _pnlStatusBarControls[4];
				//txtUserName
				char _txtUserNameBuff[USERNAME_BUFF_WIDTH*USERNAME_BUFF_HEIGHT+1];

				//txtBatteryStatus
				char _txtBatteryStatusBuff[BATTERYSTATUS_BUFF_WIDTH*BATTERYSTATUS_BUFF_HEIGHT+1];

				//txtOsanve
				char _txtOsanveBuff[2*MSGBOX_CONTENT_BUFF_WIDTH];
				
				//txtForce
				char _txtForce[FORCESTATUS_BUFF_WIDTH*FORCESTATUS_BUFF_HEIGHT+1];

//				//_strMainFormSignal
//				char _strMainFormSignal[MAINFORM_TEXT_SIGNAL_BUFF_WIDTH*MAINFORM_TEXT_SIGNAL_BUFF_HEIGHT];

			//_txtOsanveList
			TextField _txtOsanveList; // список отображает массив OSNV
			ubyte_t osanveListPage; // страница списка
			char _bufOsanveList[MSGBOX_CONTENT_BUFF_WIDTH*MSGBOX_CONTENT_BUFF_HEIGHT*MSGBOX_CONTENT_BUFF_PAGES];

		#pragma endregion
		#pragma region _mnuMainMenu
		//_mnuMainMenu; - меню осанве
		ScatteredMenu _mnuMainMenu;
		IMenuItem* _mnuMainMenuItems[3];

			//
			MenuItem _menuFight;
			TextField _txtMenuFight;
			char _bufMenuFight[16];
			//
			MenuItem _menuSet;
			TextField _txtMenuSet;
			char _bufMenuSet[16];
			//
			MenuItem _menuList;
			TextField _txtMenuList;
			char _bufMenuList[16];

		#pragma endregion
		fresult InitMainForm();
		fresult InitStatusBar();
		fresult InitMainFormMnu();
	#pragma endregion 

	// Roman Меню Осанве ///////////////////////////////////
	#pragma region frmOsanveForm
	//frmOsanveForm
		#pragma region _pnlOsanveForm
		//_pnlOsanveForm
		Panel _pnlOsanveForm;
		IControl* _pnlOsanveFormControls[2];
		//_txtOsanveFormTitle
		TextField _txtOsanveFormTitle;
		char _txtOsanveFormTitleTextBuff[32];

		#pragma endregion
		#pragma region _mnuOsanveMenu
		//_mnuOsanveMenu
		ScatteredMenu _mnuOsanveMenu;
		IMenuItem* _mnuOsanveMenuItems[OSANVE_COUNT];
		MenuItem _miOsanve[OSANVE_COUNT];
		TextField _txtOsanveMenu[OSANVE_COUNT];
		char _bufOsanveMenu[OSANVE_COUNT][16];

		#pragma endregion
		fresult InitOsanveForm();
		fresult InitOsanveFormMenu();
	#pragma endregion

	// Roman Меню последствий ///////////////////////////////////
	#pragma region frmConsForm
	//frmConsForm
		#pragma region _pnlConsForm
		//_pnlConsForm
		Panel _pnlConsForm;
		IControl* _pnlConsFormControls[2];
		//_txtConsFormTitle
		TextField _txtConsFormTitle;
		char _txtConsFormTitleTextBuff[32];

		#pragma endregion
		#pragma region _mnuConsMenu
		//_mnuConsMenu
		ScatteredMenu _mnuConsMenu;
		IMenuItem* _mnuConsMenuItems[CONS_COUNT+1];
		MenuItem _miCons[CONS_COUNT+1];
		TextField _txtConsMenu[CONS_COUNT+1];
		char _bufConsMenu[CONS_COUNT+1][16];

		#pragma endregion
		fresult InitConsForm();
		fresult InitConsFormMenu();
	#pragma endregion
	
// Roman Меню боевого режима ///////////////////////////////////
	#pragma region frmBattle
	//frmBattle
		#pragma region _pnlBattleForm
		Panel _pnlBattleForm;

		IControl* _pnlBattleFormControls[2];

		TextField _txtBattleCap;// заголовок
		ubyte_t battleListPage; // страница списка
		char _bufBattleCap[36];

		#pragma endregion
		#pragma region _mnuBattleMenu
		//_frmBattleMenu; - меню боевого режима
		ScatteredMenu _mnuBattleMenu;
		IMenuItem* _mnuBattleMenuItems[N_BUTTONS];

			//
			MenuItem _miFighters[6];
			TextField _txtFighters[6];
			char _bufFighters[6][16];
			struct nick* fightersOnScreen[6];
			//
			MenuItem _menuOsanve;
			TextField _txtMenuOsanve;
			char _bufMenuOsanve[16];
			//
			MenuItem _menuConsSet;
			TextField _txtMenuConsSet;
			char _bufMenuConsSet[16];
			//
			MenuItem _menuBattleList;
			TextField _txtMenuBattleList;
			char _bufMenuBattleList[16];

		#pragma endregion
		fresult InitBattleForm();
//		fresult InitStatusBar();
		fresult InitBattleFormMnu();
	#pragma endregion 

// Roman Меню сражения ///////////////////////////////////
	#pragma region frmFight
	//frmFight
		#pragma region _pnlFightForm
		Panel _pnlFightForm;

		IControl* _pnlFightFormControls[2];

		TextField _txtFightField;
		char _bufFightField[MSGBOX_CONTENT_BUFF_WIDTH*MSGBOX_CONTENT_BUFF_HEIGHT];
		struct nick* currentFighter; // указатель на элемент массива FGHT

		#pragma endregion
		#pragma region _mnuFightMenu
		//_mnuFightMenu; - меню боевого режима
		ScatteredMenu _mnuFightMenu;
		IMenuItem* _mnuFightMenuItems[5];
			//
			MenuItem _menuFightUP;
			TextField _txtMenuFightUP;
			char _bufMenuFightUP[16];
			//
			MenuItem _menuFightDOWN;
			TextField _txtMenuFightDOWN;
			char _bufMenuFightDOWN[16];

			MenuItem _menuAtack;
			TextField _txtMenuAtack;
			char _bufMenuAtack[16];
			//
			MenuItem _menuBattle;
			TextField _txtMenuBattle;
			char _bufMenuBattle[16];
			//
			MenuItem _menuDef;
			TextField _txtMenuDef;
			char _bufMenuDef[16];

			MenuItem _menuHeal;
			TextField _txtMenuHeal;
			char _bufMenuHeal[16];
		#pragma endregion
		fresult InitFightForm();
//		fresult InitStatusBar();
		fresult InitFightFormMnu();
	#pragma endregion 

	/// Roman Using it for Defense mode
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
	Form* currentForm; // отображаемая форма

	Form _frmMainForm; // осанве форма
	Form _frmOsanveForm; // осанве-выбор форма
	Form _frmConsForm; // последствия-выбор форма
	Form _frmBattleForm; // боевого режима форма
	Form _frmFightForm; //  форма боя
//	Form _frmDefForm;

	// Поля статусной области
		TextField txtUserName;
		TextField txtBatteryStatus;
		TextField txtOsanve;
		TextField txtForce;

//_pnlMainForm;
	fresult SetOsanve(void); // обновить список видимых осанве-побратимов
	fresult SetForces(void); // обновить поле сил персонажа
	fresult SetBattle(void); // Заполнить список доступных бойцов
	fresult SetFightField(void); // Обновить форму боя

// сообщение
	Form _frmMsgBox;
// лог
	Form _frmLogForm;

public:
	fresult AppendLog(ubyte_t logId, const char* message);
	fresult Init();
	fresult Draw();
	fresult ShowForm(Form* form);
	fresult CloseForm();
	fresult MessageBoxShow(const char* caption, const char* text, ubyte_t pictureId);
	fresult MessageBoxClose();

	
	fresult virtual OnMainMnuFight(IMenuItem* sender);
	fresult	virtual OnMainMnuList(IMenuItem* sender);
	fresult virtual OnMainMnuSet(IMenuItem* sender);

	fresult virtual OnOsanveMnuCancel(IMenuItem* sender);
	fresult virtual OnOsanveMnuSelect(IMenuItem* sender);
	
	fresult virtual OnBattleMnuSet(IMenuItem* sender); // выбор последствия
    fresult virtual OnBattleMnuOsanve(IMenuItem* sender); // в осанве
    fresult virtual OnBattleMnuList(IMenuItem* sender);
	fresult virtual OnFighterSelect(IMenuItem* sender); // выбор противника
 
	fresult virtual OnConsMnuCancel(IMenuItem* sender);
    fresult virtual OnConsMnuSelect(IMenuItem* sender);

	fresult virtual OnFightMnuUP(IMenuItem* sender); // сильнее
	fresult virtual OnFightMnuDOWN(IMenuItem* sender); // слабее
	fresult virtual OnFightMnuAtack(IMenuItem* sender); // Атака
	fresult virtual OnFightMnuDefence(IMenuItem* sender); // защита
    fresult virtual OnFightMnuBattle(IMenuItem* sender); // к выбору противника
	fresult virtual OnFightMnuHeal(IMenuItem* sender); // лечить противника

	fresult virtual OnLogFormMnuScrollUp(IMenuItem* sender);
	fresult	virtual OnLogFormMnuScrollDown(IMenuItem* sender);
	fresult	virtual OnLogFormMnuPrev(IMenuItem* sender);	
	fresult virtual OnLogFormMnuNext(IMenuItem* sender);
	fresult virtual OnLogFormMnuBack(IMenuItem* sender);
	fresult virtual OnLogFormMnuCleanLog(IMenuItem* sender);

	fresult virtual OnMsgBoxMnuOk		 (IMenuItem* sender);
	fresult virtual OnMsgBoxMnuScrollUp  (IMenuItem* sender);
	fresult virtual OnMsgBoxMnuScrollDown(IMenuItem* sender);

	void _SystemOnButtonClick(uword_t button);
	
};
