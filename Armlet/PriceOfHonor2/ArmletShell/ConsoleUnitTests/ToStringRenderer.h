#pragma once

#include "ArmletShell.h"
#include "IRender.h"
using namespace ThreeKShell;

struct FormatStub
{
	Color FgColor;
	Color BgColor;
	char Representor;
};

class ToStringRenderer : public IRender
{
	FormatStub* _representors;
	int _representorsCount;
	char GetFormatRepresenterChar(TextFormat* format);

public:
	Size _sizetx;

	char* _buff;
	char* _formatBuff;

	virtual fresult DrawString( Position pospx, const char* str, uword_t length, TextFormat* format);

	fresult Init(Size size);
	fresult Cls();
	char* GetScreen();
	char* GetFormatScreen();

	virtual fresult DrawRect( Position pospx, Size sizepx, Color color );

	virtual fresult DrawBitmap( Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos );


};