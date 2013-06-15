#include "ArmletShell.h"
#include "armletapi.h"
#include "UIDesigner.h"
#include "menudelegate.h"
#include "Images.h"
#include "ColorSchema.h"
#include <stdio.h>

#pragma warning(disable:4100)

char* OSANVES[OSANVE_COUNT] = {"все норм","творю","радуюсь","жду помощь","скучаю","подавлен","страдаю","ранен","веду бой"};
char* CONS[CONS_COUNT] = {"истощение","миражи","безумие","печаль","боль","берсерк"};

//W160H128
// Roman: Время Имя Сила/Макс Батарея
// настройка верхней статусной строки
fresult UIDesigner::InitStatusBar()
{
	//txtUserName - Имя
	Size controlSize;
	Size controlBuffSizeTx;
	Position controlPosition;
	fresult fres;
	controlSize.Width = USERNAME_WIDTH_PX;
	controlSize.Height = USERNAME_HEIGHT_PX;

	controlPosition.Top = USERNAME_TOP_PX;
	controlPosition.Left = USERNAME_LEFT_PX;

	controlBuffSizeTx.Width=USERNAME_BUFF_WIDTH;
	controlBuffSizeTx.Height=USERNAME_BUFF_HEIGHT;

	fres = InitTextField(&txtUserName, controlSize, controlPosition, FormatHeader, FALSE, _txtUserNameBuff, controlBuffSizeTx, Player.name);
	if (fres != SUCCESS)
		return fres;
	
	//txtBatteryStatus - батарея
	controlSize.Width = BATTERYSTATUS_WIDTH_PX;
	controlSize.Height = BATTERYSTATUS_HEIGHT_PX;

	controlPosition.Top = BATTERYSTATUS_TOP_PX;
	controlPosition.Left = BATTERYSTATUS_LEFT_PX;

	controlBuffSizeTx.Width=BATTERYSTATUS_BUFF_WIDTH;
	controlBuffSizeTx.Height=BATTERYSTATUS_BUFF_HEIGHT;

	fres = InitTextField(&txtBatteryStatus, controlSize, controlPosition, FormatHeader, FALSE, _txtBatteryStatusBuff, controlBuffSizeTx, "100");
	if (fres != SUCCESS)
		return fres;

	//txtOsanve - OSANVE (своё)
	controlSize.Width = SCREENX;
	controlSize.Height = TIMESTATUS_HEIGHT_PX;

	controlPosition.Top = USERNAME_HEIGHT_PX;
	controlPosition.Left = TIMESTATUS_LEFT_PX;

	controlBuffSizeTx.Width=MSGBOX_CONTENT_BUFF_WIDTH;
	controlBuffSizeTx.Height=1;

	fres = InitTextField(&txtOsanve, controlSize, controlPosition, FormatHeader, FALSE, _txtOsanveBuff, controlBuffSizeTx, OSANVES[Player.osanve]);
	if (fres != SUCCESS)
		return fres;

	//txtFORCE - сила
	controlPosition.Top = FORCESTATUS_TOP_PX;
	controlPosition.Left = FORCESTATUS_LEFT_PX;

	controlSize.Height = FORCESTATUS_HEIGHT_PX;
	controlSize.Width = FORCESTATUS_WIDTH_PX;

	controlBuffSizeTx.Height=FORCESTATUS_BUFF_HEIGHT;
	controlBuffSizeTx.Width=FORCESTATUS_BUFF_WIDTH;
	char tmp[9];
	sprintf( tmp, "%03d/%03d ", Player.force, Player.maxForce);
	fres = InitTextField(&txtForce, controlSize, controlPosition, FormatHeader, FALSE, _txtForce, controlBuffSizeTx, tmp);
	if (fres != SUCCESS)
		return fres;


	//pnlStatus - строка со всеми созданными элементами
	controlPosition.Top = STATUSBAR_TOP_PX;
	controlPosition.Left = STATUSBAR_LEFT_PX;

	controlSize.Width = STATUSBAR_WIDTH_PX;
	controlSize.Height = STATUSBAR_HEIGHT_PX;

	_pnlStatusBarControls[0] = &txtUserName;
	_pnlStatusBarControls[1] = &txtForce;
	_pnlStatusBarControls[2] = &txtBatteryStatus;
	_pnlStatusBarControls[3] = &txtOsanve;

	fres = _pnlStatusBar.Init(controlSize, controlPosition, &_renderer, _pnlStatusBarControls, 4);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Создание основного меню
// Меню осанве: L - Бой E - Листать R - Выбор
// Все меню текстовые, без картинок
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
	controlSize.Width = SCREENX / 3;
	controlSize.Height = MENU_VERTICAL_GAP;

	Size tfSize;
	tfSize.Height = 1;
	tfSize.Width = 9;

	//////////L////////////////////
	controlPosition.Top = MENU_ITEM_L_TOP;
	controlPosition.Left = MENU_ITEM_L_LEFT;
	fres = InitMenuItem(&_menuFight, &_txtMenuFight, controlSize, controlPosition, FormatMenu, _bufMenuFight, tfSize, MENU_OSANVE_L, FormatMenu,
		NULL, zeroSize, zeroPos, NULL, NULL, OnMainMnuFightH, BUTTON_L);
	if (fres!=SUCCESS)
		return fres;
	_menuFight.SetSecondAccelarator(BUTTON_HOLD_L);

	//////////E////////////////////
	controlPosition.Top = MENU_ITEM_E_TOP;
	controlPosition.Left = MENU_ITEM_E_LEFT;
	fres = InitMenuItem(&_menuList, &_txtMenuList, controlSize, controlPosition, FormatMenu, _bufMenuList, tfSize, MENU_OSANVE_E, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnMainMnuListH, BUTTON_E);
	if (fres!=SUCCESS)
		return fres;
	_menuList.SetSecondAccelarator(BUTTON_HOLD_E);

	//////////R////////////////////
	controlPosition.Top = MENU_ITEM_R_TOP;
	controlPosition.Left = MENU_ITEM_R_LEFT;
	fres = InitMenuItem(&_menuSet, &_txtMenuSet, controlSize, controlPosition, FormatMenu, _bufMenuSet, tfSize, MENU_OSANVE_R, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnMainMnuSetH, BUTTON_R);
	if (fres!=SUCCESS)
		return fres;
	_menuSet.SetSecondAccelarator(BUTTON_HOLD_R);


	//mnuMainMenu
	_mnuMainMenuItems[0] = &_menuFight;
	_mnuMainMenuItems[1] = &_menuList;
	_mnuMainMenuItems[2] = &_menuSet;
	

	fres =_mnuMainMenu.Init(_mnuMainMenuItems, 3);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Настройка основного интерфейса: центральный список, статусбар, меню
fresult UIDesigner::InitMainForm()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;
	Size controlBuffSizeTx;

	//_txtOsanveList
	controlPosition.Top = DIALOG_TOP_PX;
	controlPosition.Left = DIALOG_LEFT_PX;

	controlSize.Height = DIALOG_HEIGHT_PX;
	controlSize.Width = DIALOG_WIDTH_PX;

	controlBuffSizeTx.Width=MSGBOX_CONTENT_BUFF_WIDTH;
	controlBuffSizeTx.Height=MSGBOX_CONTENT_BUFF_HEIGHT*MSGBOX_CONTENT_BUFF_PAGES;

	fres = InitTextField(&_txtOsanveList, controlSize, controlPosition, FormatText, TRUE, _bufOsanveList, controlBuffSizeTx, NULL);
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

	_pnlMainFormControls[0] = &_txtOsanveList;
	_pnlMainFormControls[1] = &_pnlStatusBar;

	fres = _pnlMainForm.Init(controlSize, controlPosition, &_renderer, _pnlMainFormControls, 2);
	if (fres != SUCCESS)
		return fres;

	fres = InitMainFormMnu();
//	fres=SetOsanve();
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}


//Roman
//Select OSANVE
fresult UIDesigner::InitOsanveFormMenu()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	Size zeroSize;
	Position zeroPosition;

	zeroSize.data =0;
	zeroPosition.data =0;

	controlSize.Height = MENU_VERTICAL_GAP;
	controlSize.Width = 60;
	controlBuffSizeTx.Height=1;
	controlBuffSizeTx.Width=10;
// пункты меню выбора осанве
  for( int i = 0; i < (OSANVE_COUNT - 1); i++)
  {
 		if( i < 6) {
 		  controlPosition.Top = i % 3 * (MENU_IMAGE_HEIGHT + MENU_VERTICAL_GAP) + STATUSBAR_HEIGHT_PX + MENU_VERTICAL_GAP;
 		  controlPosition.Left = i / 3 * 100;
 		} else {
 		  controlPosition.Top = 120;
          controlPosition.Left = (SCREENX/3) * (i%3);
    }
	  fres = InitMenuItem(&_miOsanve[i], &_txtOsanveMenu[i], controlSize, controlPosition, FormatMenu, _bufOsanveMenu[i], controlBuffSizeTx, OSANVES[i], FormatMenu,
		       NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnOsanveMnuSelectH, i);
	  if (fres != SUCCESS)
		  return fres;
	  _mnuOsanveMenuItems[i] = &_miOsanve[i];
  }
  
  // пункт отмена
  controlPosition.Top = 120;
  controlPosition.Left = 120;
  fres = InitMenuItem(&_miOsanve[OSANVE_COUNT - 1], &_txtOsanveMenu[OSANVE_COUNT - 1], controlSize, controlPosition, FormatMenu, _bufOsanveMenu[OSANVE_COUNT - 1], controlBuffSizeTx, "Отмена", FormatMenu,
		       NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnOsanveMnuCancelH, BUTTON_R);
  if (fres != SUCCESS)
		  return fres;
	_mnuOsanveMenuItems[OSANVE_COUNT - 1] = &_miOsanve[OSANVE_COUNT - 1];
	
	fres = _mnuOsanveMenu.Init(_mnuOsanveMenuItems, OSANVE_COUNT);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Форма осанве
fresult UIDesigner::InitOsanveForm()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	controlPosition.Top = STATUSBAR_HEIGHT_PX;
	controlPosition.Left = MENU_IMAGE_WIDTH;

	controlSize.Height = MENU_VERTICAL_GAP;
	controlSize.Width = SCREENX - (controlPosition.Left*2);

	controlBuffSizeTx.Height=1;
	controlBuffSizeTx.Width=12;

	osanveCount = 0;

	fres = InitTextField(&_txtOsanveFormTitle, controlSize, controlPosition, FormatHeader, FALSE, _txtOsanveFormTitleTextBuff, controlBuffSizeTx, "Ваше осанве:");
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = 0;
	controlPosition.Left = 0;

	controlSize.Height = SCREENY;
	controlSize.Width = SCREENX;

	_pnlOsanveFormControls[0] = &_txtOsanveFormTitle;
	_pnlOsanveFormControls[1] = &_pnlStatusBar;
	fres = _pnlOsanveForm.Init(controlSize, controlPosition, &_renderer, _pnlOsanveFormControls, 2);
	if (fres!=SUCCESS)
		return fres;

	fres = InitOsanveFormMenu();
	return fres;
}

//Roman
//Select Consequence 
fresult UIDesigner::InitConsFormMenu()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	Size zeroSize;
	Position zeroPosition;

	zeroSize.data =0;
	zeroPosition.data =0;

	controlSize.Height = MENU_VERTICAL_GAP;
	controlSize.Width = 60;
	controlBuffSizeTx.Height=1;
	controlBuffSizeTx.Width=10;
	controlPosition.Left = 1;
// пункты меню выбора последствия
  for( int i = 0; i < (CONS_COUNT ); i++)
  {
 	  controlPosition.Top = i % 3 * (MENU_IMAGE_HEIGHT + MENU_VERTICAL_GAP) + STATUSBAR_HEIGHT_PX + MENU_VERTICAL_GAP;
 	  controlPosition.Left = i / 3 * 100;
	  fres = InitMenuItem(&_miCons[i], &_txtConsMenu[i], controlSize, controlPosition, FormatMenu, _bufConsMenu[i], controlBuffSizeTx, CONS[i], FormatMenu,
		       NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnConsMnuSelectH, i);
	  if (fres != SUCCESS)
		  return fres;
		_mnuConsMenuItems[i] = &_miCons[i];
  }
  
  // пункт отмена
  controlPosition.Top = 120;
  controlPosition.Left = 120;
  fres = InitMenuItem(&_miCons[CONS_COUNT], &_txtConsMenu[CONS_COUNT ], controlSize, controlPosition, FormatMenu, _bufConsMenu[CONS_COUNT ], controlBuffSizeTx, "Отмена", FormatMenu,
		       NULL, zeroSize, zeroPosition, NO_IMAGE, NULL, OnConsMnuCancelH, BUTTON_R );
  if (fres != SUCCESS)
		  return fres;
	_mnuConsMenuItems[CONS_COUNT ] = &_miCons[CONS_COUNT ];
	
	fres = _mnuConsMenu.Init(_mnuConsMenuItems, CONS_COUNT+1);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Форма выбора последствия
fresult UIDesigner::InitConsForm()
{
	fresult fres;

	Size controlSize;
	Position controlPosition;
	Size controlBuffSizeTx;

	controlPosition.Top = STATUSBAR_HEIGHT_PX;
	controlPosition.Left = MENU_IMAGE_WIDTH;

	controlSize.Height = MENU_VERTICAL_GAP;
	controlSize.Width = SCREENX - (controlPosition.Left*2);

	controlBuffSizeTx.Height=1;
	controlBuffSizeTx.Width=11;

	fres = InitTextField(&_txtConsFormTitle, controlSize, controlPosition, FormatHeader, FALSE, _txtConsFormTitleTextBuff, controlBuffSizeTx, "Последствие");
	if (fres!=SUCCESS)
		return fres;

	controlPosition.Top = 0;
	controlPosition.Left = 0;

	controlSize.Height = SCREENY;
	controlSize.Width = SCREENX;

	_pnlConsFormControls[0] = &_txtConsFormTitle;
	_pnlConsFormControls[1] = &_pnlStatusBar;
	fres = _pnlConsForm.Init(controlSize, controlPosition, &_renderer, _pnlConsFormControls,2);
	if (fres!=SUCCESS)
		return fres;

	fres = InitConsFormMenu();
	return fres;
}


// Roman
// Создание меню боевого режима
// Меню: L - назад в осанве E - листать R - Выбор последствия
// ABCXYZ - выбор противника
// Все меню текстовые, без картинок
fresult UIDesigner::InitBattleFormMnu()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;
	Size tfSize;
	Position zeroPos;
	Size zeroSize;

	zeroPos.data =0;
	zeroSize.data =0;

// Side Items
	controlSize.Width = SCREENX / 2;
	controlSize.Height = 2*MENU_VERTICAL_GAP;
	tfSize.Height = 2;
	tfSize.Width = 13;
// пункты меню выбора противника
  for( int i = 0; i < 6; i++)
  {
	  char* fname = ""; // если кнопке не досталось бойца
 	  if( fightersOnScreen[i] )
		  fname = CONF[fightersOnScreen[i]->userId].fname;
	  controlPosition.Top = i % 3 * (MENU_IMAGE_HEIGHT + MENU_VERTICAL_GAP) + STATUSBAR_HEIGHT_PX + MENU_VERTICAL_GAP;
 	  controlPosition.Left = i / 3 * 81;
	  fres = InitMenuItem(&_miFighters[i], &_txtFighters[i], controlSize, controlPosition, FormatMenu, _bufFighters[i], tfSize, 
	         fname, // берем боевое имя игрока из конфигурации по номеру в списке
	         FormatMenu, NULL, zeroSize, zeroPos, NO_IMAGE, NULL, OnFighterSelectH, i);
	  if (fres != SUCCESS)
		  return fres;
	  _mnuBattleMenuItems[i] = &_miFighters[i];
//	  fightersOnScreen[i] = NULL;
  }

	// Low Items
	controlSize.Width = SCREENX / 3;
	controlSize.Height = MENU_VERTICAL_GAP;
	tfSize.Height = 1;
	tfSize.Width = 9;

//////////L////////////////////
	controlPosition.Top = MENU_ITEM_L_TOP;
	controlPosition.Left = MENU_ITEM_L_LEFT;
	fres = InitMenuItem(&_menuOsanve, &_txtMenuOsanve, controlSize, controlPosition, FormatMenu, _bufMenuOsanve, tfSize, MENU_BATTLE_L, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnBattleMnuOsanveH, BUTTON_L);
	if (fres!=SUCCESS)
		return fres;
	_menuFight.SetSecondAccelarator(BUTTON_HOLD_L);

	//////////E////////////////////
	controlPosition.Top = MENU_ITEM_E_TOP;
	controlPosition.Left = MENU_ITEM_E_LEFT;
	fres = InitMenuItem(&_menuBattleList, &_txtMenuBattleList, controlSize, controlPosition, FormatMenu, _bufMenuBattleList, tfSize, MENU_BATTLE_E, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnBattleMnuListH, BUTTON_E);
	if (fres!=SUCCESS)
		return fres;
	_menuList.SetSecondAccelarator(BUTTON_HOLD_E);

	//////////R////////////////////
	controlPosition.Top = MENU_ITEM_R_TOP;
	controlPosition.Left = MENU_ITEM_R_LEFT;
	fres = InitMenuItem(&_menuConsSet, &_txtMenuConsSet, controlSize, controlPosition, FormatMenu, _bufMenuConsSet, tfSize, MENU_BATTLE_R, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnBattleMnuSetH, BUTTON_R);
	if (fres!=SUCCESS)
		return fres;
	_menuSet.SetSecondAccelarator(BUTTON_HOLD_R);


	//mnuBattleMenu
	_mnuBattleMenuItems[6] = &_menuOsanve;
	_mnuBattleMenuItems[7] = &_menuBattleList;
	_mnuBattleMenuItems[8] = &_menuConsSet;
	

	fres =_mnuBattleMenu.Init(_mnuBattleMenuItems, 9);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Настройка основного интерфейса: центральный список, статусбар, меню
fresult UIDesigner::InitBattleForm()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;
	Size controlBuffSizeTx;

	controlPosition.Top = STATUSBAR_HEIGHT_PX;
	controlPosition.Left = MENU_IMAGE_WIDTH;

	controlSize.Height = MENU_VERTICAL_GAP;
	controlSize.Width = SCREENX - controlPosition.Left*2;

	controlBuffSizeTx.Height=1;
	controlBuffSizeTx.Width=18;

	fres = InitTextField(&_txtBattleCap, controlSize, controlPosition, FormatHeader, FALSE, _bufBattleCap, controlBuffSizeTx, "Выбери противника");
	if (fres!=SUCCESS)
		return fres;


	//if (fres != SUCCESS)
	//	return fres;
	battleListPage = 0;
	//_pnlBattleForm
	controlPosition.Top = MAINFORM_TOP;
	controlPosition.Left = MAINFORM_LEFT;

	controlSize.Height = MAINFORM_HEIGHT;
	controlSize.Width = MAINFORM_WIDTH;

	fres = InitStatusBar();
	if (fres !=SUCCESS)
		return fres;

	_pnlBattleFormControls[0] = &_pnlStatusBar;
	_pnlBattleFormControls[1] = &_txtBattleCap;

	fres = _pnlBattleForm.Init(controlSize, controlPosition, &_renderer, _pnlBattleFormControls, 2);
	if (fres != SUCCESS)
		return fres;

	fres = InitBattleFormMnu();
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Создание меню сражения
// Меню: R - назад к выбору противников E - защита Y - Атака
// X - повышение атаки Z - понижение атаки
// Все меню текстовые, без картинок
fresult UIDesigner::InitFightFormMnu()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;

	Position zeroPos;
	Size zeroSize;
	zeroPos.data =0;
	zeroSize.data =0;

	//All Items
	controlSize.Height = MENU_VERTICAL_GAP;
	controlSize.Width = SCREENX / 3;

	Size tfSize;
	tfSize.Height = 1;
	tfSize.Width = 9;

//////////X////////////////////
	controlPosition.Top = MENU_ITEM_X_TOP;
	controlPosition.Left = MENU_ITEM_X_LEFT;
	fres = InitMenuItem(&_menuFightUP, &_txtMenuFightUP, controlSize, controlPosition, FormatMenu, _bufMenuFightUP, tfSize, MENU_FIGHT_X, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnFightMnuUPH, BUTTON_X);
	if (fres!=SUCCESS)
		return fres;
	_menuFight.SetSecondAccelarator(BUTTON_HOLD_X);

//////////Y////////////////////
	controlPosition.Top = MENU_ITEM_Y_TOP;
	controlPosition.Left = MENU_ITEM_Y_LEFT;
	fres = InitMenuItem(&_menuAtack, &_txtMenuAtack, controlSize, controlPosition, FormatMenu, _bufMenuAtack, tfSize, MENU_FIGHT_Y, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnFightMnuAtackH, BUTTON_Y);
	if (fres!=SUCCESS)
		return fres;
	_menuFight.SetSecondAccelarator(BUTTON_HOLD_Y);

//////////Z////////////////////
	controlPosition.Top = MENU_ITEM_Z_TOP;
	controlPosition.Left = MENU_ITEM_Z_LEFT;
	fres = InitMenuItem(&_menuFightDOWN, &_txtMenuFightDOWN, controlSize, controlPosition, FormatMenu, _bufMenuFightDOWN, tfSize, MENU_FIGHT_Z, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,
		NULL, OnFightMnuDOWNH, BUTTON_Z);
	if (fres!=SUCCESS)
		return fres;
	_menuFight.SetSecondAccelarator(BUTTON_HOLD_Z);

	//////////L////////////////////
	controlPosition.Top = MENU_ITEM_L_TOP;
	controlPosition.Left = MENU_ITEM_L_LEFT;
	fres = InitMenuItem(&_menuHeal, &_txtMenuHeal, controlSize, controlPosition, FormatMenu, _bufMenuHeal, tfSize, MENU_FIGHT_L, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,NULL, OnFightMnuHealH, BUTTON_L);
	if (fres!=SUCCESS)
		return fres;
	_menuSet.SetSecondAccelarator(BUTTON_HOLD_L);

	//////////E////////////////////
	controlPosition.Top = MENU_ITEM_E_TOP;
	controlPosition.Left = MENU_ITEM_E_LEFT;
	fres = InitMenuItem(&_menuDef, &_txtMenuDef, controlSize, controlPosition, FormatMenu, _bufMenuDef, tfSize, MENU_FIGHT_E, FormatMenu,
		NULL, zeroSize, zeroPos, NULL,NULL, OnFightMnuDefH, BUTTON_E);
	if (fres!=SUCCESS)
		return fres;
	_menuList.SetSecondAccelarator(BUTTON_HOLD_E);

	//////////R////////////////////
	controlPosition.Top = MENU_ITEM_R_TOP;
	controlPosition.Left = MENU_ITEM_R_LEFT;
	fres = InitMenuItem(&_menuBattle, &_txtMenuBattle, controlSize, controlPosition, FormatMenu, _bufMenuBattle, tfSize, MENU_FIGHT_R, FormatMenu,
		NULL, zeroSize, zeroPos, NULL, NULL, OnFightMnuBattleH, BUTTON_R);
	if (fres!=SUCCESS)
		return fres;
	_menuSet.SetSecondAccelarator(BUTTON_HOLD_R);

	currentFighter = NULL;

	//mnuFightMenu
	_mnuFightMenuItems[0] = &_menuFightUP;
	_mnuFightMenuItems[1] = &_menuFightDOWN;
	_mnuFightMenuItems[2] = &_menuAtack;
    _mnuFightMenuItems[3] = &_menuHeal;
	_mnuFightMenuItems[4] = &_menuDef;
	_mnuFightMenuItems[5] = &_menuBattle;
		

	fres =_mnuFightMenu.Init(_mnuFightMenuItems, 6);
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
}

// Roman
// Настройка основного интерфейса: центральный список, статусбар, меню
fresult UIDesigner::InitFightForm()
{
	fresult fres;
	Position controlPosition;
	Size controlSize;
	Size controlBuffSizeTx;

	//_txtFightField
	controlPosition.Top = DIALOG_TOP_PX;
	controlPosition.Left = DIALOG_LEFT_PX;

	controlSize.Height = DIALOG_HEIGHT_PX;
	controlSize.Width = SCREENX - (SCREENX/3); // справа у нас менюшечка

	controlBuffSizeTx.Width=FIGHT_CONTENT_BUFF_WIDTH;
	controlBuffSizeTx.Height=FIGHT_CONTENT_BUFF_HEIGHT;

	fres = InitTextField(&_txtFightField, controlSize, controlPosition, FormatText, TRUE, _bufFightField, controlBuffSizeTx, "");
	if (fres != SUCCESS)
		return fres;

	//_pnlFightForm
	controlPosition.Top = MAINFORM_TOP;
	controlPosition.Left = MAINFORM_LEFT;

	controlSize.Height = MAINFORM_HEIGHT;
	controlSize.Width = MAINFORM_WIDTH;

	fres = InitStatusBar();
	if (fres !=SUCCESS)
		return fres;

	_pnlFightFormControls[0] = &_txtFightField;
	_pnlFightFormControls[1] = &_pnlStatusBar;

	fres = _pnlFightForm.Init(controlSize, controlPosition, &_renderer, _pnlFightFormControls, 2);
	if (fres != SUCCESS)
		return fres;

	fres = InitFightFormMnu();
	if (fres != SUCCESS)
		return fres;

	return SUCCESS;
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
		&_imgLogFormMnuScrollUp, controlSize, controlPosition, BlueArrow2Up,
		NULL, OnLogFormMnuScrollUpH, BUTTON_B);
	if (fres!=SUCCESS)
		return fres;
	_miLogFormMnuScrollUp.SetSecondAccelarator(BUTTON_HOLD_B);

	//_miLogFormMnuScrollDown
	controlPosition.Top = MENU_ITEM_C_TOP;
	controlPosition.Left = MENU_ITEM_C_LEFT;

	fres = InitMenuItem(&_miLogFormMnuScrollDown, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuScrollDown, controlSize, controlPosition, BlueArrow2Down,
		NULL, OnLogFormMnuScrollDownH, BUTTON_C);
	if (fres!=SUCCESS)
		return fres;
	_miLogFormMnuScrollDown.SetSecondAccelarator(BUTTON_HOLD_C);

	//_miLogFormMnuPrev
	controlPosition.Top = MENU_ITEM_L_TOP;
	controlPosition.Left = MENU_ITEM_L_LEFT;
	fres = InitMenuItem(&_miLogFormMnuPrev, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuPrev, controlSize, controlPosition, BlueArrow2Left,
		NULL, OnLogFormMnuPrevH, BUTTON_L);
	if (fres!=SUCCESS)
		return fres;

	//INVIZILBLE
	//_miLogFormCleanLog
	controlPosition.Top = MENU_ITEM_A_TOP;
	controlPosition.Left = MENU_ITEM_A_LEFT;
	fres = InitMenuItem(&_miLogFormCleanLog, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormClean, controlSize, controlPosition, RedCancel,
		NULL, OnLogFormMnuCleanLogH, BUTTON_HOLD_A);
	if (fres!=SUCCESS)
		return fres;
	_imgLogFormClean.SetVisible(FALSE);

	//_miLogFormMnuBack
	controlPosition.Top = MENU_ITEM_Y_TOP;
	controlPosition.Left = MENU_ITEM_Y_LEFT;
	fres = InitMenuItem(&_miLogFormMnuBack, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuBack, controlSize, controlPosition, GreenOk,
		NULL, OnLogFormMnuBackH, BUTTON_Y);
	if (fres!=SUCCESS)
		return fres;
	_miLogFormMnuBack.SetSecondAccelarator(BUTTON_E);

	//_miLogFormMnuNext
	controlPosition.Top = MENU_ITEM_R_TOP;
	controlPosition.Left = MENU_ITEM_R_LEFT;
	fres = InitMenuItem(&_miLogFormMnuNext, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		&_imgLogFormMnuNext, controlSize, controlPosition, BlueArrow2Right,
		NULL, OnLogFormMnuNextH, BUTTON_R);
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


// Конструктор. Создает все формы
fresult UIDesigner::Init()
{
	fresult fres;
// Режим Осанве
	fres = InitMainForm();
	if (fres != SUCCESS)
		return fres;
	_frmMainForm.FormPanel = &_pnlMainForm;
	_frmMainForm.Menu = &_mnuMainMenu;
	_frmMainForm.BackgroundForm = NULL;
// режим выбора осанве
	fres = InitOsanveForm();
	if (fres != SUCCESS)
		return fres;
	_frmOsanveForm.FormPanel = &_pnlOsanveForm;
	_frmOsanveForm.Menu = &_mnuOsanveMenu;
	_frmOsanveForm.BackgroundForm = NULL;
// Режим выбора последствия
	fres = InitConsForm();
	if (fres != SUCCESS)
		return fres;
	_frmConsForm.FormPanel = &_pnlConsForm;
	_frmConsForm.Menu = &_mnuConsMenu;
	_frmConsForm.BackgroundForm = NULL;
// Режим битвы
	fres = InitBattleForm();
	if (fres != SUCCESS)
		return fres;
	_frmBattleForm.FormPanel = &_pnlBattleForm;
	_frmBattleForm.Menu = &_mnuBattleMenu;
	_frmBattleForm.BackgroundForm = NULL;
// Режим боя
	fres = InitFightForm();
	if (fres != SUCCESS)
		return fres;
	_frmFightForm.FormPanel = &_pnlFightForm;
	_frmFightForm.Menu = &_mnuFightMenu;
	_frmFightForm.BackgroundForm = NULL;


	fres = InitLogForm();
	if (fres != SUCCESS)
		return fres;
	_frmLogForm.FormPanel = &_pnlLogForm;
	_frmLogForm.Menu = &_mnuLogFormMenu;
	_frmLogForm.BackgroundForm = NULL;

	fres = InitMsgBoxForm();
	if (fres != SUCCESS)
		return fres;
	_frmMsgBox.FormPanel = &_pnlMsgBox;
	_frmMsgBox.Menu = &_mnuMsgBoxMenu;
	_frmMsgBox.BackgroundForm = NULL;

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

// Обновление индикатора силы
fresult UIDesigner::SetForces(void)
{
	char tmp[8];
	sprintf( tmp,"%d/%d", Player.force, Player.maxForce);
	return txtForce.SetText(tmp);
}

// обновить список видимых осанве-побратимов
fresult UIDesigner::SetOsanve(void)
{
	int t = ArmletApi::GetUpTime();
	char tmp[MSGBOX_CONTENT_BUFF_WIDTH*MSGBOX_CONTENT_BUFF_HEIGHT*MSGBOX_CONTENT_BUFF_PAGES];
	int i = 0;
	Position pos;
	if( Player.status != AL_STATUS_OSANVE) // только если список отображается
		return SUCCESS;
	pos = _txtOsanveList.GetScrollPosition();
//	_txtOsanveList.Clear();
	osanveCount = 0;
	memset( tmp, 0, sizeof(tmp));
	// ищем первого противника для отображения
	for(i = 0; i < MAX_ARMLET; ++i)
	{ // непустой, недавно обновился, из нашей группы
		if( Player.userId != CONF[i].userId && FGHT[i].userId != 0 && (t - FGHT[i].time) < OSANVE_MAX_WAIT_TIME && Player.groupId == CONF[i].groupId)
		{ 
			char s[MSGBOX_CONTENT_BUFF_WIDTH];
			sprintf( s, "%s-%s", CONF[i].name, OSANVES[FGHT[i].osanve]);
			int slen = strlen(s);
			for( int j = 0; j < MSGBOX_CONTENT_BUFF_WIDTH; ++j) // копируем сформированную строку
		      if( j<slen)
				tmp[(osanveCount)*MSGBOX_CONTENT_BUFF_WIDTH+j] = s[j];
			  else
				tmp[(osanveCount)*MSGBOX_CONTENT_BUFF_WIDTH+j] = ' ';
			/*_txtOsanveList.AppendText(CONF[i].name);
			_txtOsanveList.AppendText("-");
			_txtOsanveList.AppendText(OSANVES[FGHT[i].osanve]);*/
			osanveCount++;
		}
		if( osanveCount >= MSGBOX_CONTENT_BUFF_HEIGHT*MSGBOX_CONTENT_BUFF_PAGES) //(osanveCount*MSGBOX_CONTENT_BUFF_HEIGHT))
			break; // если виртуально заполнили пролистанные страницы, остановимся
	}
	if(osanveCount>0) // предосторожность, если использовали весь список
	  tmp[(osanveCount)*MSGBOX_CONTENT_BUFF_WIDTH-1] = '\0';
	_txtOsanveList.SetText(tmp); // не уверен, не нужно ли SetText???
	if(osanveCount<=MSGBOX_CONTENT_BUFF_HEIGHT)
	{
		Position zero = { 0 };
		return _txtOsanveList.SetScrollPosition(zero);
	}
	_txtOsanveList.SetScrollPosition(pos);
	_txtOsanveList.Draw(); // RedrawIfForeground(&_frmMainForm);  
	return SUCCESS;
}

// Заполнить список доступных бойцов на экране
fresult UIDesigner::SetBattle(void)
{
	ubyte_t fcount = 0;
	int t = ArmletApi::GetUpTime();
	int i = 0;
	// Очистим менюшки - по другому не придумал :(
	for( i = 0; i<6; ++i){
	  fightersOnScreen[i]=NULL; // ссылки на противников
	  _txtFighters[i].SetText( ""); // сотрем менюшку
	}
	// ищем первого противника для отображения
	for(i = 0; i < MAX_ARMLET; ++i)
	{
		if( FGHT[i].maxForce > 0 && (t - FGHT[i].time) < FIGHT_MAX_WAIT_TIME)
		{ // ячейка не пуста
			fcount++;
		}
		if( fcount > (battleListPage*6))
			break; // если виртуально заполнили пролистанные страницы, остановимся
	}
	// назначим новую отображаемую страницу (для случая сокращения списка)
	battleListPage = fcount / 6;
	fcount = 0;
	// заполняем менюшку с игроками
	for(i = 0; i < MAX_ARMLET; ++i)
	{
	  int item = fcount % 6;
	  if( FGHT[i].maxForce == 0 || (t - FGHT[i].time) >= FIGHT_MAX_WAIT_TIME)
		{ continue;
		}
		fcount++;
		if( fcount <= (battleListPage*6))
			continue; // не дошли до страницы
		if( fcount > ((battleListPage+1)*6))
			break; // всех занесли, кого надо было
	  // рисуем новые имена
		char tmp[32];
		fightersOnScreen[item]=&FGHT[i];
		sprintf( tmp, "%s\n%d/%d", CONF[i].fname, FGHT[i].force,FGHT[i].maxForce);
		tmp[31] = '\0';
	    _txtFighters[item].SetText( tmp);
	}
	return SUCCESS;
}

// Обновить форму боя
fresult UIDesigner::SetFightField(void)
{
  char tmp[FIGHT_CONTENT_BUFF_WIDTH*FIGHT_CONTENT_BUFF_HEIGHT];
  if( Player.enemyId <=0 || Player.enemyId >= MAX_ARMLET || !currentFighter)
	return GENERAL_WARNING;

  sprintf( tmp, "Бой с\n%s:\n%d/%d\n\nВаши силы:\n%d/%d\n\nАтакуем силой:\n%d", CONF[currentFighter->userId].fname,
	  currentFighter->force,currentFighter->maxForce,Player.force,Player.maxForce,Player.atack);
  return _txtFightField.SetText( tmp);
}

// Roman
// Show message
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
	if( Player.status == AL_STATUS_DEFEAT && Player.defeatTime > 0)
		return SUCCESS; // проверка на использование в режиме поражения (запрет закрытия)
	return CloseForm();
}

//Roman
//text field
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

// Roman
// Image
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
		/*fres = GetImageById(pictureId, &bitmap, &bitmapSize);
		if (fres !=SUCCESS)
			return fres;*/

	}

	return SUCCESS;
}


// Roman
// Image or text menu item
fresult UIDesigner::InitMenuItem(MenuItem* mi,
								 TextField* tf, Size tfControlSize, Position tfControlPosition, ubyte_t textFormatId, char* buff, Size buffSize, const char* text, ubyte_t selTextFormatId,
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

fresult UIDesigner::ShowForm( Form* form )
{
	form->BackgroundForm = currentForm;
	currentForm = form;
	Draw();
	return SUCCESS;
}

fresult UIDesigner::CloseForm()
{
	Form* curForm = currentForm;
	currentForm = currentForm->BackgroundForm;
	curForm ->BackgroundForm = NULL;
	Draw();
	return SUCCESS;
}


// Roman
// Add message to the log
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
		icon = BlueRss1;
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

// Roman
// Button handler
fresult UIDesigner::OnButtonPressed( ButtonState button )
{
	return SUCCESS;
}

// Roman
// Handlers functions
// Osanve -> Battle
fresult UIDesigner::OnMainMnuFight(IMenuItem* sender)
{	// To the BATTLE MODE
	Player.status = AL_STATUS_COMBAT;
	SetBattle();
	ShowForm( &_frmBattleForm);
	return SUCCESS;
}

fresult	UIDesigner::OnMainMnuList(IMenuItem* sender)
{
	Position cur = _txtOsanveList.GetScrollPosition();
	if( cur.Top >= (osanveCount-MSGBOX_CONTENT_BUFF_HEIGHT))
	{
		Position zero = { 0 };
		return _txtOsanveList.SetScrollPosition(zero);
	}
	return _txtOsanveList.ScrollDown();
}

// Osanve -> Osanve set
fresult UIDesigner::OnMainMnuSet( IMenuItem* sender )
{
	Player.status = AL_STATUS_OSSEL;
	
	return ShowForm(&_frmOsanveForm);
}

// Osanve set -> Osanve
fresult UIDesigner::OnOsanveMnuSelect( IMenuItem* sender )
{
	int i = 0;
	for( i = 0; i<OSANVE_COUNT; i++)
	  if( sender == _mnuOsanveMenuItems[i])
	    break;
	if( i < OSANVE_COUNT)
	  SetOsanveNum( i);
	Player.status = AL_STATUS_OSANVE;
	// Обновим в статусе
	txtOsanve.SetText( OSANVES[i]);
	return ShowForm(&_frmMainForm);
}

// Osanve set cancel -> Osanve
fresult UIDesigner::OnOsanveMnuCancel( IMenuItem* sender )
{
	Player.status = AL_STATUS_OSANVE;
	
	return ShowForm(&_frmMainForm);
}

// Cons set -> Combat mode
fresult UIDesigner::OnConsMnuSelect( IMenuItem* sender )
{
	int i = 0;
	for( i = 0; i<CONS_COUNT; i++)
	  if( sender == _mnuConsMenuItems[i])
	    break;
	if( i < CONS_COUNT)
	  Player.cons = ubyte_t(i);
	Player.status = AL_STATUS_COMBAT;
	ShowForm(&_frmBattleForm);
	return SUCCESS;
}

// Cons set cancel -> Combat mode
fresult UIDesigner::OnConsMnuCancel( IMenuItem* sender )
{
	sender = sender;
	Player.status = AL_STATUS_COMBAT;
	ShowForm(&_frmBattleForm);
	return SUCCESS;
}

// Combat mode -> Cons set
fresult UIDesigner::OnBattleMnuSet( IMenuItem* sender )
{
	Player.status = AL_STATUS_CONSEL;
	ShowForm(&_frmConsForm);
	return SUCCESS;
}

// Combat mode listing
fresult UIDesigner::OnBattleMnuList( IMenuItem* sender )
{
	ubyte_t oldPage = battleListPage++; // cтарая траница списка
	SetBattle();
	if(battleListPage<=oldPage){ // SetBattle вернула уменьшенную страницу - сбрасываем её в 0
		battleListPage = 0;
		SetBattle();
	}
	return  _frmBattleForm.Menu->Draw();
}

// Combat mode -> Osanve
fresult UIDesigner::OnBattleMnuOsanve( IMenuItem* sender )
{
	Player.status = AL_STATUS_OSANVE;
	ShowForm(&_frmMainForm);
	return SUCCESS;
}

// Combat mode -> Fight mode (with selected fighter)
fresult UIDesigner::OnFighterSelect( IMenuItem* sender )
{ // Определим кого выбрали (все 6 боковых кнопок вызывают эту функцию)
	unsigned char a_packet[4];
	int i=0;
	for( i=0; i < 6; i++)
	  if( _mnuBattleMenuItems[i] == sender)
		break;
	if( i>= 6) // неверный указатель на меню
	  return GENERAL_WARNING;
	if( fightersOnScreen[i]==NULL) // пустое место в меню (например поледняя страница списка бойцов)
	  return SUCCESS;
	Player.enemyId = fightersOnScreen[i]->userId;
	Player.status = AL_STATUS_FIGHT;
	currentFighter = fightersOnScreen[i];
	// Сразу посылка нулевой атаки
	GetAtackPacket( a_packet, 0);
	ArmletApi::SendRadioPacket( a_packet, 4);
	// отображение формы атаки
	SetFightField();
	ShowForm(&_frmFightForm);
	return SUCCESS;
}

// Fight mode atack up
fresult UIDesigner::OnFightMnuUP( IMenuItem* sender )
{
	if( (Player.atack + FORCE_STEP) > Player.force)
	  Player.atack = Player.force;
	else
	 Player.atack += FORCE_STEP;
	SetFightField(); //currentForm->Menu->Draw();
	return SUCCESS;
}

// Fight mode atack down
fresult UIDesigner::OnFightMnuDOWN( IMenuItem* sender )
{
	if( (Player.atack - FORCE_STEP) < 0) {
//		if( Player.force >= 0)
			Player.atack = 0;
		//else
	 //       Player.atack = Player.force;
	} else
	  Player.atack -= FORCE_STEP;
	SetFightField(); ///currentForm->Menu->Draw();
	return SUCCESS;
}

// Fight mode do atack
fresult UIDesigner::OnFightMnuAtack( IMenuItem* sender )
{
	unsigned char a_packet[4];
	struct ATACK_PACKET* ap = (struct ATACK_PACKET*)a_packet;
	GetAtackPacket( a_packet, Player.atack);
	ArmletApi::SendRadioPacket( a_packet, 4);
	Atack( ap->atack); // списание силы
	SetFightField();
	return Draw();
}

// Fight mode do heal
fresult UIDesigner::OnFightMnuHeal( IMenuItem* sender )
{
	unsigned char h_packet[4];
	struct ATACK_PACKET* hp = (struct ATACK_PACKET*)h_packet;
	GetHealPacket( h_packet, Player.atack);
	ArmletApi::SendRadioPacket( h_packet, 4);
	Atack( hp->atack); // списание силы
	SetFightField();
	return Draw();
}

// Fight mode do defense
fresult UIDesigner::OnFightMnuDefence( IMenuItem* sender )
{ // пока выход из защиты по повторному нажатию
	if( Player.status  == AL_STATUS_DEFENSE){
		Player.status  = AL_STATUS_FIGHT;
		ShowForm(&_frmFightForm);
	} else {
	  Player.status  = AL_STATUS_DEFENSE; // здесь не должно быть пенальти - переключение из боевой формы
	  MessageBoxShow( "ЗАЩИТА!", "Сейчас вы получаете в 10 раз меньше вреда от всех атак", NO_IMAGE );
	  //ShowForm(&_frmDefenseForm);
	}
	return SUCCESS;
}

// Fight mode -> battle mode
fresult UIDesigner::OnFightMnuBattle( IMenuItem* sender )
{
  Player.status  = AL_STATUS_COMBAT;
  Player.enemyId = 0;
  SetFightField();
  currentFighter = NULL;
  return ShowForm(&_frmBattleForm);
}

// Форма МессаджБокса используется для режима защиты
fresult UIDesigner::OnMsgBoxMnuOk( IMenuItem* sender )
{
	fresult rval = MessageBoxClose();
	if( Player.status  == AL_STATUS_DEFENSE) {
	  DefenceOFF();
	  SetFightField();
	  rval = ShowForm(&_frmFightForm);
	}
	return rval;
}

fresult UIDesigner::OnMsgBoxMnuScrollUp( IMenuItem* sender )
{
	return _txtMessageBoxContent.ScrollUp();
}

fresult UIDesigner::OnMsgBoxMnuScrollDown( IMenuItem* sender )
{
	return _txtMessageBoxContent.ScrollDown();
}

//fresult UIDesigner::OnWoundMnuOk( IMenuItem* sender )
//{
//	txtWoundResult.Clear();
//	return CloseForm();
//}

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

//Roman
//MessageBox
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

	fres = InitImage(&_imgMessageBoxIcon, controlSize, controlPosition, OrangeTarget);
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
	controlPosition.Top  = SCREENY-MENU_IMAGE_HEIGHT;
	controlPosition.Left = (SCREENX-MENU_IMAGE_WIDTH)/2;

	controlSize.Height  = MENU_IMAGE_HEIGHT;
	controlSize.Width = MENU_IMAGE_WIDTH;
	fres = InitMenuItem(&_miMsgBoxMnuOk, 
							NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
							&_imgMsgBoxMnuOk, controlSize, controlPosition, GreenOk,
							NULL, OnMsgBoxMnuOkH, BUTTON_X);
	if (fres != SUCCESS)
		return fres;
	_miMsgBoxMnuOk.SetSecondAccelarator(BUTTON_E);

	//_miMsgBoxScrollUp
	fres = InitMenuItem(&_miMsgBoxMnuScrollUp, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		NULL, zeroSize, zeroPos, NO_IMAGE,
		NULL, OnMsgBoxMnuScrollUpH, BUTTON_B);
	
	if (fres != SUCCESS)
		return fres;
	_miMsgBoxMnuScrollUp.SetSecondAccelarator(BUTTON_HOLD_B);

	//_miMsgBoxScrollUp
	fres = InitMenuItem(&_miMsgBoxMnuScrollDown, 
		NULL, zeroSize, zeroPos, NULL, NULL, zeroSize, NULL, NULL,
		NULL, zeroSize, zeroPos, NO_IMAGE,
		NULL, OnMsgBoxMnuScrollDownH, BUTTON_C);
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

