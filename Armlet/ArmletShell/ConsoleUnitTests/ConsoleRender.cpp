
#include "ConsoleRender.h"


ConsoleRender::ConsoleRender()
{
	_pConsole = new Console();
}

fresult ConsoleRender::putchar( int x, int y, char charater, char format )
{
	_pConsole->putchar(x,y,charater, format);
	return SUCCESS;
}

ConsoleRender::~ConsoleRender()
{
	delete _pConsole;
}


