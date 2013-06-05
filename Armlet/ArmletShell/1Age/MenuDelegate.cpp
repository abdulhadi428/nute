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
	case OnOsanveSelectHandler:
		fres = _ui->OnOsanveSelectHandler(sender);
		break;
	case OnOsanveCancelHandler:
		fres = _ui->OnOsanveCancelHandler(sender);
		break;
	case OnConsSelectHandler:		 
		fres = _ui->OnConsSelectHandler(sender);
		break;
	case OnConsCancelHandler:
		fres = _ui->OnConsCancelHandler(sender);
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
	case OnBattleMnuEnemy:
		fres = _ui->OnBattleMnuEnemy(sender);
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
