#include "lcd1200.h"
#include "kl_lib.h"
#include "lcd_font.h"
#include "string.h"
#include <stdarg.h>

Lcd_t Lcd;

void Lcd_t::Task(void) {
    // Output one byte of bufer every time
    static uint16_t i=0;
    WriteData(IBuf[i++]);
    if (i == LCD_VIDEOBUF_SIZE) i=0;    // Start from beginning
}

void Lcd_t::Init(void) {
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    klGpioSetupByMsk(LCD_GPIO, LCD_XRES | LCD_XCS | LCD_SCLK | LCD_SDA, GPIO_Mode_Out_PP);
    // ==== Backlight: Timer15 Ch2 ====
    // Setup pin
    klGpioSetupByMsk(GPIOB, GPIO_Pin_15, GPIO_Mode_AF_PP);
    // Remap Timer15 to PB14 & PB15
    AFIO->MAPR2 |= 0x00000001;
    // Setup timer15
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
    TIM15->CR1 = 0x01;      // Enable timer
    TIM15->CR2 = 0;
    TIM15->PSC = 0;         // Do not divide input freq
    TIM15->ARR = 100;       // Autoreload register: full brightness=100
    TIM15->BDTR = 0xC000;   // Main output Enable
    TIM15->CCMR1 = 0x6000;  // PWM mode1 on Ch2 enabled
    TIM15->CCER = 0x0010;   // Output2 enabled, polarity not inverted

    // ========================= Init LCD ======================================
    SCLK_Lo();
    XCS_Hi();
    // Reset display
    XRES_Lo();
    Delay.ms(7);
    XRES_Hi();
    WriteCmd(0xAF);    // display ON
    // Reset display again
    XRES_Lo();
    Delay.ms(7);
    XRES_Hi();
    Delay.ms(7);
    // Initial commands
    WriteCmd(0xAF);    // display ON
    WriteCmd(0xA4);    // Set normal display mode
    WriteCmd(0x2F);    // Charge pump on
    WriteCmd(0x40);    // Set start row address = 0

    WriteCmd(0xC8);    // mirror Y axis
    //WriteCmd(0xA1);    // Mirror X axis
    // Set x=0, y=0
    WriteCmd(0xB0);    // Y axis initialization
    WriteCmd(0x10);    // X axis initialisation1
    WriteCmd(0x00);    // X axis initialisation2
    Cls();             // clear LCD buffer
}


void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    SCLK_Lo();
    SDA_Lo();
    Backlight(0);
}

void Lcd_t::WriteCmd(uint8_t AByte) {
    SCLK_Lo();
    XCS_Lo();   // Select chip
    // Send "Cmd" bit
    SDA_Lo();
    SCLK_Hi();
    SCLK_Lo();
    // Send byte
    for(uint8_t i=0; i<8; i++) {
        if(AByte & 0x80) SDA_Hi();
        else SDA_Lo();
        SCLK_Hi();
        SCLK_Lo();
        AByte <<= 1;
    }
    XCS_Hi();
}
void Lcd_t::WriteData(uint8_t AByte) {
    SCLK_Lo();
    XCS_Lo();   // Select chip
    // Send "Data" bit
    SDA_Hi();
    SCLK_Hi();
    SCLK_Lo();
    // Send byte
    for(uint8_t i=0; i<8; i++) {
        if(AByte & 0x80) SDA_Hi();
        else SDA_Lo();
        SCLK_Hi();
        SCLK_Lo();
        AByte <<= 1;
    }
    XCS_Hi();
}

/* ==== Pseudographics ====
 *  Every command consists of PseudoGraph_t AChar, uint8_t RepeatCount.
 *  Example: LineHorizDouble, 11 => print LineHorizDouble 11 times.
 *  Cmds end up with 0.
 */
void Lcd_t::Symbols(const uint8_t x, const uint8_t y, ...) {
    uint16_t Indx = CharXY2Indx(x, y);
    uint8_t FCharCode=1, RepeatCount;
    va_list Arg;
    va_start(Arg, y);    // Set pointer to last argument
    while(1) {
        FCharCode = (uint8_t)va_arg(Arg, int32_t);
        if(FCharCode == 0) break;
        RepeatCount = (uint8_t)va_arg(Arg, int32_t);
        for(uint8_t j=0; j<RepeatCount; j++) DrawChar(&Indx, FCharCode);
    }
    va_end(Arg);
}


// ================================= Printf ====================================
void Lcd_t::Printf(const uint8_t x, const uint8_t y, const char *S, ...) {
    uint16_t Indx = CharXY2Indx(x, y);
    char c;
    bool WasPercent = false;
    va_list Arg;
    va_start(Arg, S);    // Set pointer to first argument
    while ((c = *S) != 0) {
        if (c == '%') {
            if (WasPercent) {
                DrawChar(&Indx, c);  // %%
                WasPercent = false;
            }
            else WasPercent = true;
        }
        else { // not %
            if (WasPercent) {
                // Get number of characters
                char c1 = *(S+1);
                uint8_t N;
                if ((c1 >= '0') and (c1 <= '9')) {
                    N = c1 - '0';
                    S++;
                }
                else N = 0;
                // Parse c
                if (c == 'c') DrawChar(&Indx, (uint8_t)va_arg(Arg, int32_t));
                else if (c == 'u') PrintUint(&Indx, va_arg(Arg, uint32_t), N);
                else if (c == 'i') PrintInt(&Indx, va_arg(Arg, int32_t), N);
//                else if ((c == 's') || (c == 'S')) PrintString(va_arg(Arg, char*));
                WasPercent = false;
            } // if was percent
            else DrawChar(&Indx, c);
        }
        S++;
    } // while
    va_end(Arg);
}

void Lcd_t::PrintUint (uint16_t *PIndx, uint32_t ANumber, uint8_t ACharCount) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};

    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= m[i]) {
            digit++;
            ANumber -= m[i];
        }
        if ((digit != '0') or ShouldPrint or (9-i < ACharCount)) {
            DrawChar(PIndx, digit);
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    DrawChar(PIndx, (uint8_t)('0'+ANumber));
}

void Lcd_t::PrintInt (uint16_t *PIndx, int32_t ANumber, uint8_t ACharCount) {
    if (ANumber < 0) {
        DrawChar(PIndx, '-');
        ANumber = -ANumber;
    }
    return PrintUint(PIndx, ANumber, ACharCount);
}


// ================================ Graphics ===================================
/*
 * Prints char at specified buf indx, returns next indx
 */
void Lcd_t::DrawChar(uint16_t *PIndx, uint8_t AChar) {
    uint8_t b;
    for(uint8_t i=0; i<6; i++) {
        b = Font_6x8_Data[AChar][i];
        IBuf[*PIndx] = b;
        (*PIndx)++;
        if (*PIndx >= LCD_VIDEOBUF_SIZE) *PIndx = 0;
    }
}

void Lcd_t::DrawImage(const uint8_t x, const uint8_t y, const uint8_t* Img) {
    uint16_t i=0, Indx;
    uint8_t Width = Img[i++], Height = Img[i++];
    for(uint8_t fy=y; fy<y+Height; fy++) {
        Indx = XY2Indx(x, fy);
        for(uint8_t fx=x; fx<x+Width; fx++) {
            IBuf[Indx++] = Img[i++];
            if (Indx >= LCD_VIDEOBUF_SIZE) break;
        } // fx
    } // fy
}

/*
 * Draw char at any y, i.e. between lines
 */
void Lcd_t::DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode) {
    uint16_t Indx;
    uint8_t b;
    // First row
    uint8_t Row = y/8;
    uint8_t ry = y - Row*8;         // y within row
    Indx = x + (Row<<6) + (Row<<5);  // Move to position
    for (uint8_t xi=0; xi<6; xi++) {
        b = Font_6x8_Data[ACode][xi];
        b <<= ry;
        IBuf[Indx++] |= b;
        if (Indx >= LCD_VIDEOBUF_SIZE) Indx = 0;
    }
    // Second row
    Row++;
    ry = 8-ry;
    Indx = x + (Row<<6) + (Row<<5);  // Move to position
    for (uint8_t xi=0; xi<6; xi++) {
        b = Font_6x8_Data[ACode][xi];
        b >>= ry;
        IBuf[Indx++] |= b;
        if (Indx >= LCD_VIDEOBUF_SIZE) Indx = 0;
    }
}
