#pragma once

#include "project.h"
#include "TextFormat.h"
#include "IRender.h"

class GraphicRenderer : public IRender
{
public:
	virtual fresult DrawString( Position pospx, const char* str, int length, TextFormat* format );

	virtual fresult DrawRect( Position pospx, Size sizepx, Color color );

	virtual fresult DrawBitmap( Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos );

};