/*
 * Font2.cpp
 *
 *  Created on: 23.03.2013
 *      Author: Vladimir "Dair" Lebedev-Schmidthof <dair@dair.spb.ru>
 *      Armlet Project for Russian RPG Community (http://www.rpg.ru)
 */

#include "Font2.h"
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

Font2::Font2(const char* filename):
    symbols(NULL),
    height(0),
    data(NULL),
    kerningSize(0),
    kerning(NULL)
{
    symbols = (uint16_t*)chHeapAlloc(NULL, sizeof(uint16_t)*256);
    if (symbols == NULL)
        return;
    memset(symbols, 0, sizeof(uint16_t)*256);
    FIL fil;

    FRESULT res = f_open(&fil, filename, FA_READ|FA_OPEN_EXISTING);
    if (res != FR_OK)
    {
        return;
    }

    UINT rred = 0;
    uint16_t count = 0;
    res = f_read(&fil, &count, 2, &rred);

    Uart.Printf("Asked for %u, got %u\n", 2, rred);
    chThdSleepMilliseconds(100);

    Uart.Printf("Count: %u\n", count);
    if (res != FR_OK)
    {
        Uart.Printf("Error reading file %s: %d\n", filename, res);
        count = 0;
    }

    if (count == 0)
        return;

    rred = 0;
    res = f_read(&fil, &height, 1, &rred);
    if (res != FR_OK)
    {
        Uart.Printf("Error reading file %s: %d\n", filename, res);
        return;
    }
    Uart.Printf("Height: %u\n", height);

    DWORD fpos = 3;
    uint16_t totalSize = 0;
    for (uint16_t i = 0; i < count; ++i)
    {
        GlyphHeader header;
        f_read(&fil, &header, sizeof(GlyphHeader), &rred);
        if (res != FR_OK)
        {
            Uart.Printf("Error reading file %s: %d\n", filename, res);
            return;
        }
        uint16_t w = header.w;
        uint16_t h = header.h;
        uint16_t bmpSize = w * h / 2;

        symbols[header.code] = totalSize;

//        Uart.Printf("header: %u: code: %02X, x = %02X, y = %02X, ax = %02X, w = %02X, h = %02X, pos = %u, headersize = %u\n", i, header.code, header.x, header.y, header.ax, header.w, header.h, symbols[header.code], sizeof(GlyphHeader));

        fpos += sizeof(GlyphHeader) + bmpSize;
        res = f_lseek(&fil, fpos);

        if (res != FR_OK)
        {
            Uart.Printf("Error lseeking file %s: %d\n", filename, res);
            break;
        }

        totalSize += sizeof(GlyphHeader) + bmpSize;
    }

    for (uint16_t i = 0; i < 256; ++i)
    {
        Uart.Printf("Symbol %u -> %u\n", i, symbols[i]);
        chThdSleepMilliseconds(10);
    }

    // now the kerning
    res = f_read(&fil, &kerningSize, 2, &rred);
    if (res != FR_OK)
    {
        Uart.Printf("Error reading file %s: %d\n", filename, res);
        return;
    }
    if (kerningSize > 0)
    {
        kerning = (Kerning*)chHeapAlloc(NULL, kerningSize*sizeof(Kerning));
        Kerning* k = kerning;
        for (uint16_t i = 0; i < kerningSize; ++i, ++k)
        {
            f_read(&fil, k, sizeof(Kerning), &rred);
        }
    }

    f_lseek(&fil, 3);
    data  = (uint8_t*)chHeapAlloc(NULL, totalSize);
    if (data != NULL)
    {
        memset(data, 0, totalSize);
        rred = 0;
        res = f_read(&fil, data, totalSize, &rred);
        if (res != FR_OK)
        {
            Uart.Printf("Error reading file %s: %d\n", filename, res);
            return;
        }
        else
        {
            Uart.Printf("Rred %u bytes of %u\n", rred, totalSize);
        }
    }

    uint16_t offset = 0;
    for (uint16_t i = 0; i < count; ++i)
    {
        GlyphHeader* header = (GlyphHeader*)(data + offset);
        Uart.Printf("#%3u: offset=%u (%04X), code = %02X, x=%02X, y=%02X, ax=%02X, w=%02X, h=%02X\n", i, offset, offset, header->code, header->x, header->y, header->ax, header->w, header->h);
        uint16_t size = (uint16_t)(header->w)* header->h / 2;
        for (uint16_t j = 0; j < size; ++j)
        {
            if (j%16 == 0)
            {
                Uart.Printf("%04X: ", offset);
            }

            Uart.Printf("%02X ", *(data + offset + 6 + j));
            if ((j+1)%16==0)
            {
                Uart.Printf("\n");
            }
            else if ((j+1)%8 == 0)
            {
                Uart.Printf(" ");
            }
        }
        Uart.Printf("\n");
        offset += 6 + size;
    }

    f_close(&fil);
}

Font2::~Font2()
{
    if (data != NULL)
        chHeapFree(data);
    if (kerning != NULL)
        chHeapFree(kerning);
    if (symbols != NULL)
        chHeapFree(symbols);
}

int8_t Font2::drawChar(uint8_t x, uint8_t y, uint16_t color, uint8_t c)
{
    GlyphHeader *header = (GlyphHeader*)(data + symbols[c]);

    Uart.Printf("drawChar: c:%u, code: %02X, x = %02X, y = %02X, ax = %02X, w = %02X, h = %02X, pos = %u, headersize = %u\n", c, header->code, header->x, header->y, header->ax, header->w, header->h, symbols[header->code], sizeof(GlyphHeader));

    lcd_putGlyph(x + header->x, y + header->y, header->w, header->h, data + symbols[c] + 6, color);
    return header->ax;
}

int8_t Font2::findKerning(uint16_t p)
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

void Font2::drawString(uint8_t x, uint8_t y, uint16_t color, const char* s)
{
    if (s == NULL) return;
    int8_t k = 0;

    while (*s != 0)
    {
        int8_t dx = drawChar(x, y, color, *s);
        x += dx;
        uint16_t* p = (uint16_t*)s;
        k = findKerning(*p);
        x += k;
        ++s;
    }
}

uint8_t Font2::getHeight() const
{
    return height;
}
