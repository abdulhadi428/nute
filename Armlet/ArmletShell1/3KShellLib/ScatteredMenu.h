#pragma once
#include "MenuBase.h"

namespace ThreeKShell {

class ScatteredMenu : public MenuBase
{
public:
	
	fresult Init(IMenuItem** items, ubyte_t itemsCount)
	{
		 return MenuBaseInit(items,itemsCount);
	}
	
	virtual void SetMustHaveSelected( bool_t mustHaveSelected ) 
	{
		_MustHaveSelected = mustHaveSelected;
	}

	virtual fresult Draw() 
	{
		return MenuBase::Draw();
	}

};

}