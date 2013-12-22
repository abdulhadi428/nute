#include "lcd1200.h"
#include "lcd_font.h"
#include "string.h"
#include <stdarg.h>

#include "tiny_sprintf.h"

#define LCD_DMA_TX_MODE     DMA_PRIORITY_LOW | \
                            STM32_DMA_CR_MSIZE_HWORD | \
                            STM32_DMA_CR_PSIZE_HWORD | \
                            STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                            STM32_DMA_CR_DIR_M2P |    /* Direction is memory to peripheral */ \
                            STM32_DMA_CR_CIRC
                           // STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */

Lcd_t Lcd;

void Lcd_t::Init(void) {
    BckLt.Init(LCD_BCKLT_GPIO, LCD_BCKLT_PIN, LCD_BCKLT_TMR, LCD_BCKLT_CHNL, LCD_TOP_BRIGHTNESS);
    // ==== GPIOs ====
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    PinSetupOut(LCD_BCKLT_GPIO, LCD_XRES, omPushPull);
    PinSetupOut(LCD_BCKLT_GPIO, LCD_XCS,  omPushPull);
    PinSetupOut(LCD_BCKLT_GPIO, LCD_SCLK, omPushPull);
    PinSetupOut(LCD_BCKLT_GPIO, LCD_SDA,  omPushPull);
    // ========================= Init LCD ======================================
    SCLK_Lo();
    XCS_Hi();
    // Reset display
    XRES_Lo();
    chThdSleepMilliseconds(7);
    XRES_Hi();
    WriteCmd(0xAF);    // display ON
    // Reset display again
    XRES_Lo();
    chThdSleepMilliseconds(7);
    XRES_Hi();
    chThdSleepMilliseconds(7);
    // Initial commands
    WriteCmd(0xAF);    // display ON
    WriteCmd(0xA4);    // Set normal display mode
    WriteCmd(0x2F);    // Charge pump on
    WriteCmd(0x40);    // Set start row address = 0

    //WriteCmd(0xC8);    // Mirror Y axis
    WriteCmd(0xA1);    // Mirror X axis
    // Set x=0, y=0
    WriteCmd(0xB0);    // Y axis initialization
    WriteCmd(0x10);    // X axis initialisation1
    WriteCmd(0x00);    // X axis initialisation2
    Cls();             // clear LCD buffer

    // ====================== Switch to USART + DMA ============================
    PinSetupAlterFunc(LCD_GPIO, LCD_SCLK, omPushPull, pudNone, AF7, ps40MHz);
    PinSetupAlterFunc(LCD_GPIO, LCD_SDA,  omPushPull, pudNone, AF7, ps40MHz);
    // ==== USART init ==== clock enabled, idle low, first edge, enable last bit pulse
    rccEnableUSART3(FALSE);
    USART3->CR1 = USART_CR1_UE;     // Enable
    USART3->BRR = Clk.APB1FreqHz / 100000;
    USART3->CR2 = USART_CR2_CLKEN | USART_CR2_LBCL; // Enable clock, enable last bit clock
    USART3->CR1 = USART_CR1_UE | USART_CR1_M | USART_CR1_TE;
    USART3->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    // DMA
    dmaStreamAllocate     (STM32_DMA1_STREAM2, IRQ_PRIO_LOW, nullptr, NULL);
    dmaStreamSetPeripheral(STM32_DMA1_STREAM2, &USART3->DR);
    dmaStreamSetMemory0   (STM32_DMA1_STREAM2, IBuf);
    dmaStreamSetTransactionSize(STM32_DMA1_STREAM2, LCD_VIDEOBUF_SIZE);
    dmaStreamSetMode      (STM32_DMA1_STREAM2, LCD_DMA_TX_MODE);
    // Start transmission
    XCS_Lo();
    dmaStreamEnable(STM32_DMA1_STREAM2);
}


void Lcd_t::Shutdown(void) {
//    DMA_Cmd(DMA1_Channel2, DISABLE);
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

void Lcd_t::Cls() {
    for (uint32_t i=0; i < LCD_VIDEOBUF_SIZE; i++) IBuf[i] = 0x0001;
}
// ================================= Printf ====================================
void Lcd_t::Printf(const uint8_t x, const uint8_t y, const char *S, ...) {
    GotoCharXY(x, y);
    char FBuf[17];
    // Printf to buffer
    va_list args;
    va_start(args, S);
    uint32_t Cnt = tiny_vsprintf(FBuf, 17, S, args);
    va_end(args);
    // Draw what printed
    for(uint32_t i=0; i<Cnt; i++) DrawChar(FBuf[i], NotInverted);
}

// ================================ Graphics ===================================
/*
 * Prints char at specified buf indx, returns next indx
 */
void Lcd_t::DrawChar(uint8_t AChar, Invert_t AInvert) {
    uint8_t b;
    uint16_t w;
    for(uint8_t i=0; i<6; i++) {
        b = Font_6x8_Data[AChar][i];
        if(AInvert == Inverted) b = ~b;
        w = b;
        w = (w << 1) | 0x0001;
        IBuf[CurrentPosition++] = w;
        if(CurrentPosition >= LCD_VIDEOBUF_SIZE) CurrentPosition = 0;
    }
}

void Lcd_t::DrawImage(const uint8_t x, const uint8_t y, const uint8_t* Img) {
    uint16_t i=0;
    uint8_t Width = Img[i++], Height = Img[i++];
    for(uint8_t fy=y; fy<y+Height; fy++) {
        GotoXY(x, fy);
        for(uint8_t fx=x; fx<x+Width; fx++) {
            IBuf[CurrentPosition++] = Img[i++];
            if(CurrentPosition >= LCD_VIDEOBUF_SIZE) continue;
        } // fx
    } // fy
}

#ifdef LCD_LARGEFONTS_H_ // ================== LargeFonts ======================
#include "cmd_uart.h"

void Lcd_t::PrintfFont(const uint8_t *PFont, uint8_t x, uint8_t y, const char *S, ...) {
    uint32_t height = PFont[2], MaxWidth = PFont[1];
    uint8_t FirstSymbolCode = PFont[0];
    char c;
    uint16_t w;
    while((c = *S) != 0) {
        // ==== Draw char ====
        uint8_t *P = (uint8_t*)PFont + 3 + (c - FirstSymbolCode) * (MaxWidth*height + 1);  // Pointer to char
        uint32_t width = 1 + *P++;
        uint32_t Indx;
        for(uint8_t i=0; i<width; i++) {
            for(uint8_t h=0; h<height; h++) {
                Indx = x + i + (y + h) * 96;
                uint32_t dw = *P++;
                dw = __RBIT(dw);
                dw >>= 23;
                w = dw | 0x0001;
                IBuf[Indx] = w;
            }
        }
        S++;
        x += width;
    }
}
#endif

