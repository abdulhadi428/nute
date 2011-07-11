#include "matrix_lcd.h"
#include "common.h"
#include <util/delay.h>

void LCD_Init(void) {
    // Ports setup
    LCD_CNTR_DDR  |=   (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_E);
    LCD_CNTR_PORT &= ~((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_E));
    _delay_ms(150);
    // Set 8-bit mode
    LCD_DATA_DDR |= (1<<LCD_DATA7)|(1<<LCD_DATA6)|(1<<LCD_DATA5)|(1<<LCD_DATA4);
    LCD_DATA_PORT &= ~((1<<LCD_DATA7)|(1<<LCD_DATA6));
    LCD_DATA_PORT |= (1<<LCD_DATA5)|(1<<LCD_DATA4);
    LCD_CycleE();
    _delay_ms(5);
    LCD_CycleE();
    _delay_ms(2);
    LCD_CycleE();
    _delay_ms(2);
    // Set 4-bit mode
    LCD_DATA_PORT &= ~((1<<LCD_DATA7)|(1<<LCD_DATA6)|(1<<LCD_DATA4));
    LCD_DATA_PORT |= (1<<LCD_DATA5);
    LCD_CycleE();
    _delay_ms(2);
    // Repeat 4-bit mode with needed F & N flags
    #ifdef LCD_2_LINE
    LCD_Command(0b00101000);
    #else
    LCD_Command(0b00100000);
    #endif
    // D=1: image on; C=0: no underline cursor; B=0: no big cursor
    LCD_Command(0b00001100);
    // I/D=1: increase address counter; S=0: do not move screen
    LCD_Command(0b00000110);
    LCD_cls();
}

FORCE_INLINE void LCD_cls(void) {
    LCD_BusyWaitBF();
    LCD_Command(0x01);
}

void LCD_WriteChar(uint8_t AChar) {
    LCD_RS_HI();
    LCD_Write(AChar);
    LCD_BusyWaitBF();
}

void LCD_SetCursor(uint8_t x, uint8_t y) {
    uint8_t FNumber = 0x80;
    FNumber += x;
    if (y==1) FNumber += 0x40;
    LCD_BusyWaitBF();
    LCD_Command(FNumber);
}

void LCD_PrintString_P(const uint8_t x, const uint8_t y, const char *S) {
    LCD_SetCursor(x, y);
    uint8_t FChar;
    while ((FChar = pgm_read_byte(S++)) != '\0')
        LCD_WriteChar(FChar);
}

void LCD_PrintUint32(const uint8_t x, const uint8_t y, uint32_t ANumber) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t FArr[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};
    LCD_SetCursor(x, y);

    // Iterate until ANumber > 10
    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= FArr[i]) {
            digit++;
            ANumber -= FArr[i];
        }
        if((digit != '0') || ShouldPrint) {
                LCD_WriteChar(digit);
                ShouldPrint = true;
        }
        digit = '0';
    }
    // Print last digit
    LCD_WriteChar('0'+ANumber);
}


// ============================= Inner use =====================================
void LCD_BusyWaitBF(void) {
    uint8_t Reply;
    do {
        Reply = LCD_ReadCntr();
    } while (Reply & 0x80);
}

uint8_t LCD_ReadCntr(void) {
    // Set COMMAND mode
    LCD_RS_LO();
    return LCD_Read();
}

void LCD_Command(uint8_t ACmd) {
    LCD_RS_LO();
    LCD_Write(ACmd);
    LCD_BusyWaitBF();
}

FORCE_INLINE void LCD_Write(uint8_t AByte) {
    uint8_t tetra;
    // Set data lines as outputs
    LCD_DATA_DDR |= (1<<LCD_DATA7)|(1<<LCD_DATA6)|(1<<LCD_DATA5)|(1<<LCD_DATA4);
    // Set WRITE mode
    LCD_RW_LO();
    // Write upper tetra
    tetra = AByte & 0xF0;
    tetra >>= 1;
    LCD_DATA_PORT &= ~((1<<LCD_DATA7)|(1<<LCD_DATA6)|(1<<LCD_DATA5)|(1<<LCD_DATA4));
    LCD_DATA_PORT |= tetra;
    LCD_CycleE();
    // Write lower tetra
    tetra = AByte & 0x0F;
    tetra <<= 3;
    LCD_DATA_PORT &= ~((1<<LCD_DATA7)|(1<<LCD_DATA6)|(1<<LCD_DATA5)|(1<<LCD_DATA4));
    LCD_DATA_PORT |= tetra;
    LCD_CycleE();
}
FORCE_INLINE uint8_t LCD_Read(void) {
    uint8_t tetra, result;
    // Set data lines as inputs
    LCD_DATA_DDR &= ~((1<<LCD_DATA7)|(1<<LCD_DATA6)|(1<<LCD_DATA5)|(1<<LCD_DATA4));
    // Set READ mode
    LCD_RW_HI();
    // Read hi tetra
    tetra = LCD_CycleE();
    tetra <<= 1;
    result = tetra & 0xF0;
    // Read low tetra
    asm volatile (
        "nop"   "\n\t"
        "nop"   "\n\t"
        "nop"   "\n\t"
    ::);
    tetra = LCD_CycleE();
    tetra >>= 3;
    result |= (tetra & 0x0F);
    return result;
}

FORCE_INLINE uint8_t LCD_CycleE(void) {
    LCD_E_HI();
    asm volatile (
        "nop"   "\n\t"
        "nop"   "\n\t"
        "nop"   "\n\t"
    ::);
    uint8_t result = LCD_DATA_PIN;
    LCD_E_LO();
    return result;
}