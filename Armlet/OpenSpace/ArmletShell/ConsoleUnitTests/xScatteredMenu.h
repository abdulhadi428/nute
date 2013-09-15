#pragma once

#include "ArmletAppSDK.h"
#include "TextForm.h"
#include "ToStringRenderer.h"
#include "ScatteredMenu.h"

class xScatteredMenu
{
	fresult CreateTextField( char* str, Size size, Position position, TextFormat* format, TextField** opTextField, Size buffSize);
	fresult CreateTextMenu( Position* positions, char** texts, ubyte_t count, Size itemSize, TextFormat* format, TextFormat* selFormat, ScatteredMenu** oMnu );
	fresult CreateDefaultMenu(ScatteredMenu** mnu);
	ToStringRenderer* _renderer;

public:
	void doTests();
	fresult Init();


	void xLoopThroughtItems();

};
