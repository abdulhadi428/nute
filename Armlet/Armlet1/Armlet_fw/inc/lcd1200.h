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

enum Invert_t {NotInverted, Inverted};

class Lcd_t {
private:
    // Variables
    uint8_t IBuf[LCD_VIDEOBUF_SIZE];
    uint16_t CurrentPosition;   // x, y to place data to
    // Text
    uint32_t Timer;
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
    void GotoXY(uint8_t x, uint8_t y);
    void PrintUint (uint32_t ANumber);
    void PrintInt (int32_t ANumber);
    void PrintString (const uint8_t x, const uint8_t y, const char *S, Invert_t AInvert);
    void PrintStringLen(const char *S, uint16_t ALen, Invert_t AInvert);
public:
    // General use
    void Init(void);
    void Task(void);
    void Shutdown(void);
    void Backlight(uint8_t ABrightness)  { TIM15->CCR2 = ABrightness; }
    // High-level
    void GotoCharXY(uint8_t x, uint8_t y);
    void DrawChar(uint8_t AChar, Invert_t AInvert);
    void Printf(const uint8_t x, const uint8_t y, const char *S, ...);
    void Cls(void);
    void DrawImage(const uint8_t x, const uint8_t y, const uint8_t *Img);
    void DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode);
    // Symbols printing
    void Symbols(const uint8_t x, const uint8_t y, ...);
};

extern Lcd_t Lcd;

#endif	/* LCD110X_H */

