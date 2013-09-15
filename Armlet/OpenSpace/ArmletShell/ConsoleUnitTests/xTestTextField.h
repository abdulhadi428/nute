#pragma once

#include "ArmletAppSDK.h"
#include "TextField.h"
#include "ToStringRenderer.h"

class xTestTextField
{
	ToStringRenderer* _renderer;

	fresult CreateTextField( char* str, Size size, Position position, TextFormat* format, bool_t wrap, TextField** opTextField, Size buffSize);
	void xTestTextField::xDrawAndScroll(TextFormat* format);
public:
	fresult Init();
	void doTests();
	
	void xInit();
	void xDrawAndScroll();

	void xPlayWithCharSize();
	void xDrawAreas();

	void xColorTest();

	void xWrapTest();
};

