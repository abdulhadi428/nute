#pragma once

#include "project.h"
#include "ScrollableControlBase.h"

class Image : public ScrollableControlBase
{
	uword_t const* _bitmap;
	Size _bitmapSize;
public:

	fresult virtual DrawArea( Position pos, Size size );
	fresult Init(Size size, Position position, IRender* render)
	{
		fresult fres;
		fres = ScrollableControlBase::BaseInit(size, position, render);
		return fres;
	}

	fresult SetImage(uword_t const*const bitmap, Size bitmapSize)
	{
		fresult fres = SUCCESS;
		_bitmap = bitmap;
		_bitmapSize = bitmapSize;
		return fres;
	}

};