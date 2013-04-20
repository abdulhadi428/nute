#pragma once

#include "project.h"
#include "IMenu.h"

class MenuBase : public IMenu
{
protected:

	IMenuItem** _Items;
	ubyte_t _ItemsCount;

	ubyte_t _SelectedIndex;

	bool_t _MustHaveSelected;

	virtual ubyte_t  GetSelectedItemIndex() 
	{
		return _SelectedIndex;
	};

	virtual IMenuItem* GetItem( ubyte_t index ) 
	{
		if (index < _ItemsCount)
		{
			return _Items[index];
		}
		else
		{
			return NULL;
		}
	};

	virtual bool_t  OnKey( ubyte_t key ) 
	{
		return FALSE;
	};

	virtual fresult  SelectItem( ubyte_t index );

	virtual fresult  SelectNext();

	virtual fresult  SelectPrevious();

	virtual fresult  Draw();

	virtual void  SetMustHaveSelected( bool_t mustHaveSelected )
	{
		_MustHaveSelected = mustHaveSelected;
	};

	virtual bool_t  GetMustHaveSelected() 
	{
		return _MustHaveSelected;
	};

	fresult MenuBaseInit(IMenuItem** items, ubyte_t itemsCount)
	{
		if (items == NULL)
		{
			return GENERAL_ERROR;;
		}
		_Items = items;
		
		if (itemsCount <=0)
		{
			return GENERAL_ERROR;
		}
		_ItemsCount = itemsCount;

		if (GetMustHaveSelected())
		{
			_Items[0]->Select();
		}

		return SUCCESS;
	}

};