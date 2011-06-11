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

    // ==== DMA IRQ ====
    NVIC_InitTypeDef NVIC_InitStructure;
    // Enable DMA1 channel3 IRQ Channel
    NVIC_InitStructure.NVIC_IRQChannel = LCD_DMA_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable DMA Transfer Complete interrupt
    DMA_ITConfig(LCD_DMA_CHNL, DMA_IT_TC, ENABLE);

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
    Ram.Cmd[6] = Distort(LCD_CMD, 0xC8);    // mirror Y axis
    Ram.Cmd[6] = Distort(LCD_CMD, 0xA1);    // Mirror X axis
#endif
    // Start transmission
    XCS_Lo();
    DMA_Cmd(LCD_DMA_CHNL, ENABLE);          // Enable USARTy DMA TX Channel
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
}
// ================================ Graphics ===================================
void Lcd_t::Cls() {
    for (uint16_t i = 0; i < LCD_VIDEOBUF_SIZE; i++) Ram.Data[i] = ((0xA2 << 1) | 0x01);
}
//void LCD_DrawImage(const uint8_t x, const uint8_t y, prog_uint8_t *I, bool AInvert) {
//    uint8_t Width = pgm_read_byte(I++), Height = pgm_read_byte(I++);
//    uint8_t b;
//    for(uint8_t fy=y; fy<y+Height; fy++) {
//        LCD_GotoXY(x, fy);
//        for(uint8_t fx=x; fx<x+Width; fx++) {
//            b = pgm_read_byte(I++);
//            if(AInvert) b = ~b;
//            LCD_WriteData(b);
//        } // fx
//    } // fy
//
//}

//void LCD_PrintUint(const uint8_t x, const uint8_t y, uint16_t ANumber, bool AInvert) {
//    uint8_t digit = '0';
//    bool ShouldPrint = false;
//    const uint16_t FArr[9] = {10000, 1000, 100, 10};
//    LCD_GotoXYstr(x, y);
//    // Iterate until ANumber > 10
//    for(uint8_t i=0; i<4; i++) {
//        while (ANumber >= FArr[i]) {
//            digit++;
//            ANumber -= FArr[i];
//        }
//        if((digit != '0') || ShouldPrint) {
//                LCD_DrawChar(digit, AInvert);
//                ShouldPrint = true;
//        }
//        digit = '0';
//    }
//    // Print last digit
//    LCD_DrawChar('0'+ANumber, AInvert);
//}
//void LCD_PrintInt(const uint8_t x, const uint8_t y, int16_t ANumber, bool AInvert) {
//    LCD_GotoXYstr(x, y);
//    if(ANumber < 0) {
//        LCD_DrawChar('-', AInvert);
//        LCD_PrintUint(x+1, y, -ANumber, AInvert);
//    }
//    else LCD_PrintUint(x, y, -ANumber, AInvert);
//}
//// Print uint ANumber, 0<=ANumber<=99
//void LCD_PrintUint0_99(const uint8_t x, const uint8_t y, uint8_t ANumber, bool AInvert) {
//    uint8_t digit = '0';
//    LCD_GotoXYstr(x, y);
//    while(ANumber >= 10) {
//        digit++;
//        ANumber -= 10;
//    }
//    LCD_DrawChar(digit, AInvert);
//    LCD_DrawChar('0'+ANumber, AInvert);
//}
//

//
//void LCD_PrintTime(uint8_t x, uint8_t y, bool InvertHours, bool InvertMinTens, bool InvertMinUnits) {
//    LCD_GotoXYstr(x, y);
//    // Print hours
//    uint8_t t = Time.Hour;
//    uint8_t digit = '0';
//    while(t >= 10) {
//        digit++;
//        t -= 10;
//    }
//    LCD_DrawChar(digit, InvertHours);
//    LCD_DrawChar('0'+t, InvertHours);
//    // Print delimiter
//    LCD_DrawChar(':', false);
//    // Print minutes
//    t = Time.Minute;
//    digit = '0';
//    while(t >= 10) {
//        digit++;
//        t -= 10;
//    }
//    LCD_DrawChar(digit, InvertMinTens);
//    LCD_DrawChar('0'+t, InvertMinUnits);
//}


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
        // Set and reset LCD_CS to sync next frame
        Lcd.XCS_Hi();
        // Clear DMA1 Channel3 Half Transfer, Transfer Complete and Global interrupt pending bits
        DMA_ClearITPendingBit(DMA1_IT_GL2);
        Lcd.XCS_Lo();
        UART_Print('i');
    } // if IT_TC3
}