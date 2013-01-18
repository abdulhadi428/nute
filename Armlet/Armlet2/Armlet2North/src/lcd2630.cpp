#include "lcd2630.h"
#include "kl_lib_f2xx.h"
#include "lcd_font.h"
#include <string.h>
#include <stdarg.h>
#include "core_cmInstr.h"
#include "tiny_sprintf.h"

#include "lcdFont8x8.h"

#define LCD_DMA         STM32_DMA1_STREAM3

// Variables
Lcd_t Lcd;
uint8_t PackedBuf[LCD_H][LCD_W];
bool Changed[AREA_H_CNT][AREA_W_CNT];   // Flags "rect changed"
// Buffer to write to display, 3/2 bytes per pixel, plus several cmds at beginning
#define BUF2WRITE_SZ    ((1+4)+(1+4)+1+(AREA_H*AREA_W*3)/2)
uint16_t BufToWrite[BUF2WRITE_SZ];
// Buffer for printf
static char CharBuf[198];

// Pin driving functions
#define LCD_DELAY   Delay_ms(1);
void XRES_Hi() { PinSet  (LCD_GPIO, LCD_XRES); LCD_DELAY}
void XRES_Lo() { PinClear(LCD_GPIO, LCD_XRES); LCD_DELAY}
void XCS_Hi () { PinSet  (LCD_GPIO, LCD_XCS);  LCD_DELAY}
void XCS_Lo () { PinClear(LCD_GPIO, LCD_XCS);  LCD_DELAY}

// Prototypes
static inline void LcdUartInit();
static inline void Write9Bit(uint16_t AWord);
static void WriteCmd(uint16_t ACmd, uint8_t AParamCount=0, ...);
static inline uint16_t ReverseAndShift(uint16_t AData) {
    uint32_t dw = AData;
    dw = __RBIT(dw);
    dw >>= 23;
    return (uint16_t)dw;
}

// ==== Lcd Thread ====
static WORKING_AREA(waLcdThread, 128);
static msg_t LcdThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Lcd");
    while(1) {
        for(uint8_t ya=0; ya<AREA_H_CNT; ya++) {
            for(uint8_t xa=0; xa<AREA_W_CNT; xa++) {
                if(Changed[ya][xa]) {
                    Changed[ya][xa] = false;
                    uint8_t x0 = xa * AREA_W, y0 = ya * AREA_H; // Coords of top left corner of area
                    // Setup bounds
                    BufToWrite[2] = ReverseAndShift(0x100 | (x0+LCD_X_0));
                    BufToWrite[4] = ReverseAndShift(0x100 | (x0+LCD_X_0+AREA_W-1));
                    BufToWrite[7] = ReverseAndShift(0x100 | (y0+LCD_Y_0));
                    BufToWrite[9] = ReverseAndShift(0x100 | (y0+LCD_Y_0+AREA_H-1));
                    // Fill buffer to write
                    uint16_t Counter=11; // There is commands ahead
                    for(uint8_t y=y0; y<(y0+AREA_H); y++) {
                        uint8_t x=x0;
                        while(x<(x0+AREA_W)) {
                            // Pixel 1
                            uint8_t v = PackedBuf[y][x++];
                            uint16_t r = v & 0b00000111;
                            uint16_t g = v & 0b00111000;
                            uint16_t b = v & 0b11000000;
                            BufToWrite[Counter++] = 0x001 | (r<<1) | (g<<2);
                            // Pixel 2
                            v = PackedBuf[y][x++];
                            r = v & 0b00000111;
                            BufToWrite[Counter++]   = 0x001 | (b>>5) | (r<<5);
                            g = v & 0b00111000;
                            b = v & 0b11000000;
                            BufToWrite[Counter++] = 0x001 | (g>>2) | (b>>1);
                        } // while x
                    } // for y
                    // Write data
                    dmaStreamSetTransactionSize(LCD_DMA, BUF2WRITE_SZ);
                    dmaStreamEnable(LCD_DMA);
                    dmaWaitCompletion(LCD_DMA);
                } // if changed
            } // xa
        } // ya
    } // while 1
    return 0;
}

void Lcd_t::Init() {
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    PinSetupOut(LCD_GPIO, LCD_XRES, omPushPull);
    PinSetupOut(LCD_GPIO, LCD_XCS,  omPushPull);
    PinSetupOut(LCD_GPIO, LCD_BCKLT,omPushPull);    // Backlight

    BacklightOn();

    // Init buffer to write: add bound commands
    BufToWrite[0]  = ReverseAndShift(0x02A);    // Set column bounds
    BufToWrite[1]  = ReverseAndShift(0x100);    // 0
    BufToWrite[3]  = ReverseAndShift(0x100);    // 0
    BufToWrite[5]  = ReverseAndShift(0x02B);    // Set Row bounds
    BufToWrite[6]  = ReverseAndShift(0x100);    // 0
    BufToWrite[8]  = ReverseAndShift(0x100);    // 0
    BufToWrite[10] = ReverseAndShift(0x02C);    // RAM Write
    // ======= Init LCD =======
    XCS_Hi();
    XRES_Lo();  // }
    XRES_Hi();  // } Reset display
    LcdUartInit();
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
    // ======= DMA =======
    // Here only the unchanged parameters of the DMA are configured
    dmaStreamAllocate     (LCD_DMA, 1, NULL, NULL);
    dmaStreamSetPeripheral(LCD_DMA, &USART3->DR);
    dmaStreamSetMemory0   (LCD_DMA, BufToWrite);
    dmaStreamSetMode      (LCD_DMA,
            STM32_DMA_CR_CHSEL(4)       // DMA1 Stream3 Channel4 is USART3_TX request
            | DMA_PRIORITY_HIGH
            | STM32_DMA_CR_MSIZE_HWORD
            | STM32_DMA_CR_PSIZE_HWORD
            | STM32_DMA_CR_MINC         // Memory pointer increase
            | STM32_DMA_CR_DIR_M2P      // Direction is memory to peripheral
             );
    USART3->CR3 = USART_CR3_DMAT;       // Enable DMA at transmitter
    // ======= Create and start thread =======
    chThdCreateStatic(waLcdThread, sizeof(waLcdThread), NORMALPRIO, LcdThread, NULL);
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    BacklightOff();
}

// =============================== Local use ===================================
void Write9Bit(uint16_t AWord) {
    while(!(USART3->SR & USART_SR_TXE));    // wait for empty TX buffer
    USART3->DR = ReverseAndShift(AWord);
}

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
    PinSetupAlterFunc(LCD_GPIO, LCD_SCLK, omPushPull, pudNone, AF7);
    PinSetupAlterFunc(LCD_GPIO, LCD_SDA,  omPushPull, pudNone, AF7);
    // ==== USART init ====
    rccEnableUSART3(FALSE);     // Usart3 CLK, no clock in low-power
    // Usart clock: enabled, idle low, first edge, enable last bit pulse
    USART3->CR1 = USART_CR1_UE; // Enable UART
    USART3->CR2 = USART_CR2_CLKEN | USART_CR2_LBCL;
    USART3->BRR = 1<<4;         // Baudrate = Fck/(8*number)
    USART3->CR1 =
            USART_CR1_OVER8 |   // Use 8 samples per bit, not 16 - to increase speed
            USART_CR1_UE |      // Enable USART
            USART_CR1_M |       // 9 data bit
            USART_CR1_TE;       // Transmitter enable
    // After enabling transmitter, it will send empty character
    while(!(USART3->SR & USART_SR_TC));    // wait for transmission to complete
}
