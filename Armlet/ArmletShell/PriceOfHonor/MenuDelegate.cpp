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
	case OnMainMnuHelpHandler:
		fres = _ui->OnMainMnuHelp(sender);
		break;
	case OnMainMnuScrollUpHandler:
		fres = _ui->OnMainMnuScrollUp(sender);
		break;
	case OnMainMnuScrollDownHandler:
		fres = _ui->OnMainMnuScrollDown(sender);
		break;
	case OnMainMnuCycleLeftHandler:
		fres = _ui->OnMainMnuCycleLeft(sender);
		break;
	case OnMainMnuShotHandler:
		fres = _ui->OnMainMnuShot(sender);
		break;
	case OnMainMnuKnockoutHandler:
		fres = _ui->OnMainMnuKnockout(sender);
		break;
	case OnMainMnuOpenLockHandler:
		fres = _ui->OnMainMnuOpenLock(sender);
		break;
	case OnMainMnuCycleRightHandler:
		fres = _ui->OnMainMnuCycleRight(sender);
		break;
	case OnMainMnuShowLogsHandler:
		fres = _ui->OnMainMnuShowLogs(sender);
		break;
	case OnMsgBoxMnuOkHandler:		 
		fres = _ui->OnMsgBoxMnuOk(sender);
		break;
	case OnMsgBoxMnuScrollUpHandler:
		fres = _ui->OnMsgBoxMnuScrollUp(sender);
		break;
	case OnMsgBoxMnuScrollDownHandler:
		fres = _ui->OnMsgBoxMnuScrollDown(sender);
		break;
	case OnWoundMnuHeadHandler:
		fres = _ui->OnWoundMnuHead(sender);
		break;
	case OnWoundMnuStomachHandler:
		fres = _ui->OnWoundMnuStomach(sender);
		break;
	case OnWoundMnuLeftArmHandler:
		fres = _ui->OnWoundMnuLeftArm(sender);
		break;
	case OnWoundMnuRightArmHandler:
		fres = _ui->OnWoundMnuRightArm(sender);
		break;
	case OnWoundMnuChestHandler:
		fres = _ui->OnWoundMnuChest(sender);
		break;
	case OnWoundMnuBackHandler:
		fres = _ui->OnWoundMnuBack(sender);
		break;
	case OnWoundMnuLeftLegHandler:
		fres = _ui->OnWoundMnuLeftLeg(sender);
		break;
	case OnWoundMnuRightLegHandler:
		fres = _ui->OnWoundMnuRightLeg(sender);
		break;
	case OnWoundMnuOkHandler:
		fres = _ui->OnWoundMnuOk(sender);
		break;
	case OnLogFormMnuScrollUpHandler:
		fres = _ui->OnLogFormMnuScrollUp(sender);
		break;
	case OnLogFormMnuScrollDownHandler:
		fres = _ui->OnLogFormMnuScrollDown(sender);
		break;
	case OnLogFormMnuPrevHandler:
		fres = _ui->OnLogFormMnuPrev(sender);
		break;
	case OnLogFormMnuNextHandler:
		fres = _ui->OnLogFormMnuNext(sender);
		break;
	case OnLogFormMnuBackHandler:
		fres = _ui->OnLogFormMnuBack(sender);
		break;
	case OnLogFormMnuCleanLogHandler:
		fres = _ui->OnLogFormMnuCleanLog(sender);
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
	
	switch (handler)
	{
		case Empty:
		case OnMainMnuHelpHandler:
		case OnMainMnuScrollUpHandler:
		case OnMainMnuScrollDownHandler:
		case OnMainMnuCycleLeftHandler:
		case OnMainMnuShotHandler:
		case OnMainMnuKnockoutHandler:
		case OnMainMnuOpenLockHandler:
		case OnMainMnuCycleRightHandler:
		case OnMainMnuShowLogsHandler:
		case OnMsgBoxMnuOkHandler:		 
		case OnMsgBoxMnuScrollUpHandler:
		case OnMsgBoxMnuScrollDownHandler:
		case OnWoundMnuHeadHandler:
		case OnWoundMnuStomachHandler:
		case OnWoundMnuLeftArmHandler:
		case OnWoundMnuRightArmHandler:
		case OnWoundMnuChestHandler:
		case OnWoundMnuBackHandler:
		case OnWoundMnuLeftLegHandler:
		case OnWoundMnuRightLegHandler:
		case OnWoundMnuOkHandler:
		case OnLogFormMnuScrollUpHandler:
		case OnLogFormMnuScrollDownHandler:
		case OnLogFormMnuPrevHandler:
		case OnLogFormMnuNextHandler:
		case OnLogFormMnuBackHandler:
		case OnLogFormMnuCleanLogHandler:
			_handler =handler;
			fres = SUCCESS;
			break;
		default:
			_handler = Empty;
			fres = GENERAL_ERROR;
	}

	return fres;
}

MenuDelegate g_MenuHandlers[Last];

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
