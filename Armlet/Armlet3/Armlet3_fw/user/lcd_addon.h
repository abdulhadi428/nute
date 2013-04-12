#ifndef LCD_ADDON__H
#define LCD_ADDON__H

#include <stdint.h>

inline uint16_t lcd_bitmapSize(uint8_t w, uint8_t h)
{
    return ((uint16_t)w*(uint16_t)h)*2;
}

void lcd_putGlyph(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t* glyph, uint16_t color);

void lcd_putBitmapXor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* bmp);
void lcd_fillRectXor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

void lcd_putImage(uint8_t x, uint8_t y, const char* imageFile, uint16_t colorFore = 0xFFFF, uint16_t colorBkg = 0x0000);

#endif
