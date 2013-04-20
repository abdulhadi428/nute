#pragma once

#include "project.h"
//Color
typedef Int16 Color;

#define clBlack 1
#define clWhite 2
#define clRed 3
#define clGrey 4

//Font
struct Font
{
	//Logical Name
	char* Name;
	//ID
	char ID;
	//Size of the glyph in pixel (only MonoType fonts are supported)
	Size GlyphSize;
};

fresult TODO_KELTUR_GetFontById(int fontId, Font* oFont);
