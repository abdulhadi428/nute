#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class MenuBase : public IMenu
{
protected:

	IMenuItem** _Items;
	ubyte_t _ItemsCount;

	sbyte_t _SelectedIndex;

	bool_t _MustHaveSelected;

	fresult MenuBaseInit(IMenuItem** items, ubyte_t itemsCount)
	{
		_MustHaveSelected = FALSE;
		_SelectedIndex = -1;
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

public:
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

	virtual bool_t ProcessButton( ButtonState key ) 
	{
		bool_t itemClicked = FALSE;
		for (int i=0; i<_ItemsCount; i++)
		{
			itemClicked = _Items[i]->CheckAccelerator(key);
			if (itemClicked == TRUE)
			{
				break;
			}
		}
		
		if (itemClicked)
		{
			return TRUE;
		}

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

};
