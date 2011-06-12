#include "lcd110x.h"
#include "lcd_font.h"
#include "images.h"

#include "delay_util.h"
#include "uart.h"

Lcd_t Lcd;

void Lcd_t::Init(void) {
    // ====================== Hardware setup ===================================
    // ==== GPIO init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
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
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
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

    // ==== DMA ====
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    LCD_DMA_Init();

    // ==== DMA IRQ ====
    NVIC_InitTypeDef NVIC_InitStructure;
    // Enable DMA1 channel3 IRQ Channel
    NVIC_InitStructure.NVIC_IRQChannel = LCD_DMA_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ========================= Init LCD ======================================
    XCS_Hi();
    // Reset display
    XRES_Lo();
    Delay.ms(7);
    XRES_Hi();
    // Initial commands
    Cls(); // clear LCD
    Ram.Cmd[0] = Distort(LCD_CMD, 0xA4);    // Set normal display mode
    Ram.Cmd[1] = Distort(LCD_CMD, 0x2F);    // Charge pump on
    Ram.Cmd[2] = Distort(LCD_CMD, 0x40);    // Set start row address = 0
    Ram.Cmd[3] = Distort(LCD_CMD, 0xAF);    // display ON
    // Set x=0, y=0
    Ram.Cmd[4] = Distort(LCD_CMD, 0xB0);    // Y axis initialization
    Ram.Cmd[5] = Distort(LCD_CMD, 0x00);    // X axis initialisation1
    Ram.Cmd[6] = Distort(LCD_CMD, 0x10);    // X axis initialisation2
#ifdef LCD_UPSIDEDOWN
    Ram.Cmd[7] = Distort(LCD_CMD, 0xC8);    // mirror Y axis
    Ram.Cmd[8] = Distort(LCD_CMD, 0xA1);    // Mirror X axis
#endif
    // Start transmission
    XCS_Lo();
    DMA_Cmd(LCD_DMA_CHNL, ENABLE);          // Enable USARTy DMA TX Channel
}

void Lcd_t::LCD_DMA_Init() {
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(LCD_DMA_CHNL);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&LCD_USART->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) CommonBuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = LCD_BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(LCD_DMA_CHNL, &DMA_InitStructure);
    // Enable USARTy DMA TX request
    USART_DMACmd(LCD_USART, USART_DMAReq_Tx, ENABLE);
    // Enable DMA Transfer Complete interrupt
    DMA_ITConfig(LCD_DMA_CHNL, DMA_IT_TC, ENABLE);
}

void Lcd_t::Shutdown(void) {
    DMA_Cmd(LCD_DMA_CHNL, DISABLE);
    XRES_Lo();
    XCS_Lo();
}
// ================================ Graphics ===================================
void Lcd_t::Cls() {
    for (uint16_t i = 0; i < LCD_VIDEOBUF_SIZE; i++)
        Ram.Data[i] = 0x0001;   // Empty character
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
        Ram.Data[CurrentPosition++] = Distort(LCD_DATA, b);
        if (CurrentPosition >= LCD_VIDEOBUF_SIZE) CurrentPosition = 0;
    }
}

void Lcd_t::PrintString(const uint8_t x, const uint8_t y, const char* S, Invert_t AInvert) {
    GotoCharXY(x, y);
    while (*S != '\0')
        DrawChar(*S++, AInvert);
}

void Lcd_t::DrawImage(const uint8_t x, const uint8_t y, const uint8_t* Img, Invert_t AInvert) {
    uint16_t i=0;
    uint8_t Width = Img[i++], Height = Img[i++];
    uint8_t b;
    for(uint8_t fy=y; fy<y+Height; fy++) {
        GotoXY(x, fy);
        for(uint8_t fx=x; fx<x+Width; fx++) {
            b = Img[i++];
            if(AInvert) b = ~b;
            Ram.Data[CurrentPosition++] = Distort(LCD_DATA, b);
            if (CurrentPosition >= LCD_VIDEOBUF_SIZE) CurrentPosition = 0;
        } // fx
    } // fy
}

// ============================ Inner use ======================================
uint16_t Lcd_t::Distort(uint16_t CmdDta, uint8_t AByte) {
    uint32_t Itmp = (uint32_t) AByte;
    __ASM volatile ("rbit %0, %1" : "=r" (Itmp) : "r" (Itmp)); // Reverse bit order
    __ASM volatile ("rev %0, %1" : "=r" (Itmp) : "r" (Itmp)); // Reverse byte order
    Itmp <<= 1;
    Itmp |= CmdDta;
    return (uint16_t)Itmp;
}

// ================================== Interrupts ===============================
void DMA1_Channel2_IRQHandler(void) {
    // Test on DMA1 Channel3 Transfer Complete interrupt
    if(DMA_GetITStatus(DMA1_IT_TC2)) {
        // Clear DMA Half Transfer, Transfer Complete and Global interrupt pending bits
        DMA_ClearITPendingBit(DMA1_IT_GL2);
        DMA_Cmd(LCD_DMA_CHNL, DISABLE);
        // Wait for current byte to finish transfer
        while(USART_GetFlagStatus(LCD_USART, USART_FLAG_TC) == RESET);
        Lcd.XCS_Hi();   // End transmission
        Delay.Loop(9);
        Lcd.XCS_Lo();   // Start transmission
        DMA_Cmd(LCD_DMA_CHNL, ENABLE);
    } // if
}