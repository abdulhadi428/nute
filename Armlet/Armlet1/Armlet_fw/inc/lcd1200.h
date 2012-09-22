/*
 * File:   lcd110x.h
 * Author: Kreyl Laurelindo
 *
 * Created on 24 �������� 2010 �., 14:25
 */

#ifndef LCD110X_H
#define	LCD110X_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <string.h>

// ================================ Defines ====================================
#define LCD_GPIO        GPIOB

#define LCD_SCLK        GPIO_Pin_12
#define LCD_SDA         GPIO_Pin_10
#define LCD_XRES        GPIO_Pin_11
#define LCD_XCS         GPIO_Pin_14
#define LCD_BCKLT       GPIO_Pin_15

#define LCD_STR_HEIGHT  8
#define LCD_STR_WIDTH   16

// Data sizes
#define LCD_VIDEOBUF_SIZE       864     // = 96 * 9

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

class Lcd_t {
private:
    uint8_t IBuf[LCD_VIDEOBUF_SIZE];
    // Pin driving functions
    void XRES_Hi(void) { LCD_GPIO->BSRR = LCD_XRES; }
    void XRES_Lo(void) { LCD_GPIO->BRR  = LCD_XRES; }
    void XCS_Hi (void) { LCD_GPIO->BSRR = LCD_XCS;  }
    void XCS_Lo (void) { LCD_GPIO->BRR  = LCD_XCS;  }
    void SCLK_Hi(void) { LCD_GPIO->BSRR = LCD_SCLK; }
    void SCLK_Lo(void) { LCD_GPIO->BRR  = LCD_SCLK; }
    void SDA_Hi (void) { LCD_GPIO->BSRR = LCD_SDA;  }
    void SDA_Lo (void) { LCD_GPIO->BRR  = LCD_SDA;  }
    void WriteCmd(uint8_t ACmd);
    void WriteData(uint8_t AData);
    // High-level
    uint16_t XY2Indx(uint8_t x, uint8_t y) { return (x + (y<<6)+(y<<5)); } // (y*64)+(y*32) = y*96;
    uint16_t CharXY2Indx(uint8_t x, uint8_t y) { return ((x<<2)+(x<<1) + (y<<6)+(y<<5)); }   // = x * 6; = (y*64)+(y*32) = y*96;
    void DrawChar(uint16_t *PIndx, uint8_t AChar);
public:
    // General use
    void Init(void);
    void Task(void);
    void Shutdown(void);
    void Backlight(uint8_t ABrightness)  { TIM15->CCR2 = ABrightness; }
    // High-level
    void Printf(const uint8_t x, const uint8_t y, const char *S, ...);
    void Cls(void) { for(uint16_t i = 0; i < LCD_VIDEOBUF_SIZE; i++) IBuf[i] = 0x00; }   // zero buffer
    void DrawImage(const uint8_t x, const uint8_t y, const uint8_t *Img);
    void DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode);
    // Symbols printing
    void Symbols(const uint8_t x, const uint8_t y, ...);
};

extern Lcd_t Lcd;

#endif	/* LCD110X_H */

