#pragma once

#include "Constants.h"
#include "Types.h"
#include "DownInterface.h"
#include "UpInterface.h"

#include "string.h"

extern const unsigned char Font_6x8_Data[256][6];

void Show_Glyphs(int dx, int dy, int cx,int cy, 
	int scaleFactor, int fillFactor);