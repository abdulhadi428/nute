/********************************************************************
 * MemFont.cpp
 * Created by Vladimir Lebedev-Schmidthof <dair@dair.spb.ru>
 * at Sun Apr 21 20:35:42 MSK 2013
 *******************************************************************/

#include "MemFont.h"
#include "lcd_addon.h"

uint8_t memfont_getHeight(MEMFONT font)
{
    return *((uint8_t*)font);
}

uint8_t memfont_drawChar(MEMFONT font, uint8_t x, uint8_t y, uint8_t ch, uint16_t color)
{
    uint16_t* posTable = (uint16_t*)((uint8_t*)font + 1);
    uint16_t* pos = posTable + ch;
    if (*pos == 0xFFFF)
        return 0;

    uint8_t* glyphStart = (uint8_t*)(posTable + 256);
    uint8_t* glyph = glyphStart + (*pos);
    // code, x, y, ax, w, h
    lcd_putGlyph(x + glyph[1], y + glyph[2], glyph[4], glyph[5], glyph + 6, color);
    return glyph[3];
}

void memfont_drawString(MEMFONT font, uint8_t x, uint8_t y, const char* str, uint16_t color)
{
    uint8_t xx = x;
    const char* s = str;
    while (*s != 0)
    {
        int8_t dx = memfont_drawChar(font, xx, y, *s, color);
        xx += dx;
        ++s;
    }
}
