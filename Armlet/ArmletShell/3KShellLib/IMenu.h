#pragma once

#include "project.h"
#include "IMenuItem.h"

class IMenu
{
public:
	virtual ubyte_t  GetSelectedItemIndex()=0;

	virtual IMenuItem*  GetItem(ubyte_t index) =0;

	virtual void SetMustHaveSelected(bool_t mustHaveSelected) = 0;
	virtual bool_t GetMustHaveSelected() = 0;

	virtual bool_t OnKey(ubyte_t key) =0;
	
	virtual fresult  SelectItem(ubyte_t index)=0;
	virtual fresult  SelectNext()=0;
	virtual fresult  SelectPrevious()=0;

	virtual fresult  Draw()=0;
};