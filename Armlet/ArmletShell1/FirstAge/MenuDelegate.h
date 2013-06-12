#pragma once
#include "UIDesigner.h"
#include "IMenuHandler.h"

typedef enum _ON_MNU {
	Empty,
	OnMainMnuFightH,
	OnMainMnuListH,
	OnMainMnuSetH,
	OnOsanveMnuCancelH,
	OnOsanveMnuSelectH,
	OnConsMnuCancelH,	 
	OnConsMnuSelectH,
	OnBattleMnuOsanveH,
	OnBattleMnuListH,
	OnBattleMnuSetH,
	OnFighterSelectH,
	OnFightMnuUPH,
	OnFightMnuAtackH,
	OnFightMnuDOWNH,
	OnFightMnuDefH,
	OnFightMnuBattleH,
	OnFightMnuHealH,
	OnLogFormMnuScrollUpH,
	OnLogFormMnuScrollDownH,
	OnLogFormMnuPrevH,
	OnLogFormMnuNextH,
	OnLogFormMnuBackH,
	OnLogFormMnuCleanLogH,
	OnMsgBoxMnuOkH,
	OnMsgBoxMnuScrollUpH,
	OnMsgBoxMnuScrollDownH,
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
