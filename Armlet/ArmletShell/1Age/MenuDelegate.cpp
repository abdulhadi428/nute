#include "ArmletShell.h"
#include "MenuDelegate.h"

fresult MenuDelegate::OnClick( IMenuItem* sender )
{
	fresult fres = GENERAL_ERROR;
	switch (_handler)
	{
	case Empty:
		fres = SUCCESS;
		break;
	case OnMainMnuFight:
		fres = _ui->OnMainMnuFight(sender);
		break;
	case OnMainMnuList:
		fres = _ui->OnMainMnuList(sender);
		break;
	case OnMainMnuSet:
		fres = _ui->OnMainMnuSet(sender);
		break;
	case OnOsanveMnuSelect:
		fres = _ui->OnOsanveMnuSelect(sender);
		break;
	case OnOsanveMnuCancel:
		fres = _ui->OnOsanveMnuCancel(sender);
		break;
	case OnConsMnuSelect:		 
		fres = _ui->OnConsMnuSelect(sender);
		break;
	case OnConsMnuCancel:
		fres = _ui->OnConsMnuCancel(sender);
		break;
	case OnBattleMnuOsanve:
		fres = _ui->OnBattleMnuOsanve(sender);
		break;
	case OnBattleMnuList:
		fres = _ui->OnBattleMnuList(sender);
		break;
	case OnBattleMnuSet:
		fres = _ui->OnBattleMnuSet(sender);
		break;
	case OnBattleMnuSelect:
		fres = _ui->OnBattleMnuSelect(sender);
		break;
	case OnFighterSelect:
		fres = _ui->OnFighterSelect(sender);
		break;
	case OnFightMnuUP:
		fres = _ui->OnFightMnuUP(sender);
		break;
	case OnFightMnuAtack:
		fres = _ui->OnFightMnuAtack(sender);
		break;
	case OnFightMnuDOWN:
		fres = _ui->OnFightMnuDOWN(sender);
		break;
	case OnFightMnuDef:
		fres = _ui->OnFightMnuDef(sender);
		break;
	case OnFightMnuBattle:
		fres = _ui->OnFightMnuBattle(sender);
		break;
	case OnFightMnuHeal:
		fres = _ui->OnFightMnuHeal(sender);
		break;

	case OnLogFormMnuScrollUp:
		fres = _ui->OnLogFormMnuScrollUp(sender);
		break;
	case OnLogFormMnuScrollDown:
		fres = _ui->OnLogFormMnuScrollDown(sender);
		break;
	case OnLogFormMnuPrev:
		fres = _ui->OnLogFormMnuPrev(sender);
		break;
	case OnLogFormMnuBack:
		fres = _ui->OnLogFormMnuBack(sender);
		break;
	case OnLogFormMnuCleanLog:
		fres = _ui->OnLogFormMnuCleanLog(sender);
		break;
	case OnLogFormMnuNext:
		fres = _ui->OnLogFormMnuNext(sender);
		break;

	case OnMsgBoxMnuOk:
		fres = _ui->OnMsgBoxMnuOk(sender);
		break;
	case OnMsgBoxMnuScrollUp:
		fres = _ui->OnMsgBoxMnuScrollUp(sender);
		break;
	case OnMsgBoxMnuScrollDown:
		fres = _ui->OnMsgBoxMnuScrollDown(sender);
		break;
	default:
		fres = GENERAL_ERROR;
	}
	return fres;
}

fresult MenuDelegate::Init(MenuHandlers handler, UIDesigner* ui)
{
	fresult fres = GENERAL_ERROR;	
	if (ui == NULL)
		return GENERAL_ERROR;

	_ui = ui;
	
	if (handler > Empty && handler < Last)
	{
			_handler =handler;
			fres = SUCCESS;
  } else {
  		_handler = Empty;
			fres = GENERAL_ERROR;
	}

	return fres;
}

void InitMenuHandlerDelegates( UIDesigner* ui )
{
	for (ubyte_t i=0;i< Last; i++)
	{
		g_MenuHandlers[i].Init((MenuHandlers)i, ui);
	}
}

IMenuHandler* GetMenuHandler( MenuHandlers handler )
{
	return &g_MenuHandlers[handler];
}

MenuDelegate g_MenuHandlers[Last];
