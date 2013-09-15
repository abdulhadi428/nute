#include "ThreeKShell.h"

namespace ThreeKShell {


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
		if (_IsVisible==TRUE)
		{
			return _underLyningControl->Draw();
		}
		else
		{
			return SUCCESS;
		}

	}
	else
	{
		return SUCCESS;
	}
}

fresult MenuItemBase::ProcessKey(ButtonState button, bool* found)
{
	fresult fres;
	if (_IsEnabled == TRUE)
	{
		if (button==_Accelerator || button==_Accelerator2)
		{
			fres = Click();
			*found = TRUE;
			return fres;
		}
	}
	*found = FALSE;
	return SUCCESS;
}

fresult MenuItemBase::MenuItemBaseInit(char* name, IControl* control, IMenuHandler* handler, ButtonState accelerator )
{
	_underLyningControl = control;
	if (handler!=NULL)
	{
		_OnClickHandler = handler;
	}
	else
	{
		_OnClickHandler = NULL;
	}

	_IsSelected = FALSE;
	_IsVisible = TRUE;
	_IsEnabled = TRUE;

	_Name = name;

	_Accelerator = accelerator;
	_Accelerator2 = -1;

	return SUCCESS;
}

fresult MenuItemBase::SetSecondAccelarator( ButtonState button )
{
	_Accelerator2 = button;
	return SUCCESS;
}

ButtonState MenuItemBase::GetAccelerator()
{
	return _Accelerator;
}

fresult MenuItemBase::SetVisible( bool_t val )
{
	_IsVisible = val;
	if (_underLyningControl != NULL)
	{
		_underLyningControl->SetVisible(val);
	}
	return SUCCESS;
}

bool_t MenuItemBase::GetVisible()
{
	return _IsVisible;
}

fresult MenuItemBase::SetEnabled( bool_t val )
{
	_IsEnabled = val;
	return SUCCESS;
}

bool_t MenuItemBase::GetEnabled()
{
	return _IsEnabled;
}



char* MenuItemBase::GetName()
{
	return _Name;
}

}