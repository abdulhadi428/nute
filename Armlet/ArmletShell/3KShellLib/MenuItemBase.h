#pragma once

#include "project.h"
#include "IMenuItem.h"

class MenuItemBase : public  IMenuItem
{
protected:
	IControl* _underLyningControl;
	bool_t _IsSelected;

	PMY_CALLBACK _OnClick;
	int keyBinding;

	fresult TextMenuItemBaseInit(IControl* _control)
	{
		_underLyningControl = _control;
		return SUCCESS;
	}

public:

	fresult virtual Draw();
	bool_t virtual IsSelected();
	fresult virtual Select();
	fresult virtual Deselect();
	fresult virtual Click();
};