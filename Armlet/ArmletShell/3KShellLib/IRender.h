#pragma once

//Renders content on screen
#include "project.h"
#include "TextFormat.h"

class IRender
{
public:
	
	virtual fresult DrawString(Position pospx, const char* str, int length, TextFormat* format)=0;
};
