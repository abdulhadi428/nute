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
	virtual sbyte_t  GetSelectedItemIndex() 
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

	virtual fresult ProcessButton( ButtonState key, bool* found) 
	{
		fresult fres;

		*found = FALSE;

		for (int i=0; i<_ItemsCount; i++)
		{
			
			fres = _Items[i]->ProcessKey(key, found);
			ENSURESUCCESS(fres);
			if (*found == TRUE)
			{
				break;
			}
		}
		
		return SUCCESS;
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

	virtual ubyte_t GetItemsCount();

};
