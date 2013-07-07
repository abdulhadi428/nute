#pragma once
#include "ControlBase.h"
#include "IScrollable.h"

namespace ThreeKShell {

class ScrollableControlBase : public ControlBase, public IScrollable
{

protected:
	Position _ScrollPosition;

	virtual fresult BaseInit( Size size, Position position, IRender* render)
	{
		_ScrollPosition.data =0;
		return ControlBase::BaseInit(size,position,render);
	};

public:
	virtual Position GetScrollPosition()
	{
		return _ScrollPosition;
	};

	virtual fresult SetScrollPosition (Position position)
	{
		_ScrollPosition= position;
		return SUCCESS;
	};

	virtual fresult ScrollUp() 
	{
		_ScrollPosition.Top--;
		return SetScrollPosition(_ScrollPosition);
	}

	virtual fresult ScrollDown() 
	{
		_ScrollPosition.Top++;
		return SetScrollPosition(_ScrollPosition);	
	}

};

}