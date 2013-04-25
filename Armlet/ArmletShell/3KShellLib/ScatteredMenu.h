#pragma once

#include "project.h"
#include "MenuBase.h"

class ScatteredMenu : public MenuBase
{
public:
	
	fresult Init(IMenuItem** items, ubyte_t itemsCount)
	{
		 return MenuBaseInit(items,itemsCount);
	}
	
	virtual void SetMustHaveSelected( bool_t mustHaveSelected ) 
	{
		
	}

	virtual fresult Draw() 
	{
		return MenuBase::Draw();
	}

};