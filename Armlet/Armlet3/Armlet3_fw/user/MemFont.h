/********************************************************************
 * MemFont.h
 * Created by Vladimir Lebedev-Schmidthof <dair@dair.spb.ru>
 * at Sun Apr 21 20:35:42 MSK 2013
 *******************************************************************/

#ifndef MEMFONT__H
#define MEMFONT__H

#include <stdint.h>

typedef const uint8_t* MEMFONT;

uint8_t memfont_getHeight(MEMFONT font);
uint8_t memfont_getWidth(MEMFONT font);
void memfont_drawString(MEMFONT font, uint8_t x, uint8_t y, const char* str, uint16_t color);

#endif // MEMFONT__H
