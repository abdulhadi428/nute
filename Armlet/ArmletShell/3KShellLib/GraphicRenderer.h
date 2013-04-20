#pragma once

#include "project.h"
#include "TextFormat.h"
#include "IRender.h"

class GraphicRenderer : public IRender
{
public:
	virtual fresult DrawString( Position pospx, const char* str, int length, TextFormat* format );

};