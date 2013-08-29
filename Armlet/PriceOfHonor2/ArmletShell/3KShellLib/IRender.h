#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

//Renders content on screen
class IRender
{
public:
	
	virtual fresult DrawString(Position pospx, const char* str, uword_t length, TextFormat* format)=0;
	virtual fresult DrawRect(Position pospx, Size sizepx, Color color) =0;
	virtual fresult DrawBitmap(Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos) =0;
};
