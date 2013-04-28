#pragma once

//Renders content on screen
#include "project.h"
#include "TextFormat.h"

class IRender
{
public:
	
	virtual fresult DrawString(Position pospx, const char* str, int length, TextFormat* format)=0;
	virtual fresult DrawRect(Position pospx, Size sizepx, Color color) =0;
	virtual fresult DrawBitmap(Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos) =0;
};
