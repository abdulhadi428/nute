/*
 * File:   lcd110x.h
 * Author: Kreyl Laurelindo
 *
 * Created on 24 �������� 2010 �., 14:25
 */

#ifndef LCD110X_H
#define	LCD110X_H

#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#include "common.h"

// ================================ Defines ====================================
#define LCD_DDR     DDRD
#define LCD_PORT    PORTD
#define LCD_PIN     PIND

#define LCD_SCLK    PD2
#define LCD_SDA     PD3
#define LCD_XRES    PD4
#define LCD_XCS     PD7
#define LCD_BCKLT   PD6

// Uncomment this line, if image need to be turned uside down
#define LCD_UPSIDEDOWN

// =========================== Pseudo functions ================================
#define LCD_SCLK_HI()   LCD_PORT |=  (1<<LCD_SCLK)
#define LCD_SCLK_LO()   LCD_PORT &= ~(1<<LCD_SCLK)
#define LCD_XCS_HI()    LCD_PORT |=  (1<<LCD_XCS)
#define LCD_XCS_LO()    LCD_PORT &= ~(1<<LCD_XCS)
#define LCD_XRES_HI()   LCD_PORT |=  (1<<LCD_XRES)
#define LCD_XRES_LO()   LCD_PORT &= ~(1<<LCD_XRES)
#define LCD_SDA_HI()    LCD_PORT |=  (1<<LCD_SDA)
#define LCD_SDA_LO()    LCD_PORT &= ~(1<<LCD_SDA)

#define LCD_BCKLT_OFF() LCD_PORT |=  (1<<LCD_BCKLT)
#define LCD_BCKLT_ON()  LCD_PORT &= ~(1<<LCD_BCKLT)


#define LCD_CMD     0
#define LCD_DATA    1

// =============================== Common use ==================================
#define LCD_STR_HEIGHT  8
#define LCD_STR_WIDTH   16

// =============================== Prototypes ==================================
void LCD_Init(void);
void LCD_Shutdown(void);

void LCD_Clear(void);
void LCD_DrawChar(uint8_t AChar, bool AInvert);
void LCD_PrintString(const uint8_t x, const uint8_t y, char *S, bool AInvert);
uint8_t LCD_PrintString_P(const uint8_t x, const uint8_t y, const char *S, bool AInvert);
void LCD_DrawImage(const uint8_t x, const uint8_t y, prog_uint8_t *I, bool AInvert);

uint8_t LCD_PrintUint(const uint8_t x, const uint8_t y, uint16_t ANumber, bool AInvert);
void LCD_PrintInt(const uint8_t x, const uint8_t y, int16_t ANumber, bool AInvert);
void LCD_PrintUint0_99(const uint8_t x, const uint8_t y, uint8_t ANumber, bool AInvert);

void LCD_PrintTime(uint8_t x, uint8_t y);

void LCD_GotoXY(uint8_t x, uint8_t y);
void LCD_GotoXYstr(uint8_t x, uint8_t y);

// Special
#define GAUGE_Y     4
void LCD_DrawGauge(void);
void LCD_GaugeValue(const uint8_t AValue);

// Inner use
void LCD_WriteData(uint8_t AByte);
void LCD_WriteCmd(uint8_t AByte);

#endif	/* LCD110X_H */

