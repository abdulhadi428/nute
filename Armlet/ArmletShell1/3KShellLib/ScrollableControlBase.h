#pragma once
#include "ControlBase.h"
#include "IScrollable.h"

namespace ThreeKShell {

class ScrollableControlBase : public ControlBase, public IScrollable
{

protected:
	Position _ScrollPosition;

	fresult virtual BaseInit( Size size, Position position, IRender* render)
	{
		_ScrollPosition.data =0;
		return ControlBase::BaseInit(size,position,render);
	};

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

	fresult virtual ScrollUp() 
	{
		_ScrollPosition.Top--;
		return SetScrollPosition(_ScrollPosition);
	}

	fresult virtual ScrollDown() 
	{
		_ScrollPosition.Top++;
		return SetScrollPosition(_ScrollPosition);	
	}

};

}