/*
 * lcd.cpp
 *
 *  Created on: 27.10.2013
 *      Author: Roma Jam
 */



#include "lcd.h"

//#define ENABLE_DMAUSART_MODE

Lcd_t Lcd;

// =============================== Thread ======================================
static WORKING_AREA(waLcdThread, 128);
__attribute__ ((__noreturn__))
static void LcdThread(void *arg) {
    chRegSetThreadName("LcdTask");
    while(1) Lcd.Task();
}


#ifdef ENABLE_DMAUSART_MODE
#define LCD_DMA             STM32_DMA1_STREAM2
#define LCD_DMA_MODE        DMA_PRIORITY_LOW |                              \
                            STM32_DMA_CR_MSIZE_HWORD | /* Size byte */          \
                            STM32_DMA_CR_PSIZE_HWORD |                          \
                            STM32_DMA_CR_MINC |    /* Mem pointer increase */  \
                            STM32_DMA_CR_DIR_M2P | /* Mem to peripheral */     \
                            STM32_DMA_CR_TCIE      /* Enable Transmission Int C */ \
                            STM32_DMA_CR_CIRC       /* Circular */

// ==== Inner DMA ====
extern "C" {
    void LcdDmaCompIrq(void *p, uint32_t flags);
}
#endif

void Lcd_t::Task() {
    for (uint8_t i=0; i < LCD_VIDEOBUF_SIZE; i++) {
        WriteData(IBuf[i]);
    }
    WriteCmd(0xB3);    // Y axis initialization
    WriteCmd(0x10);    // X axis initialisation1
    WriteCmd(0x08);    // X axis initialisation2
    if(BackligthValue != 50) {
        BackligthValue++;
        Backlight(BackligthValue);
    }
    chThdSleepMilliseconds(12);
}

void Lcd_t::Init() {
    // ==== Backlight: Timer15 Ch2 ====
    // Setup pin
    PinSetupAlterFuncOutput(LCD_GPIO, LCD_BCKLT, omPushPull, ps50MHz);
    // Remap Timer15 to PB14 & PB15
    AFIO->MAPR2 |= 0x00000001;
    // Setup timer15
    rccEnableAPB2(RCC_APB2ENR_TIM15EN, false);

    TIM15->CR1 = 0x01;      // Enable timer
    TIM15->CR2 = 0;
    TIM15->PSC = 0;         // Do not divide input freq
    TIM15->ARR = 100;       // Autoreload register: full brightness=100
    TIM15->BDTR = 0xC000;   // Main output Enable
    TIM15->CCMR1 = 0x6000;  // PWM mode1 on Ch2 enabled
    TIM15->CCER = 0x0010;   // Output2 enabled, polarity not inverted

    BackligthValue = 0;
    // ==== GPIOs ====
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    InitGpios();

    // ========================= Init LCD ======================================
    SCLK_Lo();
    XCS_Hi();
    // Reset display
    XRES_Lo();
    chThdSleepMilliseconds(9);
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

//    WriteCmd(0xC8);    // mirror Y axis
//    WriteCmd(0xA1);    // Mirror X axis
    // Set x=0, y=0
    WriteCmd(0xB0);    // Y axis initialization
    WriteCmd(0x10);    // X axis initialisation1
    WriteCmd(0x00);    // X axis initialisation2

    Cls();             // clear LCD buffer

    draw_mode = OVERWRITE;

// ====================== Switch to USART + DMA ============================
#ifdef ENABLE_DMAUSART_MODE
    PinSetupAlterFuncOutput(LCD_GPIO, LCD_SCLK, omPushPull, ps50MHz);
    PinSetupAlterFuncOutput(LCD_GPIO, LCD_SDA, omPushPull, ps50MHz);
    // Workaround hardware bug with disabled CK3 when SPI2 is enabled
    SPI2->CR2 |= SPI_CR2_SSOE;
    // ==== USART init ====
    rccEnableUSART3(false);
    // Usart clock: enabled, idle low, first edge, enable last bit pulse

    // Usart itself
    LCD_USART->BRR = Clk.APB1FreqHz / 100000;
    LCD_USART->CR1 = USART_CR1_TE |   /* Transmitter enabled */                 \
                     USART_CR1_M;     /* 9 bit */
    LCD_USART->CR2 = USART_CR2_CLKEN |  \
                     USART_CR2_LBCL;

    LCD_USART->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter

    LCD_USART->CR1 |= USART_CR1_UE;        // Enable USART
    // ==== DMA ====
    rccEnableDMA1();
    dmaStreamAllocate(LCD_DMA, IRQ_PRIO_MEDIUM, LcdDmaCompIrq, NULL);
    dmaStreamSetPeripheral(LCD_DMA, &USART3->DR);
    dmaStreamSetMemory0(LCD_DMA, (uint32_t)&IBuf[0]);
    dmaStreamSetMode      (LCD_DMA, LCD_DMA_MODE);
    // Start transmission
    XCS_Lo();

//    DMA_Cmd(DMA1_Channel2, ENABLE);          // Enable USARTy DMA TX Channel
#else
    for(int i=0; i < 864; i++) WriteData(0x00); // Clear all screen
#endif
    Backlight(0);
    chThdCreateStatic(waLcdThread, sizeof(waLcdThread), NORMALPRIO, (tfunc_t)LcdThread, NULL);
}

void Lcd_t::Shutdown(void) {
#ifdef ENABLE_DMAUSART_MODE
    DMA_Cmd(DMA1_Channel2, DISABLE);
#endif
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


void Lcd_t::Symbols(int column, int row, ...) {
    int index = column*6 + row*LCD_WIDTH;
    uint8_t FCharCode=1, RepeatCount;
    va_list Arg;
    va_start(Arg, row);    // Set pointer to last argument
    while(1) {
        FCharCode = (uint8_t)va_arg(Arg, int32_t);
        if(FCharCode == 0) break;
        RepeatCount = (uint8_t)va_arg(Arg, int32_t);
        for(uint8_t j=0; j<RepeatCount; j++) DrawChar(&index, FCharCode);
    }
    va_end(Arg);
}


void Lcd_t::Printf(int column, int row, const char *format, ...) {
    char buf[LCD_STR_HEIGHT*LCD_STR_WIDTH+1];
    va_list args;
    va_start(args, format);
    tiny_vsprintf(buf, format, args);
    va_end(args);

    int index = column*6 + row*LCD_WIDTH;
    for (int i = 0; buf[i] != 0; i++) DrawChar(&index, buf[i]);
}

// ================================ Graphics ===================================
/*
 * Prints char at specified buf indx, returns next indx
 */
void Lcd_t::DrawChar(int *index, uint8_t AChar) {
    for(uint8_t i=0; i < 6; i++) {
        DrawBlock((*index)++, Font_6x8_Data[AChar][i], 0xFF);
        if (*index >= LCD_VIDEOBUF_SIZE) *index = 0;
    }
}

void Lcd_t::DrawImage(int x, int y, const uint8_t* img) {
//    assert(y % 8 == 0);
    y /= 8; // y is now rows, not pixels
    uint8_t width = *img++;
    uint8_t height = *img++;
    for(int fy = y; fy < y+height; fy++) {
        int index = x + fy*LCD_WIDTH;
        for(int fx = x; fx < x+width; fx++) {
            DrawBlock(index++, *img++, 255);
            if (index > LCD_VIDEOBUF_SIZE) break;
        }
    }
}

void Lcd_t::DrawBlock(int index, uint8_t data, uint8_t mask) {
//    assert((data & ~mask) == 0);
    uint16_t *w = &IBuf[index];
#ifdef ENABLE_DMAUSART_MODE
    uint16_t data2 = data << 1;
    uint16_t mask2 = mask << 1;
#else
    uint16_t data2 = data;
    uint16_t mask2 = mask;
#endif
    switch (draw_mode) {
        case DRAW:                  *w |= data2;                            break;
        case CLEAR:                 *w &= ~data2;                           break;
        case OVERWRITE:             *w = (*w & ~mask2) | data2;             break;
        case OVERWRITE_INVERTED:    *w = (*w & ~mask2) | (data2 ^ mask2);    break;
        case INVERT:                *w ^= data2;                            break;
    }
}
