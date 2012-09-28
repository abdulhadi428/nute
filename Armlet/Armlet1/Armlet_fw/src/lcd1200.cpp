#include "lcd1200.h"
#include "kl_lib.h"
#include "lcd_font.h"
#include "string.h"
#include <stdarg.h>

#include "tiny_sprintf.h"

Lcd_t Lcd;

void Lcd_t::Task(void) {

}

void Lcd_t::Init(void) {
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

    // ==== GPIOs ====
    // Configure LCD_XRES, LCD_XCS, LCD_SCLK & LCD_SDA as Push-Pull output
    klGpioSetupByMsk(LCD_GPIO, LCD_XRES | LCD_XCS | LCD_SCLK | LCD_SDA, GPIO_Mode_Out_PP);

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

    draw_mode = OVERWRITE;

    // ====================== Switch to USART + DMA ============================
    klGpioSetupByMsk(LCD_GPIO, LCD_SCLK | LCD_SDA, GPIO_Mode_AF_PP);
    // Workaround hardware bug with disabled CK3 when SPI2 is enabled
    SPI2->CR2 |= SPI_CR2_SSOE;
    // ==== USART init ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    // Usart clock: enabled, idle low, first edge, enable last bit pulse
    USART_ClockInitTypeDef USART_ClockInitStructure;
    USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
    USART_ClockInit(USART3, &USART_ClockInitStructure);
    // Usart itself
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 100000;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART3, ENABLE);
    // ==== DMA ====
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&IBuf[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = LCD_VIDEOBUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    // Enable USARTy DMA TX request
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    // Start transmission
    XCS_Lo();
    DMA_Cmd(DMA1_Channel2, ENABLE);          // Enable USARTy DMA TX Channel
}


void Lcd_t::Shutdown(void) {
    DMA_Cmd(DMA1_Channel2, DISABLE);
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
	assert(y % 8 == 0);
	y /= 8;
    uint8_t width = *img++;
    uint8_t height = *img++;
    for(int fy = y; fy < y+height; fy++) {
        int index = x + y*LCD_WIDTH;
        for(int fx = x; fx < x+width; fx++) {
            DrawBlock(index++, *img++, 255);
            if (index > LCD_VIDEOBUF_SIZE) break;
        }
    }
}

void Lcd_t::DrawBlock(int index, uint8_t data, uint8_t mask) {
    assert((data & ~mask) == 0);
    uint16_t *w = &IBuf[index];
    uint16_t data2 = data << 1;
    uint16_t mask2 = mask << 1;
	switch (draw_mode) {
        case DRAW: 	                *w |= data2;		                    break;
        case CLEAR:	                *w &= ~data2;		                    break;
        case OVERWRITE:             *w = (*w & ~mask2) | data2;             break;
        case OVERWRITE_INVERTED:    *w = (*w & ~mask2) | (data2 ^ mask2);    break;
        case INVERT:                *w ^= data2;		                    break;
	}
}
