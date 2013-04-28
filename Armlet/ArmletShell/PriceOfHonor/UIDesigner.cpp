#include "UIDesigner.h"
#include "Constants.h"
#include "Icons.h"

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

	fres = TODO_KELTUR_GetFontById(1, &(_txtUserNameFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtUserNameFormat.BgColor = BLACK;
	_txtUserNameFormat.FgColor = WHITE;
	fres = InitTextField(&txtUserName, controlSize, controlPosition, &_txtUserNameFormat, FALSE, _txtUserNameBuff, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;

	//txtBatteryStatus
	controlSize.Width = BATTERYSTATUS_WIDTH_PX;
	controlSize.Height = BATTERYSTATUS_HEIGHT_PX;

	controlPosition.Top = BATTERYSTATUS_TOP_PX;
	//we have 4 spare pixels on screen
	controlPosition.Left = BATTERYSTATUS_LEFT_PX;

	controlBuffSizeTx.Width=BATTERYSTATUS_BUFF_WIDTH;
	controlBuffSizeTx.Height=BATTERYSTATUS_BUFF_HEIGHT;

	fres = TODO_KELTUR_GetFontById(1, &(_txtBatteryStatusFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtBatteryStatusFormat.BgColor = BLACK;
	_txtBatteryStatusFormat.FgColor = WHITE;
	txtBatteryStatus.SetTextFormat(&_txtBatteryStatusFormat);

	fres = InitTextField(&txtBatteryStatus, controlSize, controlPosition, &_txtBatteryStatusFormat, FALSE, _txtBatteryStatusBuff, controlBuffSizeTx, NULL);
	if (fres != SUCCESS)
		return fres;


	//pnlStatus
	controlPosition.Top = STATUSBAR_TOP_PX;
	controlPosition.Left = STATUSBAR_LEFT_PX;

	controlSize.Width = STATUSBAR_WIDTH_PX;
	controlSize.Height = STATUSBAR_HEIGHT_PX;

	_pnlStatusBarControls[0] = &txtUserName;
	_pnlStatusBarControls[1] = &txtBatteryStatus;

	fres = _pnlStatusBar.Init(controlSize, controlPosition, &_renderer, _pnlStatusBarControls, 2);
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

	//////////Help////////////////////
	//imgHelp
	controlPosition.Top = 8-4;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miHelp, 
							NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
							&_imgMenuHelp, controlSize, controlPosition, ORANGE_ARROW_DOWN,
							NULL, OnMainMnuHelpHandler, BUTTON_A);
	if (fres!=SUCCESS)
		return fres;
	
	//////////ScrollUp////////////////////
	//imgScrollUp
	controlPosition.Top = 40-4;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miScrollUp, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuScrollUp, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuScrollUpHandler, BUTTON_B);
	if (fres!=SUCCESS)
		return fres;
	_miScrollUp.SetSecondAccelarator(BUTTON_HOLD_B);

	//////////imgScrollDown////////////////////
	//imgScrollDown
	controlPosition.Top = 72-4;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miScrollDown, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuScrollDown, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuScrollDownHandler, BUTTON_C);
	if (fres!=SUCCESS)
		return fres;
	_miScrollDown.SetSecondAccelarator(BUTTON_HOLD_C);

	//////////imgCycleLeft////////////////////
	//imgCycleLeft
	controlPosition.Top = 104;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miCycleLeft, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuCycleLeft, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuCycleLeftHandler, BUTTON_L);
	if (fres!=SUCCESS)
		return fres;

	//////////imgNewShot////////////////////
	//imgNewShot
	controlPosition.Top = 8;
	controlPosition.Left = SCREENX - MENU_IMAGE_WIDTH-1;
	fres = InitMenuItem(&_miNewShot, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuNewShot, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuShotHandler, BUTTON_X);
	if (fres!=SUCCESS)
		return fres;

	//////////imgKnockOut////////////////////
	//imgKnockOut
	controlPosition.Top = 40;
	controlPosition.Left = SCREENX - MENU_IMAGE_WIDTH-1;
	fres = InitMenuItem(&_miKnockOut, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuKnockOut, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuKnockoutHandler, BUTTON_Y);
	if (fres!=SUCCESS)
		return fres;


	//////////imgOpenLock////////////////////
	//imgOpenLock
	controlPosition.Top = 72;
	controlPosition.Left = SCREENX - MENU_IMAGE_WIDTH-1;
	fres = InitMenuItem(&_miOpenLock, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuOpenLock, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuOpenLockHandler, BUTTON_Z);
	if (fres!=SUCCESS)
		return fres;

	//////////imgCycleRight////////////////////
	//imgCycleRight
	controlPosition.Top = 104;
	controlPosition.Left = SCREENX - MENU_IMAGE_WIDTH-1;
	fres = InitMenuItem(&_miCycleRight, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMenuCycleRight, controlSize, controlPosition, ORANGE_ARROW_DOWN,
		NULL, OnMainMnuCycleRightHandler, BUTTON_R);
	if (fres!=SUCCESS)
		return fres;

	//mnuMainMenu
	_mnuMainMenuItems[0] = &_miHelp;
	_mnuMainMenuItems[1] = &_miScrollUp;
	_mnuMainMenuItems[2] = &_miScrollDown;
	_mnuMainMenuItems[3] = &_miCycleLeft;
	_mnuMainMenuItems[4] = &_miNewShot;
	_mnuMainMenuItems[5] = &_miKnockOut;
	_mnuMainMenuItems[6] = &_miOpenLock;
	_mnuMainMenuItems[7] = &_miCycleRight;

	fres =_mnuMainMenu.Init(_mnuMainMenuItems, 8);
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

	//txtUserName

	controlPosition.Top = DIALOG_TOP_PX;
	controlPosition.Left = DIALOG_LEFT_PX;

	controlSize.Height = DIALOG_HEIGHT_PX;
	controlSize.Width = DIALOG_WIDTH_PX;

	controlBuffSizeTx.Width=STATUS_BUFF_WIDTH;
	controlBuffSizeTx.Height=STATUS_BUFF_HEIGHT;

	fres = TODO_KELTUR_GetFontById(1, &(_txtStatusFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtStatusFormat.BgColor = WHITE;
	_txtStatusFormat.FgColor = RED;
	txtStatus.SetTextFormat(&_txtStatusFormat);
	fres = InitTextField(&txtStatus, controlSize, controlPosition, &_txtStatusFormat, TRUE, _txtStatusBuff, controlBuffSizeTx, NULL);
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

	_pnlMainFormControls[0] = &txtStatus;
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

	
	fres = TODO_KELTUR_GetFontById(1, &(_txtMessageBoxTitleFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtMessageBoxTitleFormat.BgColor = BLUE;
	_txtMessageBoxTitleFormat.FgColor = WHITE;

	fres = InitTextField(&_txtMessageBoxTitle, controlSize, controlPosition, &_txtMessageBoxTitleFormat, FALSE, _txtMessageBoxTitleBuff, controlBuffSizeTx, NULL );
	if (fres != SUCCESS)
		return fres;

	//_txtMessageBoxContent
	controlPosition.Top  = MSGBOX_CONTENT_TOP;
	controlPosition.Left = MSGBOX_CONTENT_LEFT;

	controlSize.Height  = MSGBOX_CONTENT_HEIGHT;
	controlSize.Width = MSGBOX_CONTENT_WIDTH;

	controlBuffSizeTx.Height=MSGBOX_CONTENT_BUFF_HEIGHT;
	controlBuffSizeTx.Width=MSGBOX_CONTENT_BUFF_WIDTH;


	fres = TODO_KELTUR_GetFontById(1, &(_txtMessageBoxContentFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtMessageBoxContentFormat.BgColor = WHITE;
	_txtMessageBoxContentFormat.FgColor = BLACK;

	fres = InitTextField(&_txtMessageBoxContent, controlSize, controlPosition, &_txtMessageBoxContentFormat, TRUE, _txtMessageBoxContentBuff, controlBuffSizeTx, NULL );
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
							&_imgMsgBoxMnuOk, controlSize, controlPosition, ORANGE_ARROW_DOWN,
							NULL, OnMsgBoxMnuOkHandler, BUTTON_X);
	if (fres != SUCCESS)
		return fres;
	_miMsgBoxMnuOk.SetSecondAccelarator(BUTTON_HOLD_E);

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

	_currentForm = NULL;

	fres = ShowForm(&_frmMainForm);
	if (fres != SUCCESS)
		return fres;
	
	return SUCCESS;
}

void UIDesigner::_SystemOnButtonClick( int button )
{
	if (_currentForm !=NULL)
	{
		bool_t menuItemFound = _currentForm->Menu->ProcessButton(button);
		if (!menuItemFound)
		{
			OnButtonPressed(button);
		}
	}

	//TODO: remove
	Draw();
}

void UIDesigner::OnNetworkData()
{

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
	_renderer.DrawRect(pos, size, BLACK);

	if (_currentForm!=NULL)
	{
		//draw status
		if (_currentForm->FormPanel != NULL)
		{
			fres = _currentForm->FormPanel->Draw();
			if (fres!=SUCCESS)
				return fres;
		}

		if (_currentForm->Menu != NULL)
		{
			fres = _currentForm->Menu->Draw();
			if (fres!=SUCCESS)
				return fres;
		}
	}

	return SUCCESS;
}

fresult UIDesigner::OnButtonPressed( ButtonState button )
{
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

fresult UIDesigner::MessageBoxShow( const char* caption, const char* text, ubyte_t pictureId )
{
	fresult fres;
	fres = _txtMessageBoxTitle.SetText(caption);
	if (fres !=SUCCESS)
		return false;

	fres = _txtMessageBoxContent.SetText(text);
	if (fres !=SUCCESS)
		return false;

	unsigned short* bitmap;  
	Size bitmapSize;

	if (pictureId!= NO_IMAGE)
	{
		fres = GetImageById(pictureId, &bitmap, &bitmapSize);
		if (fres !=SUCCESS)
			return fres;

		fres = _imgMessageBoxIcon.SetImage(bitmap, bitmapSize);
	}

	fres = ShowForm(&_frmMsgBox);
	if (fres !=SUCCESS)
		return fres;

	return SUCCESS;
}

fresult UIDesigner::MessageBoxClose()
{
	return CloseForm();
}

fresult UIDesigner::InitTextField(TextField* tf, Size tfControlSize, Position tfControlPosition, TextFormat* textFormat, bool_t wrap, char* buff, Size buffSize, const char* text)
{
	fresult fres;

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
	unsigned short* bitmap;  
	Size bitmapSize;
	
	fres = img->Init(imgControlSize, imgControlPosition, &_renderer);
	if (fres !=SUCCESS)
		return fres;

	if (pictureId!= NO_IMAGE)
	{
		fres = GetImageById(pictureId, &bitmap, &bitmapSize);
		if (fres !=SUCCESS)
			return fres;

		fres = img->SetImage(bitmap, bitmapSize);
		if (fres !=SUCCESS)
			return fres;
	}

	return SUCCESS;
}



fresult UIDesigner::InitMenuItem(MenuItem* mi,
								 TextField* tf, Size tfControlSize, Position tfControlPosition, TextFormat* textFormat, char* buff, Size buffSize, const char* text, TextFormat* selTextFormat,
								 Image* img, Size imgControlSize, Position imgControlPosition, sbyte_t image_id,
								 Panel* pnl,
								 /*MenuHandlers*/int handler, ButtonState buttonAccel)
{
	fresult fres;
	IControl* drawControl = NULL;
	bool_t hasText = FALSE;
	bool_t hasImage  = FALSE;

	if (tf!=NULL)
	{
		hasText = TRUE;
		fres = InitTextField(tf, tfControlSize, tfControlPosition, textFormat, FALSE, buff, buffSize, text);
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

	//tmiHelp
	fres = mi->Init(tf, selTextFormat, img, drawControl, GetMenuHandler((MenuHandlers)handler), buttonAccel);
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

	controlSize.Height = WOUND_MNU_ITEM_HIEGHT;
	controlSize.Width = WOUND_MNU_ITEM_WIDTH;
	controlBuffSizeTx.Height=WOUND_MNU_TEXT_BUFF_HIEGHT;
	controlBuffSizeTx.Width=WOUND_MNU_TEXT_BUFF_WIDTH;

	fres = TODO_KELTUR_GetFontById(1, &(_mnuWoundFormMenuFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_mnuWoundFormMenuFormat.BgColor = GREEN;
	_mnuWoundFormMenuFormat.FgColor = RED;

	fres = TODO_KELTUR_GetFontById(1, &(_mnuWoundFormMenuFormatSelected.Font));
	if (fres != SUCCESS)
		return fres;
	_mnuWoundFormMenuFormatSelected.BgColor = RED;
	_mnuWoundFormMenuFormatSelected.FgColor = GREEN;

	//miWoundHead
	controlPosition.Top = 16;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miWoundHead, &_txtMnuWoundHead, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundHeadTextBuff, controlBuffSizeTx, "Голова", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuHeadHandler, BUTTON_A);
	if (fres != SUCCESS)
		return fres;

	//miWoundStomach
	controlPosition.Top = 48;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miWoundStomach, &_txtMnuWoundStomach, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundStomachTextBuff, controlBuffSizeTx, "Живот", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuStomachHandler, BUTTON_B);
	if (fres != SUCCESS)
		return fres;

	//miWoundLeftArm
	controlPosition.Top = 80;
	controlPosition.Left = 1;
	fres = InitMenuItem(&_miWoundLeftArm, &_txtMnuWoundLeftArm, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundLeftArmTextBuff, controlBuffSizeTx, "Левая\nрука", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuLeftArmHandler, BUTTON_C);
	if (fres != SUCCESS)
		return fres;

	//miWoundRightArm
	controlPosition.Top = 112;
	controlPosition.Left = 6+1;
	fres = InitMenuItem(&_miWoundLeftLeg, &_txtMnuWoundLeftLeg, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundLeftLegTextBuff, controlBuffSizeTx, " Левая\n  нога", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuLeftLegHandler, BUTTON_L);
	if (fres != SUCCESS)
		return fres;
	
	//miWoundChest
	controlPosition.Top = 16;
	controlPosition.Left = 120+3;
	fres = InitMenuItem(&_miWoundChest, &_txtMnuWoundChest, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundChestTextBuff, controlBuffSizeTx, " Грудь", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuChestHandler, BUTTON_X);
	if (fres != SUCCESS)
		return fres;

	//miWoundBack
	controlPosition.Top = 48;
	controlPosition.Left = 120+3;
	fres = InitMenuItem(&_miWoundBack, &_txtMnuWoundBack, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundBackTextBuff, controlBuffSizeTx, " Спина", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuBackHandler, BUTTON_Y);
	if (fres != SUCCESS)
		return fres;

	//miWoundLeftLeg
	controlPosition.Top = 80;
	controlPosition.Left = 120+3;
	fres = InitMenuItem(&_miWoundRightArm, &_txtMnuWoundRightArm, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundRightArmTextBuff, controlBuffSizeTx, "Правaя\n  рука", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuRightArmHandler, BUTTON_Z);
	if (fres != SUCCESS)
		return fres;

	//miWoundRightLeg
	controlPosition.Top = 112;
	controlPosition.Left = 114+3;
	fres = InitMenuItem(&_miWoundRightLeg, &_txtMnuWoundRightLeg, controlSize, controlPosition, &_mnuWoundFormMenuFormat, _txtMnuWoundRightLegTextBuff, controlBuffSizeTx, "Правая\nнога", &_mnuWoundFormMenuFormatSelected,
		NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnWoundMnuRightLegHandler, BUTTON_R);
	if (fres != SUCCESS)
		return fres;

	//miOk
	controlSize.Width = MENU_IMAGE_HEIGHT;
	controlSize.Width = MENU_IMAGE_WIDTH;
	controlPosition.Top = 104;
	controlPosition.Left = 68;

	fres = InitMenuItem(&_miWoundOk, NULL, zeroSize, zeroPosition, NULL, NULL, zeroSize, NULL, NULL,
		&_imgMnuWoundOk, controlSize, controlPosition, ORANGE_ARROW_DOWN, NULL, OnWoundMnuOkHandler, BUTTON_E);
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

	controlSize.Height = WOUND_FORM_TITLE_HIEGHT;
	controlSize.Width = WOUND_FORM_TITLE_WIDTH;

	controlBuffSizeTx.Height=WOUND_FORM_TITLE_TEXT_BUFF_HIEGHT;
	controlBuffSizeTx.Width=WOUND_FORM_TITLE_TEXT_BUFF_WIDTH;

	fres = TODO_KELTUR_GetFontById(1, &(_txtWoundFormTitleFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtWoundFormTitleFormat.BgColor = GREEN;
	_txtWoundFormTitleFormat.FgColor = BLUE;

	fres = InitTextField(&_txtWoundFormTitle, controlSize, controlPosition, &_txtWoundFormTitleFormat, FALSE, _txtWoundFormTitleTextBuff, controlBuffSizeTx, "Укажи место ранения!");
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = WOUND_RESULT_TOP;
	controlPosition.Left = WOUND_RESULT_LEFT;

	controlSize.Height = WOUND_RESULT_HEIGHT;
	controlSize.Width = WOUND_RESULT_WIDTH;

	controlBuffSizeTx.Height=WOUND_RESULT_TEXT_BUFF_HIEGHT;
	controlBuffSizeTx.Width=WOUND_RESULT_TEXT_BUFF_WIDTH;


	fres = TODO_KELTUR_GetFontById(1, &(_txtWoundResultFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtWoundResultFormat.BgColor = WHITE;
	_txtWoundResultFormat.FgColor = RED;

	fres = InitTextField(&txtWoundResult, controlSize, controlPosition, &_txtWoundResultFormat, TRUE, _txtWoundResultTextBuff, controlBuffSizeTx, NULL);
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = 0;
	controlPosition.Left = 0;

	controlSize.Height = WOUND_FORM_HIEGHT;
	controlSize.Width = WOUND_FORM_WIDTH;

	_pnlWoundFormControls[0] = &_txtWoundFormTitle;
	_pnlWoundFormControls[1] = &txtWoundResult;
	fres = _pnlWoundForm.Init(controlSize, controlPosition, &_renderer, _pnlWoundFormControls,2);
	if (fres!=SUCCESS)
		return fres;

	fres = InitWoundFormMenu();
	return fres;
}

fresult UIDesigner::ShowForm( Form* form )
{
	form->BackgroundForm = _currentForm;
	_currentForm = form;
	Draw();
	return SUCCESS;
}

fresult UIDesigner::CloseForm()
{
	Form* curForm = _currentForm;
	_currentForm = _currentForm->BackgroundForm;
	curForm ->BackgroundForm = NULL;
	Draw();
	return SUCCESS;
}

fresult UIDesigner::OnWoundMnuOk( IMenuItem* sender )
{
	txtWoundResult.Clear();
	return CloseForm();
}
