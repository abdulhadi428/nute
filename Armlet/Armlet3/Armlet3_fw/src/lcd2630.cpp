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
static inline void DC_Hi ()  { PinSet  (LCD_GPIO, LCD_DC);   LCD_DELAY}
static inline void DC_Lo ()  { PinClear(LCD_GPIO, LCD_DC);   LCD_DELAY}
#define WR_HI()     LCD_GPIO->BSRRL = 1<<LCD_WR
#define WR_LO()     LCD_GPIO->BSRRH = 1<<LCD_WR
static inline void RD_Hi ()  { PinSet  (LCD_GPIO, LCD_RD);   LCD_DELAY}
static inline void RD_Lo ()  { PinClear(LCD_GPIO, LCD_RD);   LCD_DELAY}

// Prototypes
static void WriteCmd(uint16_t ACmd, uint8_t AParamCount=0, ...);

// ==== Lcd Thread ====
static WORKING_AREA(waLcdThread, 128);
static msg_t LcdThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Lcd");
    while(1) {
        chThdSleepMilliseconds(999);
    } // while 1
    return 0;
}

void Lcd_t::Init() {
    // Backlight
    BckLt.Init(LCD_BCKLT_GPIO, LCD_BCKLT_PIN, LCD_BCKLT_TMR, LCD_BCKLT_CHNL, (LCD_TOP_BRIGHTNESS*2));
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    PinSetupOut(LCD_GPIO, LCD_DC,   omPushPull);
    PinSetupOut(LCD_GPIO, LCD_WR,   omPushPull);
    PinSetupOut(LCD_GPIO, LCD_RD,   omPushPull);
    PinSetupOut(LCD_GPIO, LCD_XRES, omPushPull);
    PinSetupOut(LCD_GPIO, LCD_XCS,  omPushPull);
    // Configure data bus as outputs
    for(uint8_t i=0; i<8; i++) PinSetupOut(LCD_GPIO, i, omPushPull, pudNone, ps50MHz);

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
    WR_HI();    // Default hi
    RD_Hi();    // Default hi
    XCS_Lo();   // Interface is always enabled

    WriteCmd(0x011);            // Sleep out
    Delay_ms(207);
    WriteCmd(0x13);             // Normal Display Mode ON

    //WriteCmd(0x3A, 1, 0x06);    // Pixel format: VIPF=0(undef), IFPF=18 bit per pixel
    //WriteCmd(0x3A, 1, 0x05);    // Pixel format: VIPF=0(undef), IFPF=16 bit per pixel
    WriteCmd(0x3A, 1, 0x03);    // Pixel format: VIPF=0(undef), IFPF=12 bit per pixel

    WriteCmd(0x29);             // Display on
    WriteCmd(0x20);             // Inv off
    WriteCmd(0x13);             // Normal Display Mode ON
    WriteCmd(0x36, 1, 0xA0);    // Display mode: Y inv, X none-inv, Row/Col exchanged
    //Cls(clWhite);                // clear LCD buffer

    // ======= Create and start thread =======
    chThdCreateStatic(waLcdThread, sizeof(waLcdThread), NORMALPRIO, LcdThread, NULL);
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    BacklightOff();
}

// =============================== Local use ===================================
static inline void WriteByte(uint8_t Byte) {
    LCD_GPIO->BSRRH = LCD_MASK_WR;  // Clear bus and set WR Low
    LCD_GPIO->BSRRL = Byte;         // Place data on bus
    LCD_DELAY
    WR_HI();    // Data is sent by Lo-to-Hi transition
} __attribute__ (())


void WriteCmd(uint16_t ACmd, uint8_t AParamCount, ...) {

    // Send Cmd 0 bit and Cmd byte
    Write9Bit(ACmd);
    // Send params if exist
    va_list args;
    va_start(args, AParamCount);
    while(AParamCount) {
        uint16_t w = (uint8_t)va_arg(args, int);
        w |= 0x100;     // Add "Data" bit
        Write9Bit(w);   // Send Data byte
        AParamCount--;
    }
    va_end(args);
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
// FIXME: add check for Changed array overflow
uint16_t Lcd_t::PutChar(uint8_t x, uint8_t y, char c, Color_t ForeClr, Color_t BckClr) {
    char *PFont = (char*)Font8x8;  // Font to use
    // Read font params
    uint8_t nCols = PFont[0];
    uint8_t nRows = PFont[1];
    uint16_t nBytes = PFont[2];
    // Get pointer to the first byte of the desired character
    const char *PChar = Font8x8 + (nBytes * (c - 0x1F));
    // Iterate rows of the char
    uint8_t row, col;
    for(uint8_t row = 0; row < nRows; row++) {
        if((y+row) >= LCD_H) break;
        uint8_t PixelRow = *PChar++;
        // loop on each pixel in the row (left to right)
        uint8_t Mask = 0x80;
        for (col = 0; col < nCols; col++) {
            if((x+col) >= LCD_W) break;
            PackedBuf[y+row][x+col] = (PixelRow & Mask)? ForeClr : BckClr;
            Mask >>= 1;
        } // col
    } // row
    // Mark area as changed
    uint8_t xaStart = x / AREA_W;
    uint8_t yaStart = y / AREA_H;
    uint8_t xaEnd = (x+nCols) / AREA_W;
    uint8_t yaEnd = (y+nRows) / AREA_H;
    for(row = yaStart; row<=yaEnd; row++)
        for(col = xaStart; col<=xaEnd; col++)
            Changed[row][col] = true;
    // Return next pixel to right
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
    for(uint8_t x=0; x<LCD_W; x++)
        for(uint8_t y=0; y<LCD_H; y++)
            PackedBuf[y][x] = (uint8_t)Color;
    for(uint8_t xc=0; xc<AREA_W_CNT; xc++)
        for(uint8_t yc=0; yc<AREA_H_CNT; yc++)
            Changed[yc][xc] = true;
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

// =========================== Low-level hardware ==============================
void LcdUartInit() {
    // Pins
    PinSetupAlterFunc(LCD_GPIO, LCD_SCLK, omPushPull, pudNone, AF7, ps100MHz);
    PinSetupAlterFunc(LCD_GPIO, LCD_SDA,  omPushPull, pudNone, AF7, ps100MHz);
    // ==== USART init ====
    rccEnableUSART3(FALSE);     // Usart3 CLK, no clock in low-power
    // Usart clock: enabled, idle low, first edge, enable last bit pulse
    USART3->CR1 = USART_CR1_UE; // Enable UART
    USART3->CR2 = USART_CR2_CLKEN | USART_CR2_LBCL;
    uint16_t brr = ((Clk.APB1FreqHz*2) / 4000000) & 0xFFF0;
    USART3->BRR = (brr == 0)? (1<<4) : brr; // Baudrate
    USART3->CR1 =
            USART_CR1_OVER8 |   // Use 8 samples per bit, not 16 - to increase speed
            USART_CR1_UE |      // Enable USART
            USART_CR1_M |       // 9 data bit
            USART_CR1_TE;       // Transmitter enable
    // After enabling transmitter, it will send empty character
    while(!(USART3->SR & USART_SR_TC));    // wait for transmission to complete
}
