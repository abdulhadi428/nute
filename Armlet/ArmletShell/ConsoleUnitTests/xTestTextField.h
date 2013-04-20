#pragma once

#include "project.h"
#include "TextField.h"
#include "ToStringRenderer.h"

class xTestTextField
{
	ToStringRenderer* _renderer;

	fresult CreateTextField( char* string, Size size, Position position, TextFormat* format, TextField** opTextField, char** oBuff);
	void xTestTextField::xDrawAndScroll(TextFormat* format);
public:
	fresult Init();
	void doTests();
	
	void xInit();
	void xDrawAndScroll();

	void xPlayWithCharSize();
	void xDrawAreas();

	void xColorTest();
};

