/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f2xx.h"
#include <stdarg.h>
#include <string.h>
#include "tiny_sprintf.h"

// ============================== UART command =================================
DbgUart_t Uart;
static char UartBuf[198];

void DbgUart_t::Printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    uint32_t Cnt = tiny_vsprintf(UartBuf, format, args);
    va_end(args);
    if(Cnt > UART_TXBUF_SIZE) Cnt = UART_TXBUF_SIZE;    // Shrink too long string

    if(IDmaIsIdle) {
        memcpy(TXBuf, UartBuf, Cnt);// Place string to buffer from beginning
        PWrite = TXBuf + Cnt;       // Prepare pointer for next time
        PRead = TXBuf + Cnt;        // Prepare pointer for next time
        ICountToSendNext = 0;       // Reset next-time counter
        // Start DMA
        IDmaIsIdle = false;
        dmaStreamSetMemory0(STM32_DMA2_STREAM7, TXBuf);
        dmaStreamSetTransactionSize(STM32_DMA2_STREAM7, Cnt);
        dmaStreamEnable(STM32_DMA2_STREAM7);
    }
    else {
        ICountToSendNext += Cnt;
        uint32_t BytesFree = UART_TXBUF_SIZE - (PWrite - TXBuf);
        if(Cnt < BytesFree) {   // Message fits in buffer, no splitting needed
            memcpy(PWrite, UartBuf, Cnt);
            PWrite += Cnt;
        }
        else { // Cnt >= BytesFree
            memcpy(PWrite, UartBuf, BytesFree);
            uint32_t Remainder = Cnt - BytesFree;
            if(Remainder) memcpy(TXBuf, &UartBuf[BytesFree], Remainder);
            PWrite = TXBuf + Remainder;
        }
    } // if not idle
}

// ==== Init & DMA ====
// Wrapper for IRQ
extern "C" {
void DbgUartIrq(void *p, uint32_t flags) { Uart.IRQDmaTxHandler(); }
}
void DbgUart_t::Init(uint32_t ABaudrate) {
    PWrite = TXBuf;
    PRead = TXBuf;
    ICountToSendNext = 0;
    IDmaIsIdle = true;
    PinSetupAlterFunc(GPIOA, 9, omPushPull, pudNone, AF7);      // TX1

    // ==== USART configuration ====
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;   // UART clock
    // Integer part computing in case Oversampling mode is 16 Samples
    uint32_t integerdivider = ((25 * 8000000) / (4 * (ABaudrate)));
    uint32_t tmpreg = (integerdivider / 100) << 4;
    uint32_t fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    USART1->BRR = tmpreg;
    USART1->CR2 = 0;
    USART1->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    USART1->CR1 = USART_CR1_TE;     // Transmitter enabled

    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (STM32_DMA2_STREAM7, 1, DbgUartIrq, NULL);
    dmaStreamSetPeripheral(STM32_DMA2_STREAM7, &USART1->DR);
    dmaStreamSetMode      (STM32_DMA2_STREAM7,
            STM32_DMA_CR_CHSEL(4) |     // DMA2 Stream7 Channel4 is USART1_TX request
            DMA_PRIORITY_LOW |
            STM32_DMA_CR_MSIZE_BYTE |
            STM32_DMA_CR_PSIZE_BYTE |
            STM32_DMA_CR_MINC |         // Memory pointer increase
            STM32_DMA_CR_DIR_M2P |      // Direction is memory to peripheral
            STM32_DMA_CR_TCIE           // Enable Transmission Complete IRQ
             );
    USART1->CR1 |= USART_CR1_UE;        // Enable USART
}


// ==== IRQs ====
void DbgUart_t::IRQDmaTxHandler() {
    dmaStreamDisable(STM32_DMA2_STREAM7);    // Registers may be changed only when stream is disabled
    if(ICountToSendNext == 0) IDmaIsIdle = true;
    else {  // There is something to transmit more
        dmaStreamSetMemory0(STM32_DMA2_STREAM7, PRead);
        // Handle pointer
        uint32_t BytesLeft = UART_TXBUF_SIZE - (PRead - TXBuf);
        if(ICountToSendNext < BytesLeft) {  // Data fits in buffer without split
            dmaStreamSetTransactionSize(STM32_DMA2_STREAM7, ICountToSendNext);
            PRead += ICountToSendNext;
            ICountToSendNext = 0;
        }
        else {  // Some portion of data placed in the beginning
            dmaStreamSetTransactionSize(STM32_DMA2_STREAM7, BytesLeft);
            PRead = TXBuf;  // Set pointer to beginning
            ICountToSendNext -= BytesLeft;
        }
        dmaStreamEnable(STM32_DMA2_STREAM7);    // Restart DMA
    }
}
