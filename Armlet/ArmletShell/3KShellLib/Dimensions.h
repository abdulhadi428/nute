#pragma once

#include "project.h"

/*
	returns 0 if both dimensions of size1 does not exceed size2
	 -1 - if width exceeds
	 -2 - if height exceeds
*/
int SizeIsSmaller(Size size1, Size size2);

int IsEmpty(Size size);

int IsPositionInsideSize(Size size, Position position);

bool_t IsRectInsideRect(Position testPos, Size testSize, Position checkedPosition, Size checkedSize);

bool_t GetRectClipping(Position pos1, Size size1, Position pos2, Size size2, Position* resPos, Size* resSize);

