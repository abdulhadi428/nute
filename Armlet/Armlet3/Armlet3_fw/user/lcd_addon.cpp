#include <stdlib.h>
#include <ch.h>
#include "lcd_addon.h"
#include "lcd2630.h"
#include "kl_lib_f2xx.h"
#include "sd/ff.h"

void lcd_putGlyph(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t* glyph, uint16_t color)
{
    uint16_t bmpSize = lcd_bitmapSize(w, h);
    uint8_t* bmp = (uint8_t*)chHeapAlloc(NULL, bmpSize);
    if (bmp == NULL)
        return;

    Lcd.GetBitmap(x, y, w, h, (uint16_t*)bmp);

    uint8_t* bmpPtr = bmp;
    uint16_t sq = (uint16_t)w*(uint16_t)h;
    uint8_t *glyphPtr = glyph;

    uint8_t r = (color >> 8) & 0x0F;
    uint8_t g = (color >> 4) & 0x0F;
    uint8_t b = (color >> 0) & 0x0F;

    bool even = true;
    bmpPtr = bmp;
    uint8_t xx = 0, yy = 0;

    for (uint16_t i = 0; i < sq; ++i, even = !even)
    {
        uint8_t a;
        if (even)
            a = *glyphPtr & 0x0F;
        else
        {
            a = *glyphPtr >> 4;
            ++glyphPtr;
        }

        uint8_t una = 0x0F - a;
        uint8_t ra = r * a;
        uint8_t ga = g * a;
        uint8_t ba = b * a;

//        uint16_t* pp = (uint16_t*)bmpPtr;
        *bmpPtr = ((ga + (*bmpPtr >> 4  ) * una) / 0x0F) << 4 |
                  ((ba + (*bmpPtr & 0x0F) * una) / 0x0F);
        ++bmpPtr;
        *bmpPtr = ((ra + (*bmpPtr & 0x0F) * una) / 0x0F);// << 4 |
        ++bmpPtr;

        //if (*pp == 0)
        //    Uart.Printf("    ");
        //else
        //    Uart.Printf("%03X ", *pp);

        ++xx;
        if (xx >= w)
        {
            ++yy;
            xx = 0;
            //Uart.Printf("\n");
        }
    }
    //Uart.Printf("\n");
    //Uart.Printf("\n");
//    printf("\n\n");
//    dumpBitmap12(w, h, bmp);

    Lcd.PutBitmap(x, y, w, h, (uint16_t*)bmp);
    chHeapFree(bmp);
}

void lcd_putBitmapXor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint16_t* pbmp)
{
    uint16_t bmpSize = lcd_bitmapSize(w, h);
    uint16_t* sbmp = (uint16_t*)chHeapAlloc(NULL, bmpSize);
    if (sbmp == NULL)
        return;
    Lcd.GetBitmap(x, y, w, h, (uint16_t*)sbmp);

    uint16_t sq = (uint16_t)w*(uint16_t)h;
    uint16_t* sPtr = sbmp;
    uint16_t* tPtr = pbmp;
    for (uint16_t i = 0; i < sq; ++i, ++sPtr, ++tPtr)
    {
        *sPtr ^= *tPtr;
    }

    Lcd.PutBitmap(x, y, w, h, sbmp);
    chHeapFree(sbmp);
}

void lcd_fillRectXor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    uint16_t bmpSize = lcd_bitmapSize(w, h);
    uint16_t* sbmp = (uint16_t*)chHeapAlloc(NULL, bmpSize);
    if (sbmp == NULL)
        return;
    Lcd.GetBitmap(x, y, w, h, (uint16_t*)sbmp);

    uint16_t sq = (uint16_t)w*(uint16_t)h;
    uint16_t* sPtr = sbmp;
    for (uint16_t i = 0; i < sq; ++i, ++sPtr)
    {
        *sPtr ^= color;
    }

    Lcd.PutBitmap(x, y, w, h, sbmp);
    chHeapFree(sbmp);
}

static void lcd_blendImageBw(FIL* f, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* bitmap, uint16_t colorFore, uint16_t colorBkg)
{
    unsigned int rred = 0;
    uint16_t bpr = (uint16_t)w;
    uint8_t* buf = (uint8_t*)chHeapAlloc(NULL, bpr);

    if (buf != NULL && bitmap != NULL)
    {
        uint16_t* bmpPtr = bitmap;
        uint8_t* imgPtr;
        uint8_t a, r, g, b;
        for (uint8_t yy = 0; yy < h; ++yy)
        {
            f_read(f, buf, bpr, &rred);
            if (rred == 0)
                break;

            imgPtr = buf;
            for (uint8_t xx = 0; xx < rred; ++xx, ++bmpPtr, ++imgPtr)
            {
                a = (*imgPtr) >> 4;
                uint8_t c0 = ((*imgPtr) & 0x0F);
                uint8_t c1 = 0x0F - c0;
                r = (((colorFore >> 8) & 0x0F) * c0 +
                     ((colorBkg  >> 8) & 0x0F) * c1) / 0x0F;
                g = (((colorFore >> 4) & 0x0F) * c0 +
                     ((colorBkg  >> 4) & 0x0F) * c1) / 0x0F;
                b = (((colorFore >> 0) & 0x0F) * c0 +
                     ((colorBkg  >> 0) & 0x0F) * c1) / 0x0F;
                uint8_t una = 0x0F - a;
                uint8_t ra = r * a;
                uint8_t ga = g * a;
                uint8_t ba = b * a;

                *bmpPtr = (((((*bmpPtr >> 8 ) & 0x0F) * una + ra) / 0x0F) << 8)  | // r
                          (((((*bmpPtr >> 4) & 0x0F) * una + ga) / 0x0F) << 4) | // g
                          (((((*bmpPtr >> 0 ) & 0x0F) * una + ba) / 0x0F) << 0); // b


//                Uart.Printf("%X%X ", a, c0);
//                chThdSleepMilliseconds(1);
            }
//            Uart.Printf("\n");
        }
    }

    chHeapFree(buf);
}

static void lcd_blendMemoryImageBw(const uint8_t* image, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* bitmap, uint16_t colorFore, uint16_t colorBkg)
{
    unsigned int rred = 0;
    uint16_t bpr = (uint16_t)w;
    const uint8_t* buf = image;

    if (buf != NULL && bitmap != NULL)
    {
        uint16_t* bmpPtr = bitmap;
        const uint8_t* imgPtr = buf;
        uint8_t a, r, g, b;
        for (uint8_t yy = 0; yy < h; ++yy)
        {
            for (uint8_t xx = 0; xx < w; ++xx, ++bmpPtr, ++imgPtr)
            {
                a = (*imgPtr) >> 4;
                uint8_t c0 = ((*imgPtr) & 0x0F);
                uint8_t c1 = 0x0F - c0;
                r = (((colorFore >> 8) & 0x0F) * c0 +
                     ((colorBkg  >> 8) & 0x0F) * c1) / 0x0F;
                g = (((colorFore >> 4) & 0x0F) * c0 +
                     ((colorBkg  >> 4) & 0x0F) * c1) / 0x0F;
                b = (((colorFore >> 0) & 0x0F) * c0 +
                     ((colorBkg  >> 0) & 0x0F) * c1) / 0x0F;
                uint8_t una = 0x0F - a;
                uint8_t ra = r * a;
                uint8_t ga = g * a;
                uint8_t ba = b * a;

                *bmpPtr = (((((*bmpPtr >> 8 ) & 0x0F) * una + ra) / 0x0F) << 8)  | // r
                          (((((*bmpPtr >> 4) & 0x0F) * una + ga) / 0x0F) << 4) | // g
                          (((((*bmpPtr >> 0 ) & 0x0F) * una + ba) / 0x0F) << 0); // b


//                Uart.Printf("%X%X ", a, c0);
//                chThdSleepMilliseconds(1);
            }
//            Uart.Printf("\n");
        }
    }
}

static void lcd_blendMemoryImageColor(const uint16_t* image, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* bitmap)
{
    uint16_t size = lcd_bitmapSize(w, h) / 2;
    const uint16_t* source = image;
    uint16_t* dest = bitmap;
    for (uint16_t i = 0; i < size; ++i, ++source, ++dest)
    {
        uint8_t a = (*source) >> 12;
        uint8_t una = 0x0F - a;

        *dest = (((((*source) >> 8) & 0x0F) * a + (((*dest) >> 8) & 0x0F) * una) / 0x0F) << 8 |
                (((((*source) >> 4) & 0x0F) * a + (((*dest) >> 4) & 0x0F) * una) / 0x0F) << 4 |
                (((((*source) >> 0) & 0x0F) * a + (((*dest) >> 0) & 0x0F) * una) / 0x0F) << 0;
    }
}


void lcd_putImage(uint8_t x, uint8_t y, const char* imageFile, uint16_t colorFore, uint16_t colorBkg)
{
    FIL f;
    FRESULT res = f_open(&f, imageFile, FA_READ|FA_OPEN_EXISTING);
    if (res != FR_OK)
    {
        Uart.Printf("Error opening file %s: %d\n", imageFile, res);
        return;
    }
    unsigned int rred = 0;
    uint8_t w = 0, h = 0, type = 0;
    f_read(&f, &type, 1, &rred);
    f_read(&f, &w, 1, &rred);
    f_read(&f, &h, 1, &rred);

    if (type != 'B' && type != 'C')
        return;
    uint16_t* bitmap = (uint16_t*)chHeapAlloc(NULL, lcd_bitmapSize(w, h));

    if (bitmap == NULL)
        return;

    Lcd.GetBitmap(x, y, w, h, bitmap);
    if (type == 'B')
    {
        lcd_blendImageBw(&f, x, y, w, h, bitmap, colorFore, colorBkg);
    }
    else
    {

    }

    f_close(&f);
    Lcd.PutBitmap(x, y, w, h, bitmap);
    chHeapFree(bitmap);
}

void lcd_putMemoryImage(uint8_t x, uint8_t y, const uint8_t* image, uint16_t colorFore, uint16_t colorBkg)
{
    uint8_t w = 0, h = 0, type = image[0];
    //f_read(&f, &type, 1, &rred);
    //f_read(&f, &w, 1, &rred);
    //f_read(&f, &h, 1, &rred);

    if (type != 'B' && type != 'C')
        return;
    w = image[1];
    h = image[2];

    uint16_t* bitmap = (uint16_t*)chHeapAlloc(NULL, lcd_bitmapSize(w, h));

    if (bitmap == NULL)
        return;

    Lcd.GetBitmap(x, y, w, h, bitmap);
    if (type == 'B')
    {
        lcd_blendMemoryImageBw(image+3, x, y, w, h, bitmap, colorFore, colorBkg);
    }
    else
    {
        lcd_blendMemoryImageColor((const uint16_t*)(image+3), x, y, w, h, bitmap);
    }

    Lcd.PutBitmap(x, y, w, h, bitmap);
    chHeapFree(bitmap);
}
