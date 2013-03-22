#include "lcd2630.h"
#include "kl_lib_f2xx.h"
#include <string.h>
#include <stdarg.h>
#include "core_cmInstr.h"
#include "tiny_sprintf.h"

#include "lcdFont8x8.h"

// Variables
Lcd_t Lcd;
static char CharBuf[198];

// Pin driving functions
#define LCD_DELAY   Delay_ms(1);
static inline void XRES_Hi() { PinSet  (LCD_GPIO, LCD_XRES); LCD_DELAY}
static inline void XRES_Lo() { PinClear(LCD_GPIO, LCD_XRES); LCD_DELAY}
static inline void XCS_Hi () { PinSet  (LCD_GPIO, LCD_XCS);  LCD_DELAY}
static inline void XCS_Lo () { PinClear(LCD_GPIO, LCD_XCS);  LCD_DELAY}
__attribute__ ((always_inline)) static inline void DC_Hi()  { PinSet  (LCD_GPIO, LCD_DC);   LCD_DELAY}
__attribute__ ((always_inline)) static inline void DC_Lo()  { PinClear(LCD_GPIO, LCD_DC);   LCD_DELAY}
__attribute__ ((always_inline)) static inline void WR_Hi()  { PinSet  (LCD_GPIO, LCD_WR);   LCD_DELAY}
__attribute__ ((always_inline)) static inline void RD_Hi()  { PinSet  (LCD_GPIO, LCD_RD);   LCD_DELAY}
//__attribute__ ((always_inline)) static inline void RD_Lo()  { PinClear(LCD_GPIO, LCD_RD);   LCD_DELAY}

// ==== Lcd Thread ====
//static WORKING_AREA(waLcdThread, 128);
//static msg_t LcdThread(void *arg) {
//    (void)arg;
//    chRegSetThreadName("Lcd");
//    while(1) {
//        chThdSleepMilliseconds(999);
//    } // while 1
//    return 0;
//}

void Lcd_t::Init() {
    // Backlight
    BckLt.Init(LCD_BCKLT_GPIO, LCD_BCKLT_PIN, LCD_BCKLT_TMR, LCD_BCKLT_CHNL, LCD_TOP_BRIGHTNESS);
    PinSetupOut(LCD_GPIO, LCD_DC,   omPushPull, pudNone, ps100MHz);
    PinSetupOut(LCD_GPIO, LCD_WR,   omPushPull, pudNone, ps100MHz);
    PinSetupOut(LCD_GPIO, LCD_RD,   omPushPull, pudNone, ps100MHz);
    PinSetupOut(LCD_GPIO, LCD_XRES, omPushPull, pudNone, ps100MHz);
    PinSetupOut(LCD_GPIO, LCD_XCS,  omPushPull, pudNone, ps100MHz);
    // Configure data bus as outputs
    for(uint8_t i=0; i<8; i++) PinSetupOut(LCD_GPIO, i, omPushPull, pudNone, ps100MHz);

    BacklightOn(100);
    // Init buffer to write: add bound commands
//    BufToWrite[0]  = ReverseAndShift(0x02A);    // Set column bounds
//    BufToWrite[1]  = ReverseAndShift(0x100);    // 0
//    BufToWrite[3]  = ReverseAndShift(0x100);    // 0
//    BufToWrite[5]  = ReverseAndShift(0x02B);    // Set Row bounds
//    BufToWrite[6]  = ReverseAndShift(0x100);    // 0
//    BufToWrite[8]  = ReverseAndShift(0x100);    // 0
//    BufToWrite[10] = ReverseAndShift(0x02C);    // RAM Write
    // ======= Init LCD =======
    XCS_Hi();
    XRES_Lo();  // }
    XRES_Hi();  // } Reset display
    DC_Lo();    // Command mode by default
    WR_Hi();    // Default hi
    RD_Hi();    // Default hi
    XCS_Lo();   // Interface is always enabled

    WriteCmd(0x11);         // Sleep out
    Delay_ms(207);
    WriteCmd(0x13);         // Normal Display Mode ON

    //WriteCmd(0x3A, 0x06);    // Pixel format: VIPF=0(undef), IFPF=18 bit per pixel
    WriteCmd(0x3A, 0x05);    // Pixel format: VIPF=0(undef), IFPF=16 bit per pixel
    //WriteCmd(0x3A, 0x03);   // Pixel format: VIPF=0(undef), IFPF=12 bit per pixel

    WriteCmd(0x29);         // Display on
    WriteCmd(0x20);         // Inv off
    WriteCmd(0x13);         // Normal Display Mode ON
    WriteCmd(0x36, 0xA0);   // Display mode: Y inv, X none-inv, Row/Col exchanged
    //Cls(clBlue);

    PinSetupOut(GPIOD, 6, omPushPull);
    while(1) {
        PinSet(GPIOD, 6);
        Cls(clGreen);
        PinClear(GPIOD, 6);
        Cls(clRed);
    }
    //Cls(clWhite);


    // ======= Create and start thread =======
    //chThdCreateStatic(waLcdThread, sizeof(waLcdThread), NORMALPRIO, LcdThread, NULL);
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    BacklightOff();
}

// =============================== Local use ===================================
__attribute__ ((always_inline)) static inline void WriteByte(uint8_t Byte) {
    LCD_GPIO->BSRRH = LCD_MASK_WR;  // Clear bus and set WR Low
    LCD_GPIO->BSRRL = Byte;         // Place data on bus
    LCD_GPIO->BSRRL = (1<<LCD_WR);  // WR high
}

// ==== WriteCmd ====
void Lcd_t::WriteCmd(uint8_t ACmd) {
    // DC is lo by default => Cmd by default
    WriteByte(ACmd);    // Send Cmd byte
}

void Lcd_t::WriteCmd(uint8_t ACmd, uint8_t AData) {
    // DC is lo by default => Cmd by default
    WriteByte(ACmd);    // Send Cmd byte
    // Send data
    DC_Hi();
    WriteByte(AData);
    DC_Lo();
}

/* ==== Pseudographics ====
 *  Every command consists of PseudoGraph_t AChar, uint8_t RepeatCount.
 *  Example: LineHorizDouble, 11 => print LineHorizDouble 11 times.
 *  Cmds end up with 0.
 */
/*
void Lcd_t::Symbols(const uint8_t x, const uint8_t y, ...) {
    GotoCharXY(x, y);
    uint8_t FCharCode=1, RepeatCount;
    va_list Arg;
    va_start(Arg, y);    // Set pointer to last argument
    while(1) {
        FCharCode = (uint8_t)va_arg(Arg, int32_t);
        if(FCharCode == 0) break;
        RepeatCount = (uint8_t)va_arg(Arg, int32_t);
        for(uint8_t j=0; j<RepeatCount; j++) DrawChar(FCharCode, NotInverted);
    }
    va_end(Arg);
}

*/
// ================================= Printf ====================================
uint16_t Lcd_t::PutChar(uint8_t x, uint8_t y, char c, Color_t ForeClr, Color_t BckClr) {
    char *PFont = (char*)Font8x8;  // Font to use
    // Read font params
    uint8_t nCols = PFont[0];
//    uint8_t nRows = PFont[1];
//    uint16_t nBytes = PFont[2];
    // Get pointer to the first byte of the desired character
//    const char *PChar = Font8x8 + (nBytes * (c - 0x1F));
    // Iterate rows of the char
//    uint8_t row, col;
//    for(uint8_t row = 0; row < nRows; row++) {
//        if((y+row) >= LCD_H) break;
//        uint8_t PixelRow = *PChar++;
//        // loop on each pixel in the row (left to right)
//        uint8_t Mask = 0x80;
//        for (col = 0; col < nCols; col++) {
//            if((x+col) >= LCD_W) break;
//            PackedBuf[y+row][x+col] = (PixelRow & Mask)? ForeClr : BckClr;
//            Mask >>= 1;
//        } // col
//    } // row
//    // Mark area as changed
//    uint8_t xaStart = x / AREA_W;
//    uint8_t yaStart = y / AREA_H;
//    uint8_t xaEnd = (x+nCols) / AREA_W;
//    uint8_t yaEnd = (y+nRows) / AREA_H;
//    for(row = yaStart; row<=yaEnd; row++)
//        for(col = xaStart; col<=xaEnd; col++)
//            Changed[row][col] = true;
//    // Return next pixel to right
    return x+nCols;
}

void Lcd_t::Printf(uint8_t x, uint8_t y, const Color_t ForeClr, const Color_t BckClr, const char *S, ...) {
    // Printf to buffer
    va_list args;
    va_start(args, S);
    uint32_t Cnt = tiny_vsprintf(CharBuf, S, args);
    va_end(args);
    // Draw what printed
    for(uint32_t i=0; i<Cnt; i++) {
        x = PutChar(x, y, CharBuf[i], ForeClr, BckClr);
        if(x>160) break;
    }
}

// ================================ Graphics ===================================
void Lcd_t::Cls(Color_t Color) {
    // Set column bounds
    WriteByte(0x2A);
    DC_Hi();
    WriteByte(0x00);            // }
    WriteByte(LCD_X_0);         // } Col addr start = 0
    WriteByte(0x00);            // }
    WriteByte(LCD_X_0+LCD_W-1); // } Col addr end
    DC_Lo();
    // Set row bounds
    WriteByte(0x2B);
    DC_Hi();
    WriteByte(0x00);            // }
    WriteByte(LCD_Y_0);         // } Row addr start = 0
    WriteByte(0x00);            // }
    WriteByte(LCD_Y_0+LCD_H-1); // } Row addr end
    DC_Lo();
    // Write RAM
    uint8_t MSB = (uint8_t)(((uint16_t)Color) >> 8);
    uint8_t LSB = (uint8_t)(((uint16_t)Color) & 0x00FF);
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    for(uint8_t x=0; x<LCD_W; x++) {
        for(uint8_t y=0; y<LCD_H; y++) {
            WriteByte(MSB);
            WriteByte(LSB);
        }
    }
    DC_Lo();
}
/*
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
*/
