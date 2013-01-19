/*
 * File:   lcd6101.h
 * Author: Kreyl Laurelindo
 *
 * Created on 10.11.2012
 */

#ifndef LCD6101_H
#define	LCD6101_H

#include "stm32f2xx.h"
#include "kl_lib_f2xx.h"
#include <string.h>

// ================================ Defines ====================================
#define LCD_GPIO        GPIOB

#define LCD_SCLK        12
#define LCD_SDA         10
#define LCD_XRES        11
#define LCD_XCS         2
#define LCD_BCKLT       9

// Color palette: BB-GGG-RRR; LSB first => lowest red is 0b00000100, not 0b00000001
enum Color_t {
    clBlack     = 0b00000000,
    clRed       = 0b00000111,
    clGreen     = 0b00111000,
    clBlue      = 0b11000000,
    clYellow    = 0b00111111,
    clMagenta   = 0b11000111,
    clCyan      = 0b11111000,
    clWhite     = 0b11111111,
};

enum PseudoGraph_t {
    CornerTopLeftDouble = 0x99,
    CornerTopRightDouble = 0x8B,
    CornerBottomLeftDouble = 0x98,
    CornerBottomRightDouble = 0x8C,
    LineHorizDouble = 0x9D,
    LineHorizDoubleUp = 0x9A,
    LineHorizDoubleDown = 0x9B,
    LineVertDouble = 0x8A,
    LineVertDoubleLeft = 0x89,
    LineVertDoubleRight = 0x9C,
    LineCrossDouble = 0x9E,
};

enum Invert_t {NotInverted, Inverted};

#define LCD_X_0             1   // }
#define LCD_Y_0             2   // } Zero pixels are shifted

#define LCD_H               128 // }
#define LCD_W               160 // } Pixels
#define AREA_H              16
#define AREA_W              16  // } Width must be even
#define AREA_W_CNT          (LCD_W/AREA_W)
#define AREA_H_CNT          (LCD_H/AREA_H)


class Lcd_t {
private:
    void GotoXY(uint8_t x, uint8_t y);
    uint16_t PutChar(uint8_t x, uint8_t y, char c, Color_t ForeClr, Color_t BckClr);
public:
    // General use
    void Init();
    void Shutdown();
    void BacklightOn()  { PinSet  (LCD_GPIO, LCD_BCKLT); }
    void BacklightOff() { PinClear(LCD_GPIO, LCD_BCKLT); }
    // High-level
    void Printf(uint8_t x, uint8_t y, Color_t ForeClr, Color_t BckClr, const char *S, ...);
    void Cls(Color_t Color);
    void DrawImage(const uint8_t x, const uint8_t y, const uint8_t *Img);
    void DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode);
    // Symbols printing
    void Symbols(const uint8_t x, const uint8_t y, ...);
};

extern Lcd_t Lcd;

#endif	/* LCD6101_H */

