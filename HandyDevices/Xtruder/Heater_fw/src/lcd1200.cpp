#include "lcd1200.h"
#include "kl_lib.h"
#include "lcd_font.h"
//#include "images.h"
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
    // Backlight
    klGpioSetupByMsk(LCD_GPIO, LCD_BCKLT, GPIO_Mode_Out_PP);
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
    // Set x=0, y=0
    WriteCmd(0xB0);    // Y axis initialization
    WriteCmd(0x10);    // X axis initialisation1
    WriteCmd(0x00);    // X axis initialisation2
    //WriteCmd(0xC8);    // mirror Y axis
    WriteCmd(0xA1);    // Mirror X axis
    Cls();             // clear LCD buffer
}


void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    SCLK_Lo();
    SDA_Lo();
    BacklightOff();
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

// ================================= Printf ====================================
void Lcd_t::Printf(const uint8_t x, const uint8_t y, const char *S, ...) {
    GotoCharXY(x, y);
    char c;
    bool WasPercent = false;
    va_list Arg;
    va_start(Arg, S);    // Set pointer to first argument
    while ((c = *S) != 0) {
        if (c == '%') {
            if (WasPercent) {
                DrawChar(c, NotInverted);  // %%
                WasPercent = false;
            }
            else WasPercent = true;
        }
        else { // not %
            if (WasPercent) {
                if (c == 'c') DrawChar((uint8_t)va_arg(Arg, int32_t), NotInverted);
                else if (c == 'u') PrintUint(va_arg(Arg, uint32_t));
                else if (c == 'i') PrintInt(va_arg(Arg, int32_t));
//                else if (c == 'X') PrintAsHex(va_arg(Arg, uint32_t));
//                else if ((c == 's') || (c == 'S')) PrintString(va_arg(Arg, char*));
//                else if (c == 'H') Print8HexArray(va_arg(Arg, uint8_t*), va_arg(Arg, uint32_t));
                WasPercent = false;
            } // if was percent
            else DrawChar(c, NotInverted);
        }
        S++;
    } // while
    va_end(Arg);
}

void Lcd_t::PrintUint (uint32_t ANumber) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};

    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= m[i]) {
            digit++;
            ANumber -= m[i];
        }
        if (digit != '0' || ShouldPrint) {
            DrawChar(digit, NotInverted);
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    DrawChar((uint8_t)('0'+ANumber), NotInverted);
}

void Lcd_t::PrintInt (int32_t ANumber) {
    if (ANumber < 0) {
        DrawChar('-', NotInverted);
        ANumber = -ANumber;
    }
    PrintUint (ANumber);
}


// ================================ Graphics ===================================
void Lcd_t::Cls() {
    for (uint16_t i = 0; i < LCD_VIDEOBUF_SIZE; i++) IBuf[i] = 0x00;   // Empty
}

void Lcd_t::GotoCharXY(uint8_t x, uint8_t y) {
    CurrentPosition =  (x<<2)+(x<<1);   // = x * 6; move to x
    CurrentPosition += (y<<6)+(y<<5);   // = (*64)+(*32) = *96; move to y
}
void Lcd_t::GotoXY(uint8_t x, uint8_t y) {
    CurrentPosition =  x;               // move to x
    CurrentPosition += (y<<6)+(y<<5);   // = (*64)+(*32) = *96; move to y
}

void Lcd_t::DrawChar(uint8_t AChar, Invert_t AInvert) {
    uint8_t b;
    for(uint8_t i=0; i<6; i++) {
        b = Font_6x8_Data[AChar][i];
        if(AInvert == Inverted) b = ~b;
        IBuf[CurrentPosition++] = b;
        if (CurrentPosition >= LCD_VIDEOBUF_SIZE) CurrentPosition = 0;
    }
}

void Lcd_t::PrintString(const uint8_t x, const uint8_t y, const char* S, Invert_t AInvert) {
    GotoCharXY(x, y);
    while (*S != '\0')
        DrawChar(*S++, AInvert);
}
void Lcd_t::PrintStringLen(const char *S, uint16_t ALen, Invert_t AInvert) {
    for (uint16_t i=0; ((i<ALen) && (*S != '\0')); i++)
        DrawChar(*S++, AInvert);
}

void Lcd_t::DrawImage(const uint8_t x, const uint8_t y, const uint8_t* Img) {
    uint16_t i=0;
    uint8_t Width = Img[i++], Height = Img[i++];
    for(uint8_t fy=y; fy<y+Height; fy++) {
        GotoXY(x, fy);
        for(uint8_t fx=x; fx<x+Width; fx++) {
            IBuf[CurrentPosition++] = Img[i++];
            if (CurrentPosition >= LCD_VIDEOBUF_SIZE) continue;
        } // fx
    } // fy
}

void Lcd_t::DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode) {
    uint16_t Pos;
    uint8_t b;
    // First row
    uint8_t Row = y/8;
    uint8_t ry = y - Row*8;         // y within row
    Pos = x + (Row<<6) + (Row<<5);  // Move to position
    for (uint8_t xi=0; xi<6; xi++) {
        b = Font_6x8_Data[ACode][xi];
        b <<= ry;
        IBuf[Pos++] |= b;
        if (Pos >= LCD_VIDEOBUF_SIZE) Pos = 0;
    }
    // Second row
    Row++;
    ry = 8-ry;
    Pos = x + (Row<<6) + (Row<<5);  // Move to position
    for (uint8_t xi=0; xi<6; xi++) {
        b = Font_6x8_Data[ACode][xi];
        b >>= ry;
        IBuf[Pos++] |= b;
        if (Pos >= LCD_VIDEOBUF_SIZE) Pos = 0;
    }
}
