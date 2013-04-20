#pragma once

#include "ControlBase.h"
#include "IScrollable.h"

class ScrollableControlBase : public ControlBase, public IScrollable
{

protected:
	Position _ScrollPosition;

public:
	Position virtual GetScrollPosition()
	{
		return _ScrollPosition;
	};

	fresult virtual SetScrollPosition (Position position)
	{
		_ScrollPosition= position;
		return SUCCESS;
	};

};