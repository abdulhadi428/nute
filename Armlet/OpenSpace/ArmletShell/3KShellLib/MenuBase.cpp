#include "ArmletShell.h"
#include "MenuBase.h"

namespace ThreeKShell {

fresult MenuBase::SelectItem( ubyte_t index )
{
	IMenuItem* menuItem;
	fresult fres = SUCCESS;

	//deselect prev
	if (_SelectedIndex !=-1)
	{
		menuItem = GetItem(_SelectedIndex);
		if (menuItem==NULL)
		{
			return GENERAL_ERROR;
		}
		fres = menuItem->Deselect();
		if (fres != SUCCESS)
		{
			return fres;
		}
	}
	

	//select new
	menuItem = GetItem(index);
	if (menuItem==NULL)
	{
		return GENERAL_ERROR;
	}
	fres = menuItem->Select();
	if (fres != SUCCESS)
	{
		return fres;
		
	}
	_SelectedIndex = index;
	

	return SUCCESS;
}

fresult MenuBase::SelectNext()
{
	fresult fres;
	// looping logic
	ubyte_t newSelIndex = _SelectedIndex +1;
	if (newSelIndex == _ItemsCount)
	{
		newSelIndex =0;
	}

	fres = SelectItem(newSelIndex);
	return fres;
}

fresult MenuBase::SelectPrevious()
{
	fresult fres;
	// looping logic
	ubyte_t newSelIndex = _SelectedIndex -1;
	if (newSelIndex == -1)
	{
		newSelIndex =0;
	}

	fres = SelectItem(newSelIndex);
	return fres;
}

fresult MenuBase::Draw()
{
	fresult fres;
	for (int i =0; i < _ItemsCount ; i++)
	{
		fres = _Items[i]->Draw();
		if (fres != SUCCESS)
		{
			return fres;
		}
	}

	return SUCCESS;
}

}
