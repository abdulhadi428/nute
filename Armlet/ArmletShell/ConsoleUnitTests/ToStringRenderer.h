#pragma once

#include "project.h"
#include "IRender.h"
#include "TextFormat.h"


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
	Size _sizetx;
	char GetFormatRepresenterChar(TextFormat* format);

public:
	char* _buff;
	char* _formatBuff;

	fresult virtual DrawString( Position pospx, const char* str, int length, TextFormat* format);

	fresult Init(Size size);
	fresult Cls();
	char* GetScreen();
	char* GetFormatScreen();


};