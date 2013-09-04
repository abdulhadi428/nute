/*#include "ArmletShell.h"
#include "UIDesigner.h"
#include "MenuDelegate.h"
#include "OpenSpaceImages.h"
#include "ColorSchema.h"

#pragma warning(disable:4100)

//W160H128

fresult UIDesigner::InitStatusBar()
{
	//txtUserName
	Size controlSize;
	controlSize.Width = USERNAME_WIDTH_PX;
	controlSize.Height = USERNAME_HEIGHT_PX;

	Position controlPosition;
	controlPosition.Top = USERNAME_TOP_PX;
	controlPosition.Left = USERNAME_LEFT_PX;

	Size controlBuffSizeTx;
	controlBuffSizeTx.Width=USERNAME_BUFF_WIDTH;
	controlBuffSizeTx.Height=USERNAME_BUFF_HEIGHT;

	
	fresult fres;

	//txtUserName
	fres = InitTextField(&txtUserName, controlSize, controlPosition, FormatHeader, FALSE, _txtUserNameBuff, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;
	
	//txtBatteryStatus
	controlSize.Width = BATTERYSTATUS_WIDTH_PX;
	controlSize.Height = BATTERYSTATUS_HEIGHT_PX;

	controlPosition.Top = BATTERYSTATUS_TOP_PX;
	controlPosition.Left = BATTERYSTATUS_LEFT_PX;

	controlBuffSizeTx.Width=BATTERYSTATUS_BUFF_WIDTH;
	controlBuffSizeTx.Height=BATTERYSTATUS_BUFF_HEIGHT;

	fres = InitTextField(&txtBatteryStatus, controlSize, controlPosition, FormatHeader, FALSE, _txtBatteryStatusBuff, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;

	//txtRoomId
	controlSize.Width = ROOMID_WIDTH_PX;
	controlSize.Height = ROOMID_HEIGHT_PX;

	controlPosition.Top = ROOMID_TOP_PX;
	controlPosition.Left = ROOMID_LEFT_PX;

	controlBuffSizeTx.Width=ROOMID_BUFF_WIDTH;
	controlBuffSizeTx.Height=ROOMID_BUFF_HEIGHT;

	fres = InitTextField(&txtRoomId, controlSize, controlPosition, FormatHeader, FALSE, _txtRoomIdBuff, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;

	//txtMainFormGateId
	controlPosition.Top = MAINFORM_TEXT_GATEID_TOP;
	controlPosition.Left = MAINFORM_TEXT_GATEID_LEFT;

	controlSize.Height = MAINFORM_TEXT_GATEID_HIEGHT;
	controlSize.Width = MAINFORM_TEXT_GATEID_WIDTH;

	controlBuffSizeTx.Height=MAINFORM_TEXT_GATEID_BUFF_HEIGHT;
	controlBuffSizeTx.Width=MAINFORM_TEXT_GATEID_BUFF_WIDTH;

	fres = InitTextField(&txtMainFormGateId, controlSize, controlPosition, FormatHeader, FALSE, _strMainFormGateId, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;

	//txtMainFormSignal
	controlPosition.Top = MAINFORM_TEXT_SIGNAL_TOP;
	controlPosition.Left = MAINFORM_TEXT_SIGNAL_LEFT;

	controlSize.Height = MAINFORM_TEXT_SIGNAL_HIEGHT;
	controlSize.Width = MAINFORM_TEXT_SIGNAL_WIDTH;

	controlBuffSizeTx.Height=MAINFORM_TEXT_SIGNAL_BUFF_HEIGHT;
	controlBuffSizeTx.Width=MAINFORM_TEXT_SIGNAL_BUFF_WIDTH;

	fres = InitTextField(&txtMainFormSignal, controlSize, controlPosition, FormatHeader, FALSE, _strMainFormSignal, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;

	//pnlStatus
	controlPosition.Top = STATUSBAR_TOP_PX;
	controlPosition.Left = STATUSBAR_LEFT_PX;

	controlSize.Width = STATUSBAR_WIDTH_PX;
	controlSize.Height = STATUSBAR_HEIGHT_PX;

	_pnlStatusBarControls[0] = &txtUserName;
	_pnlStatusBarControls[1] = &txtBatteryStatus;
	_pnlStatusBarControls[2] = &txtRoomId;
	_pnlStatusBarControls[3] = &txtMainFormGateId;
	_pnlStatusBarControls[4] = &txtMainFormSignal;

	fres = _pnlStatusBar.Init(controlSize, controlPosition, &_renderer, _pnlStatusBarControls, 5);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

fresult UIDesigner::InitMainFormMnu()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;

	Position zeroPos;
	Size zeroSize;
	zeroPos.data =0;
	zeroSize.data =0;

	//All Items
	controlSize.Width = 24;
	controlSize.Height = 24;

	Size bitmapSize;
	bitmapSize.Height = MENU_IMAGE_HEIGHT;
	bitmapSize.Width = MENU_IMAGE_WIDTH;

	//////////[0,0]////////////////////
	//imgHelp - deprecated
	
	//////////ScrollUp////////////////////
	//imgScrollUp
	controlPosition.Top = MENU_ITEM_B_TOP;
	controlPosition.Left = MENU_ITEM_B_LEFT;
	fres = InitMenuItem(&_miScrollUp, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuScrollUp, controlSize, controlPosition, BlueArrowUp,
		NULL, OnMainMnuScrollUpHandler, BUTTON_B);
	if (fres!=SUCCESS)
		return fres;
	_miScrollUp.SetSecondAccelarator(BUTTON_HOLD_B);

	//////////imgScrollDown////////////////////
	//imgScrollDown
	controlPosition.Top = MENU_ITEM_C_TOP;
	controlPosition.Left = MENU_ITEM_C_LEFT;
	fres = InitMenuItem(&_miScrollDown, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuScrollDown, controlSize, controlPosition, BlueArrowDown,
		NULL, OnMainMnuScrollDownHandler, BUTTON_C);
	if (fres!=SUCCESS)
		return fres;
	_miScrollDown.SetSecondAccelarator(BUTTON_HOLD_C);


	//////////imgNewShot////////////////////
	//imgNewShot
	controlPosition.Top = MENU_ITEM_X_TOP;
	controlPosition.Left = MENU_ITEM_X_LEFT;
	fres = InitMenuItem(&_miNewShot, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuNewShot, controlSize, controlPosition, OrangeHealth,
		NULL, OnMainMnuShotHandler, BUTTON_X);
	if (fres!=SUCCESS)
		return fres;

	//////////imgKnockOut////////////////////
	//imgKnockOut
	controlPosition.Top = MENU_ITEM_Y_TOP;
	controlPosition.Left = MENU_ITEM_Y_LEFT;
	fres = InitMenuItem(&_miKnockOut, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuKnockOut, controlSize, controlPosition, OrangeTarget,
		NULL, OnMainMnuKnockoutHandler, BUTTON_Y);
	if (fres!=SUCCESS)
		return fres;


	//////////imgOpenLock////////////////////
	//imgOpenLock
	controlPosition.Top = 72;
	controlPosition.Left = SCREENX - MENU_IMAGE_WIDTH-1;
	fres = InitMenuItem(&_miOpenLock, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuOpenLock, controlSize, controlPosition, OrangeKey,
		NULL, OnMainMnuOpenLockHandler, BUTTON_Z);
	if (fres!=SUCCESS)
		return fres;
	

	//////////imgShowLogs////////////////////
	//miShowLogs
	controlPosition.Top = MENU_ITEM_Z_TOP;
	controlPosition.Left = MENU_ITEM_Z_LEFT;
	fres = InitMenuItem(&_miShowLogs, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuShowLogs, controlSize, controlPosition, OrangeDocument,
		NULL, OnMainMnuShowLogsHandler, BUTTON_Z);
	if (fres!=SUCCESS)
		return fres;
	_miShowLogs.SetSecondAccelarator(BUTTON_R);


	//mnuMainMenu
	_mnuMainMenuItems[0] = &_miScrollUp;
	_mnuMainMenuItems[1] = &_miScrollDown;
	_mnuMainMenuItems[2] = &_miNewShot;
	_mnuMainMenuItems[3] = &_miKnockOut;
	_mnuMainMenuItems[4] = &_miShowLogs;
	

	fres =_mnuMainMenu.Init(_mnuMainMenuItems, 5);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

fresult UIDesigner::InitMainForm()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;
	Size controlBuffSizeTx;

	//_txtMainFormMedStatus
	controlPosition.Top = DIALOG_TOP_PX;
	controlPosition.Left = DIALOG_LEFT_PX;

	controlSize.Height = DIALOG_HEIGHT_PX;
	controlSize.Width = DIALOG_WIDTH_PX;

	controlBuffSizeTx.Width=MAINFORM_TEXT_MEDSTATUS_BUFF_WIDTH;
	controlBuffSizeTx.Height=MAINFORM_TEXT_MEDSTATUS_BUFF_HEIGHT*MAINFORM_TEXT_MEDSTATUS_BUFF_PAGES;

	fres = InitTextField(&_txtMainFormMedStatus, controlSize, controlPosition, FormatText, TRUE, _strMainFormMedStatusBuff, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;

	//_pnlMainForm
	controlPosition.Top = MAINFORM_TOP;
	controlPosition.Left = MAINFORM_LEFT;

	controlSize.Height = MAINFORM_HEIGHT;
	controlSize.Width = MAINFORM_WIDTH;

	fres = InitStatusBar();
	if (fres !=SUCCESS)
		return fres;

	_pnlMainFormControls[0] = &_txtMainFormMedStatus;
	_pnlMainFormControls[1] = &_pnlStatusBar;

	fres = _pnlMainForm.Init(controlSize, controlPosition, &_renderer, _pnlMainFormControls, 2);
	if (fres != SUCCESS)
		return fres;

	fres = InitMainFormMnu();
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

fresult UIDesigner::InitMsgBoxForm()
{
	fresult fres;
	
	Position controlPosition;
	Size controlSize;
	Size controlBuffSizeTx;

	Position zeroPos;
	Size zeroSize;
	zeroPos.data =0;
	zeroSize.data =0;

	//_imgMessageBoxIcon
	controlPosition.Top  = MSGBOX_ICON_TOP;
	controlPosition.Left = MSGBOX_ICON_LEFT;

	controlSize.Height  = MSGBOX_ICON_HEIGHT;
	controlSize.Width = MSGBOX_ICON_WIDTH;

	fres = InitImage(&_imgMessageBoxIcon, controlSize, controlPosition, NO_IMAGE);
	if (fres != SUCCESS)
		return fres;

	//_txtMessageBoxTitle
	controlPosition.Top  = MSGBOX_TITLE_TOP;
	controlPosition.Left = MSGBOX_TITLE_LEFT;

	controlSize.Height  = MSGBOX_TITLE_HEIGHT;
	controlSize.Width = MSGBOX_TITLE_WIDTH;

	controlBuffSizeTx.Height=MSGBOX_TITLE_BUFF_HEIGHT;
	controlBuffSizeTx.Width=MSGBOX_TITLE_BUFF_WIDTH;

	fres = InitTextField(&_txtMessageBoxTitle, controlSize, controlPosition, FormatHeader, FALSE, _txtMessageBoxTitleBuff, controlBuffSizeTx, NULL );
	if (fres != SUCCESS)
		return fres;

	//_txtMessageBoxContent
	controlPosition.Top  = MSGBOX_CONTENT_TOP;
	controlPosition.Left = MSGBOX_CONTENT_LEFT;

	controlSize.Height  = MSGBOX_CONTENT_HEIGHT;
	controlSize.Width = MSGBOX_CONTENT_WIDTH;

	controlBuffSizeTx.Height=MSGBOX_CONTENT_BUFF_HEIGHT;
	controlBuffSizeTx.Width=MSGBOX_CONTENT_BUFF_WIDTH;

	fres = InitTextField(&_txtMessageBoxContent, controlSize, controlPosition, FormatText, TRUE, _txtMessageBoxContentBuff, controlBuffSizeTx, NULL );
	if (fres != SUCCESS)
		return fres;

	//_pnlMsgBox
	controlPosition.Top  = MSGBOX_TOP;
	controlPosition.Left = MSGBOX_LEFT;

	controlSize.Height  = MSGBOX_HEIGHT;
	controlSize.Width = MSGBOX_WIDTH;

	_pnlMsgBoxControls[0] = &_imgMessageBoxIcon;
	_pnlMsgBoxControls[1] = &_txtMessageBoxTitle;
	_pnlMsgBoxControls[2] = &_txtMessageBoxContent;
	fres = _pnlMsgBox.Init(controlSize, controlPosition, &_renderer, _pnlMsgBoxControls,3);
		if (fres != SUCCESS)
			return fres;

	//_miMsgBoxMnuOk
	controlPosition.Top  = 0;
	controlPosition.Left = SCREENX-MENU_IMAGE_WIDTH -2;

	controlSize.Height  = MENU_IMAGE_HEIGHT;
	controlSize.Width = MENU_IMAGE_WIDTH;
	fres = InitMenuItem(&_miMsgBoxMnuOk, 
							NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
							&_imgMsgBoxMnuOk, controlSize, controlPosition, GreenOk,
							NULL, OnMsgBoxMnuOkHandler, BUTTON_X);
	if (fres != SUCCESS)
		return fres;
	_miMsgBoxMnuOk.SetSecondAccelarator(BUTTON_E);

	//_miMsgBoxScrollUp
	fres = InitMenuItem(&_miMsgBoxMnuScrollUp, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		NULL, zeroSize, zeroPos, NO_IMAGE,
		NULL, OnMsgBoxMnuScrollUpHandler, BUTTON_B);
	
	if (fres != SUCCESS)
		return fres;
	_miMsgBoxMnuScrollUp.SetSecondAccelarator(BUTTON_HOLD_B);

	//_miMsgBoxScrollUp
	fres = InitMenuItem(&_miMsgBoxMnuScrollDown, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		NULL, zeroSize, zeroPos, NO_IMAGE,
		NULL, OnMsgBoxMnuScrollDownHandler, BUTTON_C);
	if (fres != SUCCESS)
		return fres;
	_miMsgBoxMnuScrollDown.SetSecondAccelarator(BUTTON_HOLD_C);

	//_mnuMsgBoxMenu
	_mnuMsgBoxMenuItems[0] = &_miMsgBoxMnuOk;
	_mnuMsgBoxMenuItems[1] = &_miMsgBoxMnuScrollUp;
	_mnuMsgBoxMenuItems[2] = &_miMsgBoxMnuScrollDown;
	fres = _mnuMsgBoxMenu.Init(_mnuMsgBoxMenuItems, 3);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}



fresult UIDesigner::InitWoundFormMenu()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	Size zeroSize;
	Position zeroPosition;

	zeroSize.data =0;
	zeroPosition.data =0;

	controlSize.Height = WOUND_MNU_ITEM_HEIGHT;
	controlSize.Width = WOUND_MNU_ITEM_WIDTH;
	controlBuffSizeTx.Height=WOUND_MNU_TEXT_BUFF_HEIGHT;
	controlBuffSizeTx.Width=WOUND_MNU_TEXT_BUFF_WIDTH;

	//miWoundHead
	controlPosition.Top = 16;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miWoundHead, &_txtMnuWoundHead, controlSize, controlPosition, FormatMenu, _txtMnuWoundHeadTextBuff, controlBuffSizeTx, "Голова", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuHeadHandler, BUTTON_A);
	if (fres != SUCCESS)
		return fres;

	//miWoundStomach
	controlPosition.Top = 48;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miWoundStomach, &_txtMnuWoundStomach, controlSize, controlPosition, FormatMenu, _txtMnuWoundStomachTextBuff, controlBuffSizeTx, "Живот", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuStomachHandler, BUTTON_B);
	if (fres != SUCCESS)
		return fres;

	//miWoundLeftArm
	controlPosition.Top = 80;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miWoundLeftArm, &_txtMnuWoundLeftArm, controlSize, controlPosition, FormatMenu, _txtMnuWoundLeftArmTextBuff, controlBuffSizeTx, "Левая\n рука", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuLeftArmHandler, BUTTON_C);
	if (fres != SUCCESS)
		return fres;

	//_miWoundLeftLeg
	controlPosition.Top = 112;
	controlPosition.Left = WOUND_MNU_BOTTOMROW_LEFT_OFFSET+1;
	fres = InitMenuItem(&_miWoundLeftLeg, &_txtMnuWoundLeftLeg, controlSize, controlPosition, FormatMenu, _txtMnuWoundLeftLegTextBuff, controlBuffSizeTx, "Левая\n  нога", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuLeftLegHandler, BUTTON_L);
	if (fres != SUCCESS)
		return fres;
	
	//miWoundChest
	controlPosition.Top = 16 + WOUND_MNU_RIGHTCOL_TOP_OFFSET;
	controlPosition.Left = 120+3;
	fres = InitMenuItem(&_miWoundChest, &_txtMnuWoundChest, controlSize, controlPosition, FormatMenu, _txtMnuWoundChestTextBuff, controlBuffSizeTx, " Грудь", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuChestHandler, BUTTON_X);
	if (fres != SUCCESS)
		return fres;

	//miWoundBack
	controlPosition.Top = 48 + WOUND_MNU_RIGHTCOL_TOP_OFFSET;
	controlPosition.Left = 120+3;
	fres = InitMenuItem(&_miWoundBack, &_txtMnuWoundBack, controlSize, controlPosition, FormatMenu, _txtMnuWoundBackTextBuff, controlBuffSizeTx, " Спина", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuBackHandler, BUTTON_Y);
	if (fres != SUCCESS)
		return fres;

	//_miWoundRightArm
	controlPosition.Top = 80 + WOUND_MNU_RIGHTCOL_TOP_OFFSET;
	controlPosition.Left = 120+3;
	fres = InitMenuItem(&_miWoundRightArm, &_txtMnuWoundRightArm, controlSize, controlPosition, FormatMenu, _txtMnuWoundRightArmTextBuff, controlBuffSizeTx, "Правaя\n рука", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuRightArmHandler, BUTTON_Z);
	if (fres != SUCCESS)
		return fres;

	//miWoundRightLeg
	controlPosition.Top = 112;
	controlPosition.Left = 120 - WOUND_MNU_BOTTOMROW_LEFT_OFFSET +3;
	fres = InitMenuItem(&_miWoundRightLeg, &_txtMnuWoundRightLeg, controlSize, controlPosition, FormatMenu, _txtMnuWoundRightLegTextBuff, controlBuffSizeTx, "Правая\n нога", FormatMenu,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuRightLegHandler, BUTTON_R);
	if (fres != SUCCESS)
		return fres;

	//miOk
	controlSize.Width = MENU_IMAGE_HEIGHT;
	controlSize.Width = MENU_IMAGE_WIDTH;
	controlPosition.Top = 104;
	controlPosition.Left = 68;

	fres = InitMenuItem(&_miWoundOk, NULL, zeroSize, zeroPosition, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMnuWoundOk, controlSize, controlPosition, GreenOk, NULL, OnWoundMnuOkHandler, BUTTON_E);
	if (fres != SUCCESS)
		return fres;

	_mnuWoundMenuItems[0] = &_miWoundHead;
	_mnuWoundMenuItems[1] = &_miWoundStomach;
	_mnuWoundMenuItems[2] = &_miWoundLeftArm;
	_mnuWoundMenuItems[3] = &_miWoundRightArm;
	_mnuWoundMenuItems[4] = &_miWoundChest;
	_mnuWoundMenuItems[5] = &_miWoundBack;
	_mnuWoundMenuItems[6] = &_miWoundLeftLeg;
	_mnuWoundMenuItems[7] = &_miWoundRightLeg;
	_mnuWoundMenuItems[8] = &_miWoundOk;

	fres = _mnuWoundMenu.Init(_mnuWoundMenuItems, 9);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

fresult UIDesigner::InitWoundForm()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	controlPosition.Top = WOUND_FORM_TITLE_TOP;
	controlPosition.Left = WOUND_FORM_TITLE_LEFT;

	controlSize.Height = WOUND_FORM_TITLE_HEIGHT;
	controlSize.Width = WOUND_FORM_TITLE_WIDTH;

	controlBuffSizeTx.Height=WOUND_FORM_TITLE_TEXT_BUFF_HEIGHT;
	controlBuffSizeTx.Width=WOUND_FORM_TITLE_TEXT_BUFF_WIDTH;

	fres = InitTextField(&_txtWoundFormTitle, controlSize, controlPosition, FormatHeader, FALSE, _txtWoundFormTitleTextBuff, controlBuffSizeTx, "Укажи место ранения!");
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = WOUND_RESULT_TOP;
	controlPosition.Left = WOUND_RESULT_LEFT;

	controlSize.Height = WOUND_RESULT_HEIGHT;
	controlSize.Width = WOUND_RESULT_WIDTH;

	controlBuffSizeTx.Height=WOUND_RESULT_TEXT_BUFF_HEIGHT;
	controlBuffSizeTx.Width=WOUND_RESULT_TEXT_BUFF_WIDTH;


	fres = InitTextField(&txtWoundResult, controlSize, controlPosition, FormatText, TRUE, _txtWoundResultTextBuff, controlBuffSizeTx, NULL);
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = 0;
	controlPosition.Left = 0;

	controlSize.Height = WOUND_FORM_HEIGHT;
	controlSize.Width = WOUND_FORM_WIDTH;

	_pnlWoundFormControls[0] = &_txtWoundFormTitle;
	_pnlWoundFormControls[1] = &txtWoundResult;
	fres = _pnlWoundForm.Init(controlSize, controlPosition, &_renderer, _pnlWoundFormControls,2);
	if (fres!=SUCCESS)
		return fres;

	fres = InitWoundFormMenu();
	return fres;
}

fresult UIDesigner::InitLogFormMenu()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;

	Position zeroPos;
	Size zeroSize;
	zeroPos.data =0;
	zeroSize.data =0;

	//All Items
	controlSize.Width = MENU_IMAGE_HEIGHT;
	controlSize.Height = MENU_IMAGE_HEIGHT;

	Size bitmapSize;
	bitmapSize.Height = MENU_IMAGE_HEIGHT;
	bitmapSize.Width = MENU_IMAGE_WIDTH;

	//_miLogFormMnuScrollUp
	controlPosition.Top = MENU_ITEM_B_TOP;
	controlPosition.Left = MENU_ITEM_B_LEFT;

	fres = InitMenuItem(&_miLogFormMnuScrollUp, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuScrollUp, controlSize, controlPosition, BlueArrowUp,
		NULL, OnLogFormMnuScrollUpHandler, BUTTON_B);
	if (fres!=SUCCESS)
		return fres;
	_miScrollUp.SetSecondAccelarator(BUTTON_HOLD_B);

	//_miLogFormMnuScrollDown
	controlPosition.Top = MENU_ITEM_C_TOP;
	controlPosition.Left = MENU_ITEM_C_LEFT;

	fres = InitMenuItem(&_miLogFormMnuScrollDown, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuScrollDown, controlSize, controlPosition, BlueArrowDown,
		NULL, OnLogFormMnuScrollDownHandler, BUTTON_C);
	if (fres!=SUCCESS)
		return fres;
	_miScrollDown.SetSecondAccelarator(BUTTON_HOLD_C);

	//_miLogFormMnuPrev
	controlPosition.Top = MENU_ITEM_L_TOP;
	controlPosition.Left = MENU_ITEM_L_LEFT;
	fres = InitMenuItem(&_miLogFormMnuPrev, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuPrev, controlSize, controlPosition, BlueArrowLeft,
		NULL, OnLogFormMnuPrevHandler, BUTTON_L);
	if (fres!=SUCCESS)
		return fres;

	//INVIZILBLE
	//_miLogFormCleanLog
	controlPosition.Top = MENU_ITEM_A_TOP;
	controlPosition.Left = MENU_ITEM_A_LEFT;
	fres = InitMenuItem(&_miLogFormCleanLog, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormClean, controlSize, controlPosition, RedCancel,
		NULL, OnLogFormMnuCleanLogHandler, BUTTON_HOLD_A);
	if (fres!=SUCCESS)
		return fres;
	_imgLogFormClean.SetVisible(FALSE);

	//_miLogFormMnuBack
	controlPosition.Top = MENU_ITEM_Y_TOP;
	controlPosition.Left = MENU_ITEM_Y_LEFT;
	fres = InitMenuItem(&_miLogFormMnuBack, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuBack, controlSize, controlPosition, GreenOk,
		NULL, OnLogFormMnuBackHandler, BUTTON_Y);
	if (fres!=SUCCESS)
		return fres;
	_miLogFormMnuBack.SetSecondAccelarator(BUTTON_E);

	//_miLogFormMnuNext
	controlPosition.Top = MENU_ITEM_R_TOP;
	controlPosition.Left = MENU_ITEM_R_LEFT;
	fres = InitMenuItem(&_miLogFormMnuNext, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuNext, controlSize, controlPosition, BlueArrowRight,
		NULL, OnLogFormMnuNextHandler, BUTTON_R);
	if (fres!=SUCCESS)
		return fres;
	_miLogFormMnuNext.SetSecondAccelarator(BUTTON_Z);

	//mnuMainMenu
	_mnuLogFormMenuItems[0] = &_miLogFormMnuScrollUp;
	_mnuLogFormMenuItems[1] = &_miLogFormMnuScrollDown;
	_mnuLogFormMenuItems[2] = &_miLogFormCleanLog;
	_mnuLogFormMenuItems[3] = &_miLogFormMnuPrev;
	_mnuLogFormMenuItems[4] = &_miLogFormMnuBack;
	_mnuLogFormMenuItems[5] = &_miLogFormMnuNext;
								
	fres =_mnuLogFormMenu.Init(_mnuLogFormMenuItems, 6);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;}

fresult UIDesigner::InitLogForm()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	controlPosition.Top = LOG_FORM_TEXT_TITLE_TOP;
	controlPosition.Left = LOG_FORM_TEXT_TITLE_LEFT;

	controlSize.Height = LOG_FORM_TEXT_TITLE_HEIGHT;
	controlSize.Width =  LOG_FORM_TEXT_TITLE_WIDTH;

	controlBuffSizeTx.Height= LOG_FORM_TEXT_TITLE_BUFF_HEIGHT;
	controlBuffSizeTx.Width= LOG_FORM_TEXT_TITLE_BUFF_WIDTH;

	fres = InitTextField(&_txtLogFormTitle, controlSize, controlPosition, FormatHeader, FALSE, _txtLogFormTitleBuff, controlBuffSizeTx, "Лог событий");
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = LOG_FORM_TEXT_SUBTITLE_TOP;
	controlPosition.Left = LOG_FORM_TEXT_SUBTITLE_LEFT;

	controlSize.Height = LOG_FORM_TEXT_SUBTITLE_HEIGHT;
	controlSize.Width =  LOG_FORM_TEXT_SUBTITLE_WIDTH;

	controlBuffSizeTx.Height= LOG_FORM_TEXT_SUBTITLE_BUFF_HEIGHT;
	controlBuffSizeTx.Width= LOG_FORM_TEXT_SUBTITLE_BUFF_WIDTH;

	fres = InitTextField(&_txtLogFormSubTitle, controlSize, controlPosition, FormatHeader, FALSE, _txtLogFormSubTitleBuff, controlBuffSizeTx, NULL);
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = LOG_FORM_ICON_TOP;
	controlPosition.Left = LOG_FORM_ICON_LEFT;

	controlSize.Height = LOG_FORM_ICON_HEIGHT;
	controlSize.Width =  LOG_FORM_ICON_WIDTH;

	fres = InitImage(&_imgLogFormIcon, controlSize, controlPosition, NO_IMAGE);
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = LOG_FORM_PANEL_HEADER_TOP;
	controlPosition.Left = LOG_FORM_PANEL_HEADER_LEFT;

	controlSize.Height = LOG_FORM_PANEL_HEADER_HEIGHT;
	controlSize.Width =  LOG_FORM_PANEL_HEADER_WIDTH;

	_pnlLogFormHeaderControls[0] = &_txtLogFormTitle;
	_pnlLogFormHeaderControls[1] = &_txtLogFormSubTitle;
	_pnlLogFormHeaderControls[2] = &_imgLogFormIcon;
	fres = _pnlLogFormHeader.Init(controlSize, controlPosition, &_renderer, _pnlLogFormHeaderControls, 3);
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = LOG_FORM_TEXT_LOG_TOP;
	controlPosition.Left = LOG_FORM_TEXT_LOG_LEFT;

	controlSize.Height = LOG_FORM_TEXT_LOG_HEIGHT;
	controlSize.Width =  LOG_FORM_TEXT_LOG_WIDTH;

	controlBuffSizeTx.Height= LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_SYSTEM_BUFF_PAGES;
	controlBuffSizeTx.Width= LOG_FORM_TEXT_LOG_BUFF_WIDTH;

	fres = InitTextField(&_txtLogFormSystemLog, controlSize, controlPosition, FormatText, TRUE, _txtLogFormSystemLogBuff, controlBuffSizeTx, NULL);
	if (fres!=SUCCESS)
		return fres;
	_txtLogFormSystemLog.SetVisible(FALSE);

	controlBuffSizeTx.Height= LOG_FORM_TEXT_LOG_BUFF_HEIGHT*LOG_FORM_TEXT_LOG_MESSAGE_BUFF_PAGES;
	controlBuffSizeTx.Width= LOG_FORM_TEXT_LOG_BUFF_WIDTH;

	fres = InitTextField(&_txtLogFormMessageLog, controlSize, controlPosition, FormatText, TRUE, _txtLogFormMessageLogBuff, controlBuffSizeTx, NULL);
	if (fres!=SUCCESS)
		return fres;
	_txtLogFormMessageLog.SetVisible(FALSE);

	controlPosition.Top = LOG_FORM_TOP;
	controlPosition.Left = LOG_FORM_LEFT;

	controlSize.Height = LOG_FORM_HEIGHT;
	controlSize.Width =  LOG_FORM_WIDTH;

	_pnlLogFormContentControls[0] = &_txtLogFormSystemLog;
	_pnlLogFormContentControls[1] = &_txtLogFormMessageLog;
	fres = _pnlLogFormContent.Init(controlSize, controlPosition, &_renderer, _pnlLogFormContentControls, 2);
	if (fres!=SUCCESS)
		return fres;

	//General Init;
	_logs[LOG_SYSTEM]	= &_txtLogFormSystemLog;
	_logs[LOG_MESSAGE]	= &_txtLogFormMessageLog;

	_logNames[LOG_SYSTEM]   = LOG_SYSTEM_NAME;
	_logNames[LOG_MESSAGE]	   = LOG_MESSAGE_NAME;

	_currentLog =0;
	SetActiveLog(LOG_MESSAGE);

	controlPosition.Top = LOG_FORM_TOP;
	controlPosition.Left = LOG_FORM_LEFT;

	controlSize.Height = LOG_FORM_HEIGHT;
	controlSize.Width =  LOG_FORM_WIDTH;

	_pnlLogFormControls[0] = &_pnlLogFormHeader;
	_pnlLogFormControls[1] = &_pnlLogFormContent;
	fres = _pnlLogForm.Init(controlSize, controlPosition, &_renderer, _pnlLogFormControls, 2);
	if (fres!=SUCCESS)
		return fres;

	//init menu
	fres= InitLogFormMenu();
	if (fres!=SUCCESS)
		return fres;
	
	return SUCCESS;
}


fresult UIDesigner::Init()
{
	fresult fres;

	fres = InitMainForm();
	if (fres != SUCCESS)
		return fres;
	_frmMainForm.FormPanel = &_pnlMainForm;
	_frmMainForm.Menu = &_mnuMainMenu;
	_frmMainForm.BackgroundForm = NULL;

	fres = InitMsgBoxForm();
	if (fres != SUCCESS)
		return fres;
	_frmMsgBox.FormPanel = &_pnlMsgBox;
	_frmMsgBox.Menu = &_mnuMsgBoxMenu;
	_frmMsgBox.BackgroundForm = NULL;

	fres = InitWoundForm();
	if (fres != SUCCESS)
		return fres;
	_frmWoundForm.FormPanel = &_pnlWoundForm;
	_frmWoundForm.Menu = &_mnuWoundMenu;
	_frmWoundForm.BackgroundForm = NULL;

	fres = InitLogForm();
	if (fres != SUCCESS)
		return fres;
	_frmLogForm.FormPanel = &_pnlLogForm;
	_frmLogForm.Menu = &_mnuLogFormMenu;
	_frmLogForm.BackgroundForm = NULL;

	currentForm = NULL;
	
	return SUCCESS;
}

void UIDesigner::_SystemOnButtonClick(uword_t button )
{
	if (currentForm !=NULL)
	{
		bool_t menuItemFound = currentForm->Menu->ProcessButton(button);
		if (!menuItemFound)
		{
			OnButtonPressed(button);
		}
	}

	//TODO: remove
	Draw();
}


fresult UIDesigner::Draw()
{
	fresult fres;
	//clean!
	Size size;
	Position pos;
	pos.data =0;
	size.Height = SCREENY;
	size.Width = SCREENX;
	_renderer.DrawRect(pos, size, DEFAULT_BACKGROUND);

	if (currentForm!=NULL)
	{
		//draw status
		if (currentForm->FormPanel != NULL)
		{
			fres = currentForm->FormPanel->Draw();
			if (fres!=SUCCESS)
				return fres;
		}

		if (currentForm->Menu != NULL)
		{
			fres = currentForm->Menu->Draw();
			if (fres!=SUCCESS)
				return fres;
		}
	}

	return SUCCESS;
}


fresult UIDesigner::SetMedStatus(const char* text)
{
	return _txtMainFormMedStatus.SetText(text);
}

fresult UIDesigner::MessageBoxShow( const char* caption, const char* text, ubyte_t pictureId )
{
	
	fresult fres;
	fres = _txtMessageBoxTitle.SetText(caption);
	if (fres !=SUCCESS)
		return false;

	fres = _txtMessageBoxContent.SetText(text);
	if (fres !=SUCCESS)
		return false;

	if (pictureId!= NO_IMAGE)
	{
		BitmapImage* img = gImagesRepository.GetImageById(pictureId);
		if (img != NULL)
		{
			fres = _imgMessageBoxIcon.SetImage(img->Bitmap, img->ImageSize);
		}
	}

	if (currentForm != &_frmMsgBox)
	{
		fres = ShowForm(&_frmMsgBox);
		if (fres !=SUCCESS)
			return fres;
	}
	else
	{
		Draw();
	}

	return SUCCESS;
}

fresult UIDesigner::MessageBoxClose()
{
	return CloseForm();
}

fresult UIDesigner::InitTextField(TextField* tf, Size tfControlSize, Position tfControlPosition, ubyte_t textFormatId, bool_t wrap, char* buff, Size buffSize, const char* text)
{
	fresult fres;

	TextFormat* textFormat;
	textFormat = gFormatsRepository.GetFormatById(textFormatId);
	if (textFormat==NULL)
		return GENERAL_ERROR;

	fres = tf->Init(tfControlSize, tfControlPosition, buff, buffSize, &_renderer);
	if (fres !=SUCCESS)
		return fres;

	tf->SetTextFormat(textFormat);

	tf->SetWordWrap(wrap);

	if (text!=NULL)
	{
		fres = tf->SetText(text);
		if (fres !=SUCCESS)
			return fres;
	}

	return SUCCESS;
}

fresult UIDesigner::InitImage(Image* img, Size imgControlSize, Position imgControlPosition, ubyte_t pictureId)
{
	fresult fres;
	
	fres = img->Init(imgControlSize, imgControlPosition, &_renderer);
	if (fres !=SUCCESS)
		return fres;

	if (pictureId!= NO_IMAGE)
	{

		BitmapImage* bmp = gImagesRepository.GetImageById(pictureId);
	
		if (bmp!=NULL)
		{
			fres = img->SetImage(bmp->Bitmap, bmp->ImageSize);
			if (fres !=SUCCESS)
				return fres;
		}
		//fres = GetImageById(pictureId, &bitmap, &bitmapSize);
		//if (fres !=SUCCESS)
		///	return fres;

	}

	return SUCCESS;
}



fresult UIDesigner::InitMenuItem(MenuItem* mi,
								 TextField* tf, Size tfControlSize, Position tfControlPosition, ubyte_t textFormatId, char* buff, Size buffSize, const char* text, ubyte_t selTextFormatId,
								 Image* img, Size imgControlSize, Position imgControlPosition, sbyte_t image_id,
								 Panel* pnl,
								int handler, ButtonState buttonAccel)
{
	fresult fres;
	IControl* drawControl = NULL;
	bool_t hasText = FALSE;
	bool_t hasImage  = FALSE;

	if (tf!=NULL)
	{
		hasText = TRUE;
		fres = InitTextField(tf, tfControlSize, tfControlPosition, textFormatId, FALSE, buff, buffSize, text);
		if (fres!=SUCCESS)
			return fres;
	}

	if (img!=NULL)
	{
		hasImage = TRUE;
		fres = InitImage(img, imgControlSize, imgControlPosition, image_id);
		if (fres!=SUCCESS)
			return fres;

	}

	if (hasImage && hasText)
	{
		drawControl = pnl;
	}
	else if (hasImage)
	{
		drawControl = img;
	}
	else if (hasText)
	{
		drawControl = tf;
	}
	TextFormat* selFormat;
	selFormat = gFormatsRepository.GetFormatById(selTextFormatId);
	if (selFormat==NULL)
		return GENERAL_ERROR;

	fres = mi->Init(tf, selFormat, img, drawControl, GetMenuHandler((MenuHandlers)handler), buttonAccel);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

fresult UIDesigner::ShowForm( Form_DEPRECATED* form )
{
	form->BackgroundForm = currentForm;
	currentForm = form;
	Draw();
	return SUCCESS;
}

fresult UIDesigner::CloseForm()
{
	Form_DEPRECATED* curForm = currentForm;
	currentForm = currentForm->BackgroundForm;
	curForm ->BackgroundForm = NULL;
	Draw();
	return SUCCESS;
}




fresult UIDesigner::AppendLog( ubyte_t logId, const char* message )
{
	fresult fres;
	switch (logId)
	{
	case LOG_SYSTEM: 
			fres= _txtLogFormSystemLog.AppendText(message);
			if (fres!=SUCCESS)
				return fres;
		break;
	case LOG_MESSAGE: 
			fres = _txtLogFormMessageLog.AppendText(message);
			if (fres!=SUCCESS)
				return fres;
		break;
	default:
		return GENERAL_ERROR;
	}
	return SUCCESS;
}

fresult UIDesigner::SetActiveLog( ubyte_t logId)
{
	TextField* currentLogTf = _logs[_currentLog];
	TextField* newLogTf = NULL;
	sbyte_t icon;

	switch (logId)
	{
	case LOG_SYSTEM: 
		newLogTf = &_txtLogFormSystemLog;
		icon = BlueRss;
		break;
	case LOG_MESSAGE: 
		newLogTf = &_txtLogFormMessageLog;
		icon = BlueDiscuss;
		break;
	default:
		return GENERAL_ERROR;
	}


	currentLogTf->SetVisible(FALSE);
	newLogTf->SetVisible(TRUE);
	
	BitmapImage* iconImage = gImagesRepository.GetImageById(icon);
	
	fresult fres = _imgLogFormIcon.SetImage(iconImage->Bitmap, iconImage->ImageSize);
	if (fres != SUCCESS)
		return fres;

	_txtLogFormSubTitle.SetText(_logNames[logId]);
	_currentLog = logId;

	return SUCCESS;
}


fresult UIDesigner::OnButtonPressed( ButtonState button )
{
	return SUCCESS;
}

fresult UIDesigner::OnMainMnuScrollUp(IMenuItem* sender)
{
	return _txtMainFormMedStatus.ScrollUp();
}

fresult	UIDesigner::OnMainMnuScrollDown(IMenuItem* sender)
{
	return _txtMainFormMedStatus.ScrollDown();
}
fresult UIDesigner::OnMainMnuShowLogs( IMenuItem* sender )
{
	ShowForm(&_frmLogForm);

	return SUCCESS;
}


fresult UIDesigner::OnMsgBoxMnuOk( IMenuItem* sender )
{
	return MessageBoxClose();
}

fresult UIDesigner::OnMsgBoxMnuScrollUp( IMenuItem* sender )
{
	return _txtMessageBoxContent.ScrollUp();
}

fresult UIDesigner::OnMsgBoxMnuScrollDown( IMenuItem* sender )
{
	return _txtMessageBoxContent.ScrollDown();
}

fresult UIDesigner::OnWoundMnuOk( IMenuItem* sender )
{
	txtWoundResult.Clear();
	return CloseForm();
}

fresult UIDesigner::OnLogFormMnuScrollUp( IMenuItem* sender )
{
	
	return _logs[_currentLog]->ScrollUp();
}

fresult UIDesigner::OnLogFormMnuScrollDown( IMenuItem* sender )
{
	return _logs[_currentLog]->ScrollDown();
}

fresult UIDesigner::OnLogFormMnuPrev( IMenuItem* sender )
{
	ubyte_t newLog =0;
	if (_currentLog-1 <0)
	{
		newLog = LOGS_COUNT-1;
	}
	else
	{
		newLog = _currentLog-1;
	}
	SetActiveLog(newLog);
	Draw();

	return SUCCESS;
}

fresult UIDesigner::OnLogFormMnuNext( IMenuItem* sender )
{
	ubyte_t newLog =0;
	if (_currentLog+1 >= LOGS_COUNT )
	{
		newLog = 0;
	}
	else
	{
		newLog = _currentLog+1;
	}
	SetActiveLog(newLog);

	Draw();
	
	return SUCCESS;
}

fresult UIDesigner::OnLogFormMnuBack( IMenuItem* sender )
{
	return CloseForm();
}

fresult UIDesigner::OnLogFormMnuCleanLog( IMenuItem* sender )
{
	return _logs[_currentLog]->Clear();
}



*/