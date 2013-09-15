#ifndef _NAMESPACE_PROTECT_
	#error "Use ArmletAppSDK.h"
#endif

/*
	returns 0 if both dimensions of size1 does not exceed size2
	 WIDTH_EXCEEDS - if width exceeds
	 HEIGHT_EXCEEDS - if height exceeds
*/
fresult SizeIsSmaller(Size size1, Size size2);

bool_t IsEmpty(Size size);

bool_t IsPositionInsideSize(Size size, Position position);

bool_t IsRectInsideRect(Position testPos, Size testSize, Position checkedPosition, Size checkedSize);

bool_t GetRectClipping(Position pos1, Size size1, Position pos2, Size size2, Position* resPos, Size* resSize);

