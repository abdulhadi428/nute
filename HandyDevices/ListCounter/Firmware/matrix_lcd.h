/* 
 * File:   matrix_lcd.h
 * Author: Kreyl Laurelindo
 *
 * Created on 30 Июль 2010 г., 11:17
 */

#ifndef MATRIX_LCD_H
#define	MATRIX_LCD_H

#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

// =================================== Settings ================================
#define LCD_2_LINE

// ==================================== Ports ==================================
#define LCD_DATA_DDR    DDRD
#define LCD_DATA_PORT   PORTD
#define LCD_DATA_PIN    PIND

#define LCD_DATA7       PD6
#define LCD_DATA6       PD5
#define LCD_DATA5       PD4
#define LCD_DATA4       PD3

#define LCD_CNTR_DDR    DDRD
#define LCD_CNTR_PORT   PORTD
#define LCD_CNTR_PIN    PIND

#define LCD_RS          PD0
#define LCD_RW          PD1
#define LCD_E           PD2

// ============================ Pseudo-functions ===============================
#define LCD_E_HI()      LCD_CNTR_PORT |=  (1<<LCD_E)
#define LCD_E_LO()      LCD_CNTR_PORT &= ~(1<<LCD_E)
#define LCD_RS_HI()     LCD_CNTR_PORT |=  (1<<LCD_RS)
#define LCD_RS_LO()     LCD_CNTR_PORT &= ~(1<<LCD_RS)
#define LCD_RW_HI()     LCD_CNTR_PORT |=  (1<<LCD_RW)
#define LCD_RW_LO()     LCD_CNTR_PORT &= ~(1<<LCD_RW)

// =============================== Prototypes ==================================
void LCD_Init(void);
void LCD_cls(void);
void LCD_WriteChar(uint8_t AChar);
void LCD_SetCursor(uint8_t x, uint8_t y);

void LCD_PrintString_P(const uint8_t x, const uint8_t y, const char *S);
void LCD_PrintUint32(const uint8_t x, const uint8_t y, uint32_t ANumber);

// Inner use
uint8_t LCD_CycleE(void);
void LCD_Command(uint8_t ACmd);
void LCD_BusyWaitBF(void);
uint8_t LCD_ReadCntr(void);

uint8_t LCD_ReadWrite(void);


void LCD_Write(uint8_t AByte);
uint8_t LCD_Read(void);

#endif	/* MATRIX_LCD_H */

