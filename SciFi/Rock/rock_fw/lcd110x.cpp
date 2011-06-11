#include "lcd110x.h"
#include "lcd_font.h"
#include "images.h"

#include "delay_util.h"
#include "uart.h"

Lcd_t Lcd;

void Lcd_t::Init(void) {
    // ====================== Hardware setup ===================================
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure LCD_XRES & LCD_XCS as Push-Pull output
    GPIO_InitStructure.GPIO_Pin = LCD_XRES | LCD_XCS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_GPIO, &GPIO_InitStructure);
    // Configure LCD_SCLK & LCD_SDA as AF Push-pull
    GPIO_InitStructure.GPIO_Pin = LCD_SCLK | LCD_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LCD_GPIO, &GPIO_InitStructure);

    // Workaround hardware bug with disabled CK3 when SPI2 is enabled
    uint16_t tmp = SPI2->CR2;
    tmp |= SPI_CR2_SSOE;
    SPI2->CR2 = tmp;

    // ==== USART init ====
    // Usart clock: enabled, idle low, first edge, enable last bit pulse
    USART_ClockInitTypeDef USART_ClockInitStructure;
    USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
    USART_ClockInit(LCD_USART, &USART_ClockInitStructure);
    // Usart itself
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 250000;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(LCD_USART, &USART_InitStructure);
    // Enable USART
    USART_Cmd(LCD_USART, ENABLE);

    // ========================= Init LCD ======================================
    XCS_Hi();
    // Reset display
    XRES_Lo();
    Delay.ms(7);
    XRES_Hi();
    // Initial commands
    Write(LCD_CMD, 0xA4);   // Set normal display mode
    Write(LCD_CMD, 0x2F);   // Charge pump on
#ifdef LCD_UPSIDEDOWN
    WriteCmd(0xC8); // mirror Y axis
    WriteCmd(0xA1); // Mirror X axis
#endif
    Write(LCD_CMD, 0x40);   // Set start row address = 0
    Write(LCD_CMD, 0xAF);   // display ON
    Cls(); // clear LCD
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    //    SCLK_Lo();
    XCS_Lo();
    //SDA_Lo();
}

void Lcd_t::Cls(void) {
    Write(LCD_CMD, 0x40); // Y = 0
    Write(LCD_CMD, 0xB0);
    Write(LCD_CMD, 0x10); // X = 0
    Write(LCD_CMD, 0x00);
    for (uint16_t i = 0; i < 864; i++) Write(LCD_DATA, 0x00);
}

void Lcd_t::PrintString(const uint8_t x, const uint8_t y, const char *S, Invert_t AInvert) {
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
    for (uint8_t i = 0; i < 6; i++) {
        b = Font_6x8_Data[AChar][i];
        if (AInvert == Inverted) b = ~b;
        Write(LCD_DATA, b);
        //WriteData(0xFF);
    }
}

// ============================ Inner use ======================================
void Lcd_t::GotoXY(uint8_t x, uint8_t y) {
    Write(LCD_CMD, (0xB0 | (y & 0x0F))); // Y axis initialisation: 0100 yyyy
    Write(LCD_CMD, (0x00 | (x & 0x0F))); // X axis initialisation: 0000 xxxx ( x3 x2 x1 x0)
    Write(LCD_CMD, (0x10 | ((x >> 4)&0x07))); // X axis initialisation: 0010 0xxx  ( x6 x5 x4)
}

void Lcd_t::GotoXYstr(uint8_t x, uint8_t y) {
    x = (x << 2)+(x << 1); // x=x*6
    Write(LCD_CMD, (0xB0 | (y & 0x0F))); // Y axis initialisation: 0100 yyyy
    Write(LCD_CMD, (0x00 | (x & 0x0F))); // X axis initialisation: 0000 xxxx ( x3 x2 x1 x0)
    Write(LCD_CMD, (0x10 | ((x >> 4)&0x07))); // X axis initialisation: 0010 0xxx  ( x6 x5 x4)
}

void Lcd_t::Write(uint16_t CmdDta, uint8_t AByte) {
    // Reverse AByte bit order as USART sends data only LSB first (display needs MSB)
    uint32_t Itmp = (uint32_t)AByte;
    __ASM volatile ("rbit %0, %1" : "=r" (Itmp) : "r" (Itmp) );    // Reverse bit order, extending to uint32
    __ASM volatile ("rev %0, %1"  : "=r" (Itmp) : "r" (Itmp) );    // Reverse byte order
    Itmp <<= 1;
    Itmp |= CmdDta;
    // Send data
    XCS_Lo(); // Select chip
    USART_SendData(LCD_USART, (uint16_t)Itmp);
    /* Wait until end of transmit */
    while(USART_GetFlagStatus(LCD_USART, USART_FLAG_TC) == RESET);
    XCS_Hi();
}


//#define LCD_BIT_DELAY   10
//void Lcd_t::WriteData(uint8_t AByte) {
//    //    SCLK_Lo();
//    XCS_Lo(); // Select chip
//    // Send "Data" bit
//    //    SDA_Hi();
//    //    SCLK_Hi();
//    Delay.Loop(LCD_BIT_DELAY);
//    //    SCLK_Lo();
//    Delay.Loop(LCD_BIT_DELAY);
//    // Send byte
//    for (uint8_t i = 0; i < 8; i++) {
//        if (AByte & 0x80) SDA_Hi();
//        else SDA_Lo();
//        SCLK_Hi();
//        Delay.Loop(LCD_BIT_DELAY);
//        SCLK_Lo();
//        Delay.Loop(LCD_BIT_DELAY);
//        AByte <<= 1;
//    }
//    XCS_Lo();
//}

//void Lcd_t::WriteCmd(uint8_t AByte) {
//    SCLK_Lo();
//    XCS_Lo(); // Select chip
//    // Send "Cmd" bit
//    SDA_Lo();
//    SCLK_Hi();
//    Delay.Loop(LCD_BIT_DELAY);
//    SCLK_Lo();
//    Delay.Loop(LCD_BIT_DELAY);
//    // Send byte
//    for (uint8_t i = 0; i < 8; i++) {
//        if (AByte & 0x80) SDA_Hi();
//        else SDA_Lo();
//        SCLK_Hi();
//        Delay.Loop(LCD_BIT_DELAY);
//        SCLK_Lo();
//        Delay.Loop(LCD_BIT_DELAY);
//        AByte <<= 1;
//    }
//    XCS_Lo();
//}