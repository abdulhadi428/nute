#pragma once

#include "project.h"
#include "TextField.h"
#include "Panel.h"
#include "ScatteredMenu.h"
#include "TextMenuItem.h"
#include "GraphicRenderer.h"

class UIDesigner
{

private:

	//pnlStatusBar
	IControl* _pnlStatusBarControls[2];
		//txtUserName
		char _txtUserNameBuff[18*2];
		TextFormat _txtUserNameFormat;

		//txtBatteryStatus
		char _txtBatteryStatusBuff[2*1];
		TextFormat _txtBatteryStatusFormat;

	//pnlMainForm
	IControl* _pnlMainFormControls[1];
		//txtStatus
		char _txtStatusBuff[16*13*3];
		TextFormat _txtStatusFormat;
		
		//mnuMainMenu;
		TextFormat _mnuMainMenuFormat;
		TextFormat _mnuMainMenuFormatSelected;
		IMenuItem* _mnuMainMenuItems[3];
			//tmiOpenLock
				//txtMenuOpenLock
				TextField _txtMenuOpenLock;
				char _txtMenuOpenLockBuff[5*2];
			//tmiNewWound
				//txtMenuNewWound
				TextField _txtMenuNewWound;
				char _txtMenuNewWoundBuff[5*2];
			//tmiTest
				//txtMenuTest
				TextField _txtMenuTest;
				char _txtMenuTestBuff[5*2];

protected:	
	GraphicRenderer _renderer;
	Panel* _currentForm;
	IMenu* _currentMenu;

	Panel pnlStatusBar;
		TextField txtUserName;
		TextField txtBatteryStatus;

	Panel pnlMainForm;
		TextField txtStatus;
		ScatteredMenu mnuMainMenu;
			TextMenuItem tmiOpenLock;
			TextMenuItem tmiNewWound;
			TextMenuItem tmiTest;
	
					

	fresult InitStatusBar();
	fresult InitMainFormMnu();
	fresult InitMainForm();

protected:
	fresult Init();
public:
	
	fresult Draw();

	void _SystemOnButtonClick(int button);
	void OnNetworkData();

};
