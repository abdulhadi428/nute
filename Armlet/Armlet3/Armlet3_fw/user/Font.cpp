/*
 * Font.cpp
 *
 *  Created on: 23.03.2013
 *      Author: Vladimir "Dair" Lebedev-Schmidthof <dair@dair.spb.ru>
 *      Armlet Project for Russian RPG Community (http://www.rpg.ru)
 */

#include "Font.h"
#include <stddef.h>

#include "kl_lib_f2xx.h"
#include <ch.h>
#include "lcd2630.h"
#include "lcd_addon.h"

#pragma pack(push,1)

typedef struct
{
    uint8_t code;
    int8_t x;
    int8_t y;
    int8_t ax;
    uint8_t w;
    uint8_t h;
} GlyphHeader;
#pragma pack(pop)

Font::Font(const char* filename):
    fileName(NULL),
    count(0),
    height(0),
    symbols(NULL),
    positions(NULL),
    kerningSize(0),
    kerning(NULL)
{
    FIL fil;

    size_t s = strlen(filename);
    fileName = (char*)chHeapAlloc(NULL,s+1);
    memcpy(fileName, filename, s+1);

    FRESULT res = f_open(&fil, filename, FA_READ|FA_OPEN_EXISTING);
    if (res != FR_OK)
    {
        return;
    }

    UINT rred = 0;
    res = f_read(&fil, &count, 2, &rred);
//    Uart.Printf("Count: %u\n", count);
    if (res != FR_OK)
    {
        count = 0;
        return;
    }

    rred = 0;
    res = f_read(&fil, &height, 1, &rred);
//    Uart.Printf("Height: %u\n", height);

    symbols = (char*)chHeapAlloc(NULL, count*sizeof(char));
    positions = (DWORD*)chHeapAlloc(NULL, count*sizeof(DWORD));

    DWORD fpos = 3;
    char* symbol = symbols;
    DWORD* position = positions;
    for (uint16_t i = 0; i < count; ++i)
    {
        GlyphHeader header;
        f_read(&fil, &header, sizeof(GlyphHeader), &rred);
//        Uart.Printf("header: %u: code: %02X, x = %d, y = %d, w = %u, h = %u\n", i, header.code, header.x, header.y, header.w, header.h);
        *symbol = header.code;
        *position = fpos;
        uint16_t w = header.w;
        uint16_t h = header.h;
        uint16_t bmpSize = w * h / 2;

        fpos += sizeof(GlyphHeader) + bmpSize;
        f_lseek(&fil, fpos);
        ++symbol;
        ++position;
    }

    // now the kerning
    f_read(&fil, &kerningSize, 2, &rred);
    kerning = (Kerning*)chHeapAlloc(NULL, kerningSize*sizeof(Kerning));
//    Uart.Printf("Kerning: %u, sizeof = %u\r\n", kerningSize, sizeof(Kerning));
    Kerning* k = kerning;
    for (uint16_t i = 0; i < kerningSize; ++i, ++k)
    {
        f_read(&fil, k, sizeof(Kerning), &rred);
//        Uart.Printf("Rred: %04X -> %d\r\n", k->kernPair, k->dx);
    }

    f_close(&fil);
}

Font::~Font()
{
    if (count > 0)
    {
        chHeapFree(symbols);
        chHeapFree(positions);
    }
    chHeapFree(fileName);
}

int8_t Font::drawChar(uint8_t x, uint8_t y, uint16_t color, char ch)
{
//    if (ch == ' ')
//        return 5;
    uint16_t pos = findChar(ch);
    if (pos == 0xFFFF)
        return 0;

    FIL fil;
    f_open(&fil, fileName, FA_READ|FA_OPEN_EXISTING);

    int8_t ret = drawCharIntl(&fil, x, y, color, pos);
    f_close(&fil);
    return ret;
}

int8_t Font::drawCharIntl(FIL* fil, uint8_t x, uint8_t y, uint16_t color, uint16_t pos)
{
    f_lseek(fil, positions[pos]);

    UINT rred = 0;
    GlyphHeader header;
    f_read(fil, &header, sizeof(GlyphHeader), &rred);

    uint16_t square = (uint16_t)header.w * (uint16_t)header.h;
    uint16_t bmpSize = square / 2;
    uint8_t* bitmap = (uint8_t*)chHeapAlloc(NULL, bmpSize);
    f_read(fil, bitmap, bmpSize, &rred);

    lcd_putGlyph(x + header.x, y + header.y, header.w, header.h, bitmap, color);

    chHeapFree(bitmap);
    return header.ax;
}

uint16_t Font::findChar(char ch)
{
    uint16_t _min = 0;
    uint16_t _max = count;
    uint16_t pos;// = 0xFFFF;
    uint16_t ret = 0xFFFF;
    while (_max - _min > 1)
    {
        pos = _min + (_max - _min) / 2;
        char sc = symbols[pos];
//        Uart.Printf("Looking for %02X. Try %u: %02X\r\n", ch, pos, sc);
        if (sc == ch)
        {
            ret = pos;
            break;
        }
        else if ((unsigned char)sc > (unsigned char)ch)
        {
            _max = pos;
        }
        else
        {
            _min = pos;
        }
    }

    return ret;
}

int8_t Font::findKerning(uint16_t p)
{
    uint16_t _min = 0;
    uint16_t _max = kerningSize;
    uint16_t pos;// = 0xFFFF;
    int8_t ret = 0;

    if (kerningSize == 0)
        return 0;

    if (p < kerning[0].kernPair ||
        p > kerning[kerningSize-1].kernPair)
        return 0;

    while (_max - _min > 1)
    {
        pos = _min + (_max - _min) / 2;
        Kerning* k = kerning;
        k += pos;

//        Uart.Printf("Looking for %02X. Try %u: %02X\r\n", ch, pos, sc);
        if (k->kernPair == p)
        {
            ret = k->dx;
            break;
        }
        else if (k->kernPair > p)
        {
            _max = pos;
        }
        else
        {
            _min = pos;
        }
    }

    return ret;
}

void Font::drawString(uint8_t x, uint8_t y, uint16_t color, const char* s)
{
    if (s == NULL) return;
    int8_t k = 0;
    FIL fil;
    f_open(&fil, fileName, FA_READ|FA_OPEN_EXISTING);

    while (*s != 0)
    {
        uint16_t pos = findChar(*s);
        if (pos == 0xFFFF)
            break;

        int8_t dx = drawCharIntl(&fil, x, y, color, pos);
        x += dx;
        uint16_t* p = (uint16_t*)s;
        k = findKerning(*p);
        x += k;
        ++s;
    }
    f_close(&fil);
}

uint8_t Font::getHeight() const
{
    return height;
}
