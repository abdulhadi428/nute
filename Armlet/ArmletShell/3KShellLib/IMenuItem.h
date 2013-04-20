#pragma once

#include "project.h"
#include "IControl.h"

class IMenuItem
{
public:
	fresult virtual Draw() =0;
	bool_t virtual IsSelected()=0;

	fresult virtual Select() =0;
	fresult virtual Deselect() =0;

	fresult virtual Click() =0;
};