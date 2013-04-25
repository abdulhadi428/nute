#include "MenuItemBase.h"

bool_t MenuItemBase::IsSelected()
{
	return _IsSelected;
}

fresult MenuItemBase::Select()
{
	_IsSelected = TRUE;
	return SUCCESS;
}

fresult MenuItemBase::Deselect()
{
	_IsSelected = TRUE;
	return SUCCESS;
}

fresult MenuItemBase::Click()
{
	return SUCCESS;
}


fresult MenuItemBase::Draw()
{
	return _underLyningControl->Draw();
}

bool MenuItemBase::OnAccelerator( uword_t button )
{
	if (button==keyBinding)
	{
		Click();
		return TRUE;
	}
	return FALSE;
}
