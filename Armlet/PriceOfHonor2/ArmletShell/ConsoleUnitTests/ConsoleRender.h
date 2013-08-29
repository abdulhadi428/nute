#pragma once

#include "ArmletShell.h"
#include "IRender.h"
#include "Console.h"
using namespace ThreeKShell;

class ConsoleRender : IRender
{
	Console* _pConsole;
public:
	ConsoleRender();
	~ConsoleRender();

	fresult putchar( int x, int y, char charater, char format );
	fresult DrawString(int pospx, const char* str, int length, TextFormat* format);
};