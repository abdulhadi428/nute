#include "ArmletShell.h"
#include "MenuItemBase.h"

namespace ThreeKShell {

//typedef fresult (__thiscall MenuItemBase::* MENU_IMET_BASE_BOOL_PTR_VOID)(void);
//MENU_IMET_BASE_BOOL_PTR_VOID func = &MenuItemBase::Click;

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
	fresult fres;
	if (_OnClickHandler != NULL)
	{
		fres = _OnClickHandler->OnClick(this);
		return fres;
	}
	return SUCCESS;
}


fresult MenuItemBase::Draw()
{
	if (_underLyningControl !=NULL)
	{
		return _underLyningControl->Draw();
	}
	else
	{
		return SUCCESS;
	}
}

bool_t MenuItemBase::CheckAccelerator(ButtonState button)
{
	if (button==_Accelerator || button==_Accelerator2)
	{
		Click();
		return TRUE;
	}
	return FALSE;
}

fresult MenuItemBase::MenuItemBaseInit( IControl* _control, IMenuHandler* handler, ButtonState accelerator )
{
	_underLyningControl = _control;
	if (handler!=NULL)
	{
		_OnClickHandler = handler;
	}
	else
	{
		_OnClickHandler = NULL;
	}

	_Accelerator = accelerator;

	_Accelerator2 = -1;


	return SUCCESS;
}

fresult MenuItemBase::SetSecondAccelarator( ButtonState button )
{
	_Accelerator2 = button;
	return SUCCESS;
}

}