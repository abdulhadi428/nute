#ifndef _NAMESPACE_PROTECT_
	#error "Use ArmletAppSDK.h"
#endif

void Clear(Color backColor);

void DrawTextString(uword_t x0, uword_t y0, 
					const char* string, uword_t sz, Color foreColor, Color backColor);

void DrawRect(uword_t x, uword_t y, uword_t sx, uword_t sy, Color color);

void DrawBitmap(uword_t x, uword_t y, uword_t sx, uword_t sy, Color* bitmap);

void DrawBitmapRect(
		uword_t x, uword_t y, uword_t sx, uword_t sy, Color* bitmap,
		uword_t dx, uword_t dy, uword_t sdx, uword_t sdy);
