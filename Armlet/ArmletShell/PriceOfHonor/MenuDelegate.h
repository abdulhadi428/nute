#pragma once
#include "UIDesigner.h"
#include "IMenuHandler.h"

typedef enum _ON_MNU {
	Empty,
	OnMainMnuHelpHandler,
	OnMainMnuScrollUpHandler,
	OnMainMnuScrollDownHandler,
	OnMainMnuCycleLeftHandler,
	OnMainMnuShotHandler,
	OnMainMnuKnockoutHandler,
	OnMainMnuOpenLockHandler,
	OnMainMnuCycleRightHandler,
	OnMsgBoxMnuOkHandler,		 
	OnMsgBoxMnuScrollUpHandler,
	OnMsgBoxMnuScrollDownHandler,
	OnWoundMnuHeadHandler,
	OnWoundMnuStomachHandler,
	OnWoundMnuLeftArmHandler,
	OnWoundMnuRightArmHandler,
	OnWoundMnuChestHandler,
	OnWoundMnuBackHandler,
	OnWoundMnuLeftLegHandler,
	OnWoundMnuRightLegHandler,
	OnWoundMnuOkHandler,
	Last
} MenuHandlers;

class MenuDelegate : public IMenuHandler
{
	UIDesigner* _ui;
	MenuHandlers _handler;
public:
	fresult Init(MenuHandlers handler, UIDesigner* ui);
	fresult virtual OnClick( IMenuItem* sender);
};


extern MenuDelegate g_MenuHandlers[Last];

void InitMenuHandlerDelegates(UIDesigner* ui);

IMenuHandler* GetMenuHandler(MenuHandlers handler);