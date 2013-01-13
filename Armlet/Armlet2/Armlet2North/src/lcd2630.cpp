#include "lcd2630.h"
#include "kl_lib_f2xx.h"
#include "lcd_font.h"
#include "stm32f2xx_usart.h"
#include <string.h>
#include <stdarg.h>
#include "core_cmInstr.h"

// Commands
#define CMD_SLEEP_OUT   0x11
#define CMD_RAM_WRITE   0x2C


Lcd_t Lcd;

inline static void LcdUartInit();

void Lcd_t::Task(void) {
    // Output one byte of bufer every time
    static uint16_t i=0;
    WriteData(IBuf[i++]);
    if (i == LCD_VIDEOBUF_SIZE) i=0;    // Start from beginning
}

void Lcd_t::Init(void) {
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    PinSetupOut(LCD_GPIO, LCD_XRES, omPushPull);
    PinSetupOut(LCD_GPIO, LCD_XCS,  omPushPull);
    PinSetupOut(LCD_GPIO, LCD_BCKLT,omPushPull);    // Backlight

    BacklightOn();
    // ========================= Init LCD ======================================
    XCS_Hi();
    // Reset display
    XRES_Lo();
    XRES_Hi();

    LcdUartInit();

    WriteCmd(CMD_SLEEP_OUT);
    volatile uint32_t i=0;
    while(i<207000) i++;

    //halPolledDelay(207);    // 120 ms needed

//    // Read ID
//    ID[0] = ReadByte(0xDA);
//    ID[1] = ReadByte(0xDB);
//    ID[2] = ReadByte(0xDC);
//    Uart.Printf("Lcd ID: %A\r", ID, 3);
//    uint32_t tmp = Read24(0x04);
//    Uart.Printf("Lcd ID2: %08X\r", tmp);

    WriteCmd(0x13);             // Normal Display Mode ON
    //WriteCmd(0x3A, 1, 0x06);    // Pixel format: VIPF=0(undef), IFPF=18 bit per pixel
    WriteCmd(0x3A, 1, 0x05);    // Pixel format: VIPF=0(undef), IFPF=16 bit per pixel
    //WriteCmd(0x3A, 1, 0x03);    // Pixel format: VIPF=0(undef), IFPF=12 bit per pixel

    WriteCmd(0x29);             // Display on
    WriteCmd(0x20);             // Inv off
    WriteCmd(0x13);             // Normal Display Mode ON
    WriteCmd(0x36, 1, 0xA0);    // Display mode: Y inv, X none-inv, Row/Col exchanged
    Cls(clBlue);                // clear LCD buffer
    //PutStr("To be or not to be", 0, 0, 0x001F, 0xFFFF);
    //PutStr("A", 18, 9, 0xFF, 0x00);
}


void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    BacklightOff();
}

void Lcd_t::Write9Bit(uint16_t AWord) {
    uint32_t dw = AWord;
    dw = __RBIT(dw);
    dw >>= 23;
    while(!(USART3->SR & USART_SR_TXE));    // wait for empty TX buffer
    USART3->DR = dw & 0x000001FF;
}

void Lcd_t::WriteCmd(uint16_t ACmd, uint8_t AParamCount, ...) {
    XCS_Lo();   // Select chip
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
    while(!(USART3->SR & USART_SR_TC));    // wait for transmission to complete
    XCS_Hi();
}

uint8_t Lcd_t::ReadByte(uint8_t ACmd) {
    XCS_Lo();   // Select chip
    // Send Cmd 0 bit and Cmd byte
    Write9Bit(ACmd);
    // Read data
    uint8_t Rslt = 0;
    PinSetupIn(LCD_GPIO, LCD_SDA, pudNone);
    for(uint8_t i=0; i<8; i++) {
        Rslt <<= 1;
        if(PinIsSet(LCD_GPIO, LCD_SDA)) Rslt |= 0x01;
    }
    XCS_Hi();
    PinSetupOut(LCD_GPIO, LCD_SDA,  omPushPull);
    return Rslt;
}

uint32_t Lcd_t::Read24(uint8_t ACmd) {
    XCS_Lo();   // Select chip
    // Send Cmd 0 bit and Cmd byte
    Write9Bit(ACmd);
    // Read data
    uint32_t Rslt = 0;
    PinSetupIn(LCD_GPIO, LCD_SDA, pudNone);
    // Dummy clock cycle
//    SCLK_Hi();
//    SCLK_Lo();
    // Read data
    for(uint8_t i=0; i<24; i++) {
        Rslt <<= 1;
//        SCLK_Hi();
        if(PinIsSet(LCD_GPIO, LCD_SDA)) Rslt |= 0x01;
//        SCLK_Lo();
    }
    XCS_Hi();
    PinSetupOut(LCD_GPIO, LCD_SDA,  omPushPull);
    return Rslt;
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
void Lcd_t::PutChar(char c, int x, int y, uint16_t fColor, uint16_t bColor) {
    y += 6;
    uint16_t i,j;
    uint16_t nCols;
    uint16_t nRows;
    unsigned int nBytes;
    unsigned char PixelRow;
    unsigned char Mask;
    uint16_t Word0;
    //uint16_t Word1;
    unsigned char *pFont;
    unsigned char *pChar;

    // get pointer to the beginning of the selected font table
    pFont = 0;//(unsigned char *)FONT8x8;
    // get the nColumns, nRows and nBytes
    nCols = 8;
    nRows = 8;
    nBytes = 8;
    // get pointer to the first byte of the desired character
    pChar = pFont + (nBytes * (c - 0x1F));

    WriteCmd(0x2A, 4, 0, y, 0, y+nCols-1);    // Set column bounds
    WriteCmd(0x2B, 4, 0, x, 0, x+nRows-1);    // Set row bounds

    // WRITE MEMORY
    XCS_Lo();           // Select chip
    Write9Bit(0x02C);    // Send Cmd byte
//    SCLK_Lo();
    // loop on each row, working backwards from the bottom to the top
    for(i = 0; i < nRows; i++) {
        // copy pixel row from font table and then decrement row
        PixelRow = *pChar++;
        // loop on each pixel in the row (left to right)
        // Note: we do two pixels each loop
        Mask = 0x80;
        for (j = 0; j < nCols; j++) {
            if ((PixelRow & Mask) == 0) Word0 = bColor;
            else                        Word0 = fColor;
            Mask = Mask >> 1;
            Write9Bit(0x100 | ((Word0 >> 8) & 0x00FF));
            Write9Bit(0x100 | (Word0 & 0x00FF));
        }
//        for (j = 0; j < nCols; j += 2) {
//            // if pixel bit set, use foreground color; else use the background color
//            // now get the pixel color for two successive pixels
//            if ((PixelRow & Mask) == 0) Word0 = bColor;
//            else                        Word0 = fColor;
//            Mask = Mask >> 1;
//            if ((PixelRow & Mask) == 0) Word1 = bColor;
//            else                        Word1 = fColor;
//            Mask = Mask >> 1;
//            // use this information to output three data bytes
//            Write9Bit(0x100 | ((Word0 >> 4) & 0xFF));
//            Write9Bit(0x100 | (((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF)));
//            Write9Bit(0x100 | (Word1 & 0xFF));
//        }
    }
    // terminate the Write Memory command
    while(!(USART3->SR & USART_SR_TC));
    XCS_Hi();
}

void Lcd_t::PutStr(const char *pString, int x, int y, uint16_t fColor, uint16_t bColor) {
    // loop until null-terminator is seen
    while (*pString != 0x00) {
        // draw the character
        PutChar(*pString++, x, y, fColor, bColor);
        // advance the y position
        y = y + 8;
        // bail out if y exceeds 131
        if (y > 160) break;
    }
}
/*

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
*/
// ================================ Graphics ===================================
void Lcd_t::Cls(Color_t Color) {
    uint8_t x=1, y=1, w=160, h=130;
    WriteCmd(0x2A, 4, 0, x, 0, x+w-1);    // Set column bounds
    WriteCmd(0x2B, 4, 0, y, 0, y+h-1);    // Set row bounds
    // Write data
    XCS_Lo();           // Select chip
    Write9Bit(CMD_RAM_WRITE);
    // 16 bit
    for (uint16_t i=0; i<w*h; i++) {
        Write9Bit(0x100 | 0x00);
        Write9Bit(0x100 | 0x0F);
    }
    while(!(USART3->SR & USART_SR_TC));
    XCS_Hi();
}
/*
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
//    //USART3->CR3 = USART_CR3_DMAT;   // Enable DMA for transmission
    USART3->CR1 =
            USART_CR1_OVER8 |   // Use 8 samples per bit, not 16 - to increase speed
            USART_CR1_UE |      // Enable USART
            USART_CR1_M |       // 9 data bit
            USART_CR1_TE;       // Transmitter enable
    // After enabling transmitter, it will send empty character
    while(!(USART3->SR & USART_SR_TC));    // wait for transmission to complete
}
