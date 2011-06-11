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
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"

// ================================ Defines ====================================
#define LCD_GPIO        GPIOB
#define LCD_USART       USART3

#define LCD_SCLK        GPIO_Pin_12
#define LCD_SDA         GPIO_Pin_10
#define LCD_XRES        GPIO_Pin_11
#define LCD_XCS         GPIO_Pin_2

#define LCD_CMD     0
#define LCD_DATA    1

#define LCD_STR_HEIGHT  8
#define LCD_STR_WIDTH   16

// Uncomment this line, if image need to be turned upside down
//#define LCD_UPSIDEDOWN

enum Invert_t {NotInverted, Inverted};

class Lcd_t {
private:
    // Pin driving functions
    void SCLK_Hi(void) { LCD_GPIO->BSRR = LCD_SCLK; }
    void SCLK_Lo(void) { LCD_GPIO->BRR  = LCD_SCLK; }
    void XCS_Hi (void) { LCD_GPIO->BSRR = LCD_XCS;  }
    void XCS_Lo (void) { LCD_GPIO->BRR  = LCD_XCS;  }
    void XRES_Hi(void) { LCD_GPIO->BSRR = LCD_XRES; }
    void XRES_Lo(void) { LCD_GPIO->BRR  = LCD_XRES; }
    void SDA_Hi (void) { LCD_GPIO->BSRR = LCD_SDA;  }
    void SDA_Lo (void) { LCD_GPIO->BRR  = LCD_SDA;  }
    // Data exchange
    void WriteData(uint8_t AByte);
    void WriteCmd(uint8_t AByte);
    void GotoXY(uint8_t x, uint8_t y);
    void GotoXYstr(uint8_t x, uint8_t y);
public:
    // General use
    void Init(void);
    void Shutdown(void);
    // Grafics handling
    void Cls(void);
    void DrawChar(uint8_t AChar, Invert_t AInvert);
    void PrintString(const uint8_t x, const uint8_t y, const char *S, Invert_t AInvert);
//    void DrawImage(const uint8_t x, const uint8_t y, prog_uint8_t *I, bool AInvert);
};

extern Lcd_t Lcd;


//void LCD_PrintUint(const uint8_t x, const uint8_t y, uint16_t ANumber, bool AInvert);
//void LCD_PrintInt(const uint8_t x, const uint8_t y, int16_t ANumber, bool AInvert);
//void LCD_PrintUint0_99(const uint8_t x, const uint8_t y, uint8_t ANumber, bool AInvert);

//void LCD_PrintTime(uint8_t x, uint8_t y, bool InvertHours, bool InvertMinTens, bool InvertMinUnits);

#endif	/* LCD110X_H */

