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
	case OnMainMnuFightH:
		fres = _ui->OnMainMnuFight(sender);
		break;
	case OnMainMnuListH:
		fres = _ui->OnMainMnuList(sender);
		break;
	case OnMainMnuSetH:
		fres = _ui->OnMainMnuSet(sender);
		break;
	case OnOsanveMnuSelectH:
		fres = _ui->OnOsanveMnuSelect(sender);
		break;
	case OnOsanveMnuCancelH:
		fres = _ui->OnOsanveMnuCancel(sender);
		break;
	case OnConsMnuSelectH:		 
		fres = _ui->OnConsMnuSelect(sender);
		break;
	case OnConsMnuCancelH:
		fres = _ui->OnConsMnuCancel(sender);
		break;
	case OnBattleMnuOsanveH:
		fres = _ui->OnBattleMnuOsanve(sender);
		break;
	case OnBattleMnuListH:
		fres = _ui->OnBattleMnuList(sender);
		break;
	case OnBattleMnuSetH:
		fres = _ui->OnBattleMnuSet(sender);
		break;
	case OnFighterSelectH:
		fres = _ui->OnFighterSelect(sender);
		break;
	case OnFightMnuUPH:
		fres = _ui->OnFightMnuUP(sender);
		break;
	case OnFightMnuAtackH:
		fres = _ui->OnFightMnuAtack(sender);
		break;
	case OnFightMnuDOWNH:
		fres = _ui->OnFightMnuDOWN(sender);
		break;
	case OnFightMnuDefH:
		fres = _ui->OnFightMnuDefence(sender);
		break;
	case OnFightMnuBattleH:
		fres = _ui->OnFightMnuBattle(sender);
		break;
	case OnFightMnuHealH:
		fres = _ui->OnFightMnuHeal(sender);
		break;

	case OnLogFormMnuScrollUpH:
		fres = _ui->OnLogFormMnuScrollUp(sender);
		break;
	case OnLogFormMnuScrollDownH:
		fres = _ui->OnLogFormMnuScrollDown(sender);
		break;
	case OnLogFormMnuPrevH:
		fres = _ui->OnLogFormMnuPrev(sender);
		break;
	case OnLogFormMnuBackH:
		fres = _ui->OnLogFormMnuBack(sender);
		break;
	case OnLogFormMnuCleanLogH:
		fres = _ui->OnLogFormMnuCleanLog(sender);
		break;
	case OnLogFormMnuNextH:
		fres = _ui->OnLogFormMnuNext(sender);
		break;

	case OnMsgBoxMnuOkH:
		fres = _ui->OnMsgBoxMnuOk(sender);
		break;
	case OnMsgBoxMnuScrollUpH:
		fres = _ui->OnMsgBoxMnuScrollUp(sender);
		break;
	case OnMsgBoxMnuScrollDownH:
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
