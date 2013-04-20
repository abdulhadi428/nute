#pragma once

#include "project.h"
#include "TextForm.h"
#include "ToStringRenderer.h"
#include "ScatteredMenu.h"

class xScatteredMenu
{
	fresult CreateTextMenu( Position* positions, char** texts, ubyte_t count, Size itemSize, TextFormat* format, TextFormat* selFormat, ScatteredMenu* oMnu );
	fresult CreateDefaultMenu(ScatteredMenu* mnu);
	fresult CreateTextField( char* string, Size size, Position position, TextFormat* format, TextField** opTextField, char** oBuff);
	ToStringRenderer* _renderer;

public:
	void doTests();
	fresult Init();


	void xLoopThroughtItems();

};
