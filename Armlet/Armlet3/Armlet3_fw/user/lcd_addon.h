#ifndef LCD_ADDON__H
#define LCD_ADDON__H

#include <stdint.h>

// considering 16 bits per pixel
inline uint16_t lcd_bitmapSize(uint8_t w, uint8_t h)
{
    return ((uint16_t)w*(uint16_t)h)*2;
}

// puts 4bpp glyph to screen; x,y,w,h - glyph geometry, "w" is ALWAYS even; 
// glyph - glyph data, 4bpp;
// color -- color for text, 0x0RGB, i.e. 4 bits per component, 12 bits valuable (no alpha here)
void lcd_putGlyph(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t* glyph, uint16_t color);

// puts bitmap to screen, XORing pixel by pixel with existing data. No alpha channel is used.
void lcd_putBitmapXor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint16_t* bmp);

// fills rectangle bu XORing pixel value with "color" parameter. No alpha.
void lcd_fillRectXor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

// puts a binary image from file to screen
void lcd_putImage(uint8_t x, uint8_t y, const char* imageFile, uint16_t colorFore = 0xFFFF, uint16_t colorBkg = 0x0000);

// puts binary image from memory to screen
// x, y -- coordinates on screen
// img -- pointer to raw image data (starting with 'B' or 'C'
// for 'B' (black/white) images there could be 'colorizing' used. You can set "colorFore" and "colorBkg" to some values and then "white" color in image becomes colorFore and "black" - colorBkg
// everything in the middle -- well, everything in the middle. Alpha channel is used in image, NOT in colorizing values.
void lcd_putMemoryImage(uint8_t x, uint8_t y, const uint8_t* img, uint16_t colorFore = 0xFFFF, uint16_t colorBkg = 0x0000);

#endif
