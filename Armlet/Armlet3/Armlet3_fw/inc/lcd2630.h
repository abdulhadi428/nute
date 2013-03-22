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
#define LCD_GPIO        GPIOE
#define LCD_DC          8
#define LCD_WR          9
#define LCD_RD          10
#define LCD_XCS         11
#define LCD_XRES        12
#define LCD_MASK_WR     (0x00FF | (1<<LCD_WR))  // clear bus and set WR low


#define LCD_BCKLT_GPIO  GPIOB
#define LCD_BCKLT_PIN   9
#define LCD_BCKLT_TMR   11
#define LCD_BCKLT_CHNL  1

// Color palette: 5R-6G-5B
enum Color_t {
    clBlack     = 0x0000,
    clRed       = 0xF800,
    clGreen     = 0x07E0,
    clBlue      = 0x001F,
    clYellow    = 0xFFE0,
    clMagenta   = 0xF81F,
    clCyan      = 0x07FF,
    clWhite     = 0xFFFF,
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

#define LCD_X_0             1   // }
#define LCD_Y_0             2   // } Zero pixels are shifted

#define LCD_H               128 // }
#define LCD_W               160 // } Pixels count

#define LCD_TOP_BRIGHTNESS  100 // i.e. 100%

class Lcd_t {
private:
    PwmPin_t BckLt;
    void WriteCmd(uint8_t ACmd);
    void WriteCmd(uint8_t ACmd, uint8_t AData);
    void GotoXY(uint8_t x, uint8_t y);
    uint16_t PutChar(uint8_t x, uint8_t y, char c, Color_t ForeClr, Color_t BckClr);
public:
    // General use
    void Init();
    void Shutdown();
    void BacklightOn(uint16_t Brightness)  { BckLt.On(Brightness); }
    void BacklightOff() { BckLt.Off(); }
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

