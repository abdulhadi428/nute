#pragma once
#include "UIDesigner.h"
#include "IMenuHandler.h"

typedef enum _ON_MNU {
	Empty,
	OnMainMnuFight,
	OnMainMnuList,
	OnMainMnuSet,
	OnOsanveMnuCancel,
	OnOsanveMnuSelect,
	OnConsMnuCancel,		 
	OnConsMnuSelect,
	OnBattleMnuOsanve,
	OnBattleMnuList,
	OnBattleMnuSet,
	OnBattleMnuSelect,
	OnFighterSelect,
	OnFightMnuUP,
	OnFightMnuAtack,
	OnFightMnuDOWN,
	OnFightMnuDefence,
	OnFightMnuBattle,
	OnFightMnuHeal,
	OnLogFormMnuScrollUp,
	OnLogFormMnuScrollDown,
	OnLogFormMnuPrev,
	OnLogFormMnuNext,
	OnLogFormMnuBack,
	OnLogFormMnuCleanLog,
	OnMsgBoxMnuOk,
	OnMsgBoxMnuScrollUp,
	OnMsgBoxMnuScrollDown,
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
