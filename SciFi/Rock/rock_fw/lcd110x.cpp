#include "lcd110x.h"
#include "lcd_font.h"
#include "images.h"

#include "delay_util.h"

Lcd_t Lcd;

// ============================ Implementation =================================
void Lcd_t::Init(void) {
    // ==== Hardware setup ====
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure all as Push-Pull output
    GPIO_InitStructure.GPIO_Pin  = LCD_SCLK | LCD_SDA | LCD_XRES | LCD_XCS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_GPIO, &GPIO_InitStructure);
    // Setup initial values
    SCLK_Lo();
    XCS_Hi();
    // Reset display
    XRES_Lo();
    Delay.ms(7);
    XRES_Hi();
    // Initial commands
    WriteCmd(0xA4); // Set normal display mode
    WriteCmd(0x2F); // Charge pump on
    WriteCmd(0x40); // Set start row address = 0
#ifdef LCD_UPSIDEDOWN
    WriteCmd(0xC8); // mirror Y axis
    WriteCmd(0xA1); // Mirror X axis
#endif
    WriteCmd(0xAF); // display ON
    Cls();          // clear LCD
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    SCLK_Lo();
    XCS_Lo();
    SDA_Lo();
}


void Lcd_t::Cls(void) {
    WriteCmd(0x40); // Y = 0
    WriteCmd(0xB0);
    WriteCmd(0x10); // X = 0
    WriteCmd(0x00);
    for(uint16_t i=0;i<864;i++) WriteData(0x00);
}

void Lcd_t::PrintString (const uint8_t x, const uint8_t y, const char *S, Invert_t AInvert) {
    GotoXYstr(x, y);
    while (*S != '\0')
        DrawChar(*S++, AInvert);
}
//void LCD_DrawImage(const uint8_t x, const uint8_t y, prog_uint8_t *I, bool AInvert) {
//    uint8_t Width = pgm_read_byte(I++), Height = pgm_read_byte(I++);
//    uint8_t b;
//    for(uint8_t fy=y; fy<y+Height; fy++) {
//        LCD_GotoXY(x, fy);
//        for(uint8_t fx=x; fx<x+Width; fx++) {
//            b = pgm_read_byte(I++);
//            if(AInvert) b = ~b;
//            LCD_WriteData(b);
//        } // fx
//    } // fy
//
//}

//void LCD_PrintUint(const uint8_t x, const uint8_t y, uint16_t ANumber, bool AInvert) {
//    uint8_t digit = '0';
//    bool ShouldPrint = false;
//    const uint16_t FArr[9] = {10000, 1000, 100, 10};
//    LCD_GotoXYstr(x, y);
//    // Iterate until ANumber > 10
//    for(uint8_t i=0; i<4; i++) {
//        while (ANumber >= FArr[i]) {
//            digit++;
//            ANumber -= FArr[i];
//        }
//        if((digit != '0') || ShouldPrint) {
//                LCD_DrawChar(digit, AInvert);
//                ShouldPrint = true;
//        }
//        digit = '0';
//    }
//    // Print last digit
//    LCD_DrawChar('0'+ANumber, AInvert);
//}
//void LCD_PrintInt(const uint8_t x, const uint8_t y, int16_t ANumber, bool AInvert) {
//    LCD_GotoXYstr(x, y);
//    if(ANumber < 0) {
//        LCD_DrawChar('-', AInvert);
//        LCD_PrintUint(x+1, y, -ANumber, AInvert);
//    }
//    else LCD_PrintUint(x, y, -ANumber, AInvert);
//}
//// Print uint ANumber, 0<=ANumber<=99
//void LCD_PrintUint0_99(const uint8_t x, const uint8_t y, uint8_t ANumber, bool AInvert) {
//    uint8_t digit = '0';
//    LCD_GotoXYstr(x, y);
//    while(ANumber >= 10) {
//        digit++;
//        ANumber -= 10;
//    }
//    LCD_DrawChar(digit, AInvert);
//    LCD_DrawChar('0'+ANumber, AInvert);
//}
//

//
//void LCD_PrintTime(uint8_t x, uint8_t y, bool InvertHours, bool InvertMinTens, bool InvertMinUnits) {
//    LCD_GotoXYstr(x, y);
//    // Print hours
//    uint8_t t = Time.Hour;
//    uint8_t digit = '0';
//    while(t >= 10) {
//        digit++;
//        t -= 10;
//    }
//    LCD_DrawChar(digit, InvertHours);
//    LCD_DrawChar('0'+t, InvertHours);
//    // Print delimiter
//    LCD_DrawChar(':', false);
//    // Print minutes
//    t = Time.Minute;
//    digit = '0';
//    while(t >= 10) {
//        digit++;
//        t -= 10;
//    }
//    LCD_DrawChar(digit, InvertMinTens);
//    LCD_DrawChar('0'+t, InvertMinUnits);
//}

// ============================= Special =======================================
// Special

void Lcd_t::DrawChar(uint8_t AChar, Invert_t AInvert) {
    uint8_t b;
    for(uint8_t i=0; i<6; i++) {
        b = Font_6x8_Data[AChar][i];
        if(AInvert == Inverted) b = ~b;
        WriteData(b);
    }
 }

// ============================ Inner use ======================================
void Lcd_t::GotoXY(uint8_t x, uint8_t y) {
    WriteCmd(0xB0|(y&0x0F));         // Y axis initialisation: 0100 yyyy
    WriteCmd(0x00|(x&0x0F));         // X axis initialisation: 0000 xxxx ( x3 x2 x1 x0)
    WriteCmd(0x10|((x>>4)&0x07));    // X axis initialisation: 0010 0xxx  ( x6 x5 x4)
}
void Lcd_t::GotoXYstr(uint8_t x, uint8_t y) {
    x = (x<<2)+(x<<1);                   // x=x*6
    WriteCmd(0xB0|(y&0x0F));         // Y axis initialisation: 0100 yyyy
    WriteCmd(0x00|(x&0x0F));         // X axis initialisation: 0000 xxxx ( x3 x2 x1 x0)
    WriteCmd(0x10|((x>>4)&0x07));    // X axis initialisation: 0010 0xxx  ( x6 x5 x4)
}

#define LCD_BIT_DELAY   100
void Lcd_t::WriteData(uint8_t AByte) {
    SCLK_Lo();
    XCS_Lo();   // Select chip
    // Send "Data" bit
    SDA_Hi();
    SCLK_Hi();
    Delay.Loop(LCD_BIT_DELAY);
    SCLK_Lo();
    Delay.Loop(LCD_BIT_DELAY);
    // Send byte
    for(uint8_t i=0; i<8; i++) {
        if(AByte & 0x80) SDA_Hi();
        else SDA_Lo();
        SCLK_Hi();
        Delay.Loop(LCD_BIT_DELAY);
        SCLK_Lo();
        Delay.Loop(LCD_BIT_DELAY);
        AByte <<= 1;
    }
    XCS_Lo();
}
void Lcd_t::WriteCmd(uint8_t AByte) {
    SCLK_Lo();
    XCS_Lo();   // Select chip
    // Send "Cmd" bit
    SDA_Lo();
    SCLK_Hi();
    Delay.Loop(LCD_BIT_DELAY);
    SCLK_Lo();
    Delay.Loop(LCD_BIT_DELAY);
    // Send byte
    for(uint8_t i=0; i<8; i++) {
        if(AByte & 0x80) SDA_Hi();
        else SDA_Lo();
        SCLK_Hi();
        Delay.Loop(LCD_BIT_DELAY);
        SCLK_Lo();
        Delay.Loop(LCD_BIT_DELAY);
        AByte <<= 1;
    }
    XCS_Lo();
}