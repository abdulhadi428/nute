#include "lcd2630.h"

#include <Windows.h>
#include "ArmletSimulator.h"

#include <stdio.h>

#include <ch.h>

extern SIZE      ClientSize;
extern HBITMAP   armlet_emu_hBmp;

#define COLOR4(v) v = v | (v << 4)

Lcd_t Lcd;

void Lcd_t::PutBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t *bmp)
{
    uint16_t sq = (uint16_t)w*h;

    uint8_t xx = 0, yy = 0;
    uint16_t* bmpPtr = bmp;

    for (uint16_t i = 0; i < sq; ++i, ++bmpPtr)
    {
        uint8_t r = (*bmpPtr >> 8) & 0x0F;
        uint8_t g = (*bmpPtr >> 4) & 0x0F;
        uint8_t b = (*bmpPtr >> 0) & 0x0F;

        r |= r << 4;
        g |= g << 4;
        b |= b << 4;

        COLORREF c = RGB(r, g, b);
        
        sim_putPixel(x+xx, y+yy, c);

        ++xx;
        if (xx == w)
        {
            ++yy;
            xx = 0;
        }
    }

    sim_repaint();
}

void Lcd_t::GetBitmap(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint16_t *bmp)
{
    uint16_t sq = (uint16_t)width*height;

    uint16_t* ptr = bmp;

    for (uint8_t y = y0; y < y0+height; ++y)
    {
        for (uint8_t x = x0; x < x0+width; ++x)
        {
            COLORREF c = sim_getPixel(x, y);
            uint16_t r = (uint16_t)((c & 0x000000FF) >> 4);
            uint16_t g = (uint16_t)((c & 0x0000FF00) >> 12);
            uint16_t b = (uint16_t)((c & 0x00FF0000) >> 20);

            *ptr = (r << 8) | (g << 4) | b;
            ++ptr;
        }
    }
}

/*static void alphaBlend(uint16_t source, uint8_t a, uint16_t* dest)
{
    *dest =
    ((((((source >> 0) & 0x0F) * a + (*dest >> 0) & 0x0F) * (0x0F - a)) / 0x0F) << 0) |
    ((((((source >> 4) & 0x0F) * a + (*dest >> 4) & 0x0F) * (0x0F - a)) / 0x0F) << 4) |
    ((((((source >> 8) & 0x0F) * a + (*dest >> 8) & 0x0F) * (0x0F - a)) / 0x0F) << 8);
}*/
/*
void Lcd_t::PutBitmapAlpha(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint16_t *pBuf)
{
    uint16_t bmpSize = bitmapSize(w, h);
    uint8_t* bmp = (uint8_t*)chHeapAlloc(NULL, bmpSize);
    if (bmp == NULL)
        return;

    GetBitmap(x0, y0, w, h, (uint16_t*)bmp);

    uint8_t* bmpPtr = bmp;
    uint16_t sq = (uint16_t)w*(uint16_t)h;
    uint16_t *sourcePtr = pBuf;
    bool even = true;
    for (uint16_t i = 0; i < sq; ++i, even = !even, ++sourcePtr)
    {
        uint8_t a = *sourcePtr >> 12;
        uint8_t una = 0x0F - a;
        uint8_t ra = ((*sourcePtr >> 8) & 0x0F) * a;
        uint8_t ga = ((*sourcePtr >> 4) & 0x0F) * a;
        uint8_t ba = (*sourcePtr & 0x0F) * a;

        if (even)
        {
            *bmpPtr = ((ra + (*bmpPtr >> 4  ) * una) / 0x0F) << 4 |
                      ((ga + (*bmpPtr & 0x0F) * una) / 0x0F);
            ++bmpPtr;
            *bmpPtr = ((ba + (*bmpPtr >> 4) * una) / 0x0F) << 4 |
                      (*bmpPtr & 0x0F);
        }
        else
        {
            *bmpPtr = (*bmpPtr & 0xF0) |
                      ((ra + (*bmpPtr &0x0F) * una) / 0x0F) << 4;
            ++bmpPtr;
            *bmpPtr = ((ga + (*bmpPtr >> 4  ) * una) / 0x0F) << 4 |
                      ((ba + (*bmpPtr & 0x0F) * una) / 0x0F);
            ++bmpPtr;
        }
    }

    PutBitmap(x0, y0, w, h, (uint16_t*)bmp);
    chHeapFree(bmp);
}

static void dumpBitmap12(uint8_t w, uint8_t h, uint8_t* bmp)
{
    bool even = true;
    uint8_t xx = 0, yy = 0;
    uint8_t* bmpPtr = bmp;
    uint16_t sq = (uint16_t)w * (uint16_t)h;
    for (uint16_t i = 0; i < sq; ++i, even = !even)
    {
        if (even)
        {
            printf("%02X", *bmpPtr);
            ++bmpPtr;
            printf("%X ", (*bmpPtr) >> 4);
        }
        else
        {
            printf("%X", (*bmpPtr) &0x0F);
            ++bmpPtr;
            printf("%02X ", *bmpPtr);
            ++bmpPtr;
        }

        ++xx;
        if (xx == w)
        {
            xx = 0;
            ++yy;
            printf("\n");
        }
    }
}

*/