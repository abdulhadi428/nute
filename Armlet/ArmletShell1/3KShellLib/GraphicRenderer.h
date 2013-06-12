#pragma once
#include "IRender.h"

namespace ThreeKShell {

class GraphicRenderer : public IRender
{
public:
	virtual fresult DrawString( Position pospx, const char* str, uword_t length, TextFormat* format );

	virtual fresult DrawRect( Position pospx, Size sizepx, Color color );

	virtual fresult DrawBitmap( Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos );

};

}