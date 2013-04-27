#include "UIDesigner.h"
#include "Constants.h"

//W160H128

fresult UIDesigner::InitStatusBar()
{
	//txtUserName
	Size controlSize;
	controlSize.Width = 144;
	controlSize.Height = 16;

	Position controlPosition;
	controlPosition.Top = 0;
	controlPosition.Left = 0;

	Size controlBuffSizeTx;
	controlBuffSizeTx.Width=18;
	controlBuffSizeTx.Height=2;

	fresult fres;
	fres = txtUserName.Init(controlSize, controlPosition, _txtUserNameBuff, controlBuffSizeTx, &_renderer);
	if (fres != SUCCESS)
		return fres;
	fres = TODO_KELTUR_GetFontById(1, &(_txtUserNameFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtUserNameFormat.BgColor = BLACK;
	_txtUserNameFormat.FgColor = WHITE;
	txtUserName.SetTextFormat(&_txtUserNameFormat);

	//txtBatteryStatus
	controlSize.Width = 16;
	controlSize.Height = 16;

	controlPosition.Top = 0;
	controlPosition.Left = 144;

	controlBuffSizeTx.Width=2;
	controlBuffSizeTx.Height=2;

	fres = txtBatteryStatus.Init(controlSize, controlPosition, _txtBatteryStatusBuff, controlBuffSizeTx, &_renderer);
	if (fres != SUCCESS)
		return fres;
	fres = TODO_KELTUR_GetFontById(1, &(_txtBatteryStatusFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtBatteryStatusFormat.BgColor = BLACK;
	_txtBatteryStatusFormat.FgColor = WHITE;
	txtBatteryStatus.SetTextFormat(&_txtBatteryStatusFormat);

	//pnlStatus
	controlPosition.Top = 0;
	controlPosition.Left = 0;

	controlSize.Width = 160;
	controlSize.Height = 16;

	_pnlStatusBarControls[0] = &txtUserName;
	_pnlStatusBarControls[1] = &txtBatteryStatus;

	fres = pnlStatusBar.Init(controlSize, controlPosition, &_renderer, _pnlStatusBarControls, 2);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;

}

fresult UIDesigner::InitMainFormMnu()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;
	Size controlBuffSizeTx;

	//All Items
	controlSize.Width = 40;
	controlSize.Height = 16;

	controlBuffSizeTx.Width=5;
	controlBuffSizeTx.Height=2;

	_mnuMainMenuFormat.BgColor = BLACK;
	_mnuMainMenuFormat.FgColor = WHITE;

	fres = TODO_KELTUR_GetFontById(1, &(_mnuMainMenuFormat.Font));
	if (fres != SUCCESS)
		return fres;

	_mnuMainMenuFormatSelected.BgColor = RED;
	_mnuMainMenuFormatSelected.FgColor = WHITE;

	fres = TODO_KELTUR_GetFontById(1, &(_mnuMainMenuFormatSelected.Font));
	if (fres != SUCCESS)
		return fres;

	//txtMenuOpenLock
	controlPosition.Top = 24;
	controlPosition.Left = 128;
	fres = _txtMenuOpenLock.Init(controlSize, controlPosition, _txtMenuOpenLockBuff, controlBuffSizeTx, &_renderer);
	if (fres != SUCCESS)
		return fres;
	_txtMenuOpenLock.SetTextFormat(&_mnuMainMenuFormat);
	fres = _txtMenuOpenLock.SetText("откр.\nзамок");
	if (fres != SUCCESS)
		return fres;

	//tmiOpenLock
	fres = tmiOpenLock.Init(&_txtMenuOpenLock, &_mnuMainMenuFormatSelected);
	if (fres != SUCCESS)
		return fres;

	//txtMenuNewWound
	controlPosition.Top = 64;
	controlPosition.Left = 128;
	fres = _txtMenuNewWound.Init(controlSize, controlPosition, _txtMenuNewWoundBuff, controlBuffSizeTx, &_renderer);
	if (fres != SUCCESS)
		return fres;
	_txtMenuNewWound.SetTextFormat(&_mnuMainMenuFormat);
	fres = _txtMenuNewWound.SetText("ранен");
	if (fres != SUCCESS)
		return fres;

	//tmiNewWound
	fres = tmiNewWound.Init(&_txtMenuNewWound, &_mnuMainMenuFormatSelected);
	if (fres != SUCCESS)
		return fres;	

	//txtMenuTest
	controlPosition.Top = 104;
	controlPosition.Left = 128;
	fres = _txtMenuTest.Init(controlSize, controlPosition, _txtMenuTestBuff, controlBuffSizeTx, &_renderer);
	if (fres != SUCCESS)
		return fres;
	_txtMenuTest.SetTextFormat(&_mnuMainMenuFormat);
	fres = _txtMenuTest.SetText("тест");
	if (fres != SUCCESS)
		return fres;

	//tmiTest
	fres = tmiTest.Init(&_txtMenuTest, &_mnuMainMenuFormatSelected);
	if (fres != SUCCESS)
		return fres;

	//mnuMainMenu
	_mnuMainMenuItems[0] = &tmiOpenLock;
	_mnuMainMenuItems[1] = &tmiNewWound;
	_mnuMainMenuItems[2] = &tmiTest;

	fres =mnuMainMenu.Init(_mnuMainMenuItems, 3);
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

	controlPosition.Top = 16;
	controlPosition.Left = 0;

	controlSize.Width = 120;
	controlSize.Height = 104;

	controlBuffSizeTx.Width=16;
	//3 pages down
	controlBuffSizeTx.Height=13*3;

	fres = txtStatus.Init(controlSize, controlPosition, _txtStatusBuff, controlBuffSizeTx, &_renderer);
	if (fres != SUCCESS)
		return fres;
	fres = TODO_KELTUR_GetFontById(1, &(_txtStatusFormat.Font));
	if (fres != SUCCESS)
		return fres;
	_txtStatusFormat.BgColor = BLACK;
	_txtStatusFormat.FgColor = WHITE;
	txtStatus.SetTextFormat(&_txtStatusFormat);

	//pnlMainForm
	controlPosition.Top = 16;
	controlPosition.Left = 0;

	controlSize.Width = 160;
	controlSize.Height = 128;

	_pnlMainFormControls[0] = &txtStatus;

	fres = pnlMainForm.Init(controlSize, controlPosition, &_renderer, _pnlMainFormControls, 1);
	if (fres != SUCCESS)
		return fres;

	fres = InitMainFormMnu();
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}


fresult UIDesigner::Init()
{
	fresult fres;
	fres = InitStatusBar();
	if (fres != SUCCESS)
		return fres;

	fres = InitMainForm();
	if (fres != SUCCESS)
		return fres;

	_currentForm = &pnlMainForm;
	_currentMenu = &mnuMainMenu;

	return SUCCESS;
}

void UIDesigner::_SystemOnButtonClick( int button )
{

}

void UIDesigner::OnNetworkData()
{

}

fresult UIDesigner::Draw()
{
	fresult fres;

	//draw status
	fres = pnlStatusBar.Draw();
	if (fres!=SUCCESS)
		return fres;

	//draw current dialog
	fres = _currentForm->Draw();
	if (fres!=SUCCESS)
		return fres;

	fres = _currentMenu->Draw();
	if (fres!=SUCCESS)
		return fres;

	return SUCCESS;
}
