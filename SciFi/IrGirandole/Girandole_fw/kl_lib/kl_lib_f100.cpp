/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f100.h"
#include <stdarg.h>
#include <string.h>
#include "tiny_sprintf.h"

// ================================ Timer ======================================
void Timer_t::Init(TIM_TypeDef* PTmr) {
    ITmr = PTmr;
    if     (ITmr == TIM1)  { rccEnableTIM1(FALSE); }
    else if(ITmr == TIM2)  { rccEnableTIM2(FALSE); }
    else if(ITmr == TIM3)  { rccEnableTIM3(FALSE); }
    //else if(ITmr == TIM4)  { rccEnableTIM4(FALSE); }
    else if(ITmr == TIM6)  { rccEnableAPB1(RCC_APB1ENR_TIM6EN,  FALSE); }
    else if(ITmr == TIM7)  { rccEnableAPB1(RCC_APB1ENR_TIM7EN,  FALSE); }
    else if(ITmr == TIM15) { rccEnableAPB2(RCC_APB2ENR_TIM15EN, FALSE); }
    else if(ITmr == TIM16) { rccEnableAPB2(RCC_APB2ENR_TIM16EN, FALSE); }
    else if(ITmr == TIM17) { rccEnableAPB2(RCC_APB2ENR_TIM17EN, FALSE); }
    // Clock src
    if(ANY_OF_4(ITmr, TIM1, TIM15, TIM16, TIM17)) PClk = &Clk.APB2FreqHz;
    else PClk = &Clk.APB1FreqHz;
}

//void Timer_t::PwmInit(GPIO_TypeDef *GPIO, uint16_t N, uint8_t Chnl, Inverted_t Inverted) {
//    // GPIO
//    if     (ANY_OF_2(ITmr, TIM1, TIM2))       PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
//    else if(ANY_OF_3(ITmr, TIM3, TIM4, TIM5)) PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
//    else if(ANY_OF_4(ITmr, TIM8, TIM9, TIM10, TIM11)) PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
//    else if(ANY_OF_3(ITmr, TIM12, TIM13, TIM14)) PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
//    // Enable outputs for advanced timers
//    ITmr->BDTR = TIM_BDTR_MOE | TIM_BDTR_AOE;
//    // Output
//    uint16_t tmp = (Inverted == invInverted)? 0b111 : 0b110; // PWM mode 1 or 2
//    switch(Chnl) {
//        case 1:
//            PCCR = &ITmr->CCR1;
//            ITmr->CCMR1 |= (tmp << 4);
//            ITmr->CCER  |= TIM_CCER_CC1E;
//            break;
//
//        case 2:
//            PCCR = &ITmr->CCR2;
//            ITmr->CCMR1 |= (tmp << 12);
//            ITmr->CCER  |= TIM_CCER_CC2E;
//            break;
//
//        case 3:
//            PCCR = &ITmr->CCR3;
//            ITmr->CCMR2 |= (tmp << 4);
//            ITmr->CCER  |= TIM_CCER_CC3E;
//            break;
//
//        case 4:
//            PCCR = &ITmr->CCR4;
//            ITmr->CCMR2 |= (tmp << 12);
//            ITmr->CCER  |= TIM_CCER_CC4E;
//            break;
//
//        default: break;
//    }
//}


// ============================== UART command =================================
#ifdef DBG_UART_ENABLED
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
        dmaStreamSetMemory0(STM32_DMA1_STREAM4, TXBuf);
        dmaStreamSetTransactionSize(STM32_DMA1_STREAM4, Cnt);
        dmaStreamEnable(STM32_DMA1_STREAM4);
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
    PinSetupAlterFuncOutput(GPIOA, 9, omPushPull);      // TX1

    // ==== USART configuration ====
    rccEnableUSART1(FALSE);     // UART clock
    USART1->BRR = Clk.APB2FreqHz / ABaudrate;
    USART1->CR2 = 0;
    USART1->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    USART1->CR1 = USART_CR1_TE;     // Transmitter enabled

    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (STM32_DMA1_STREAM4, IRQ_PRIO_MEDIUM, DbgUartIrq, NULL);
    dmaStreamSetPeripheral(STM32_DMA1_STREAM4, &USART1->DR);
    dmaStreamSetMode      (STM32_DMA1_STREAM4,
            DMA_PRIORITY_HIGH |
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
    dmaStreamDisable(STM32_DMA1_STREAM4);    // Registers may be changed only when stream is disabled
    if(ICountToSendNext == 0) IDmaIsIdle = true;
    else {  // There is something to transmit more
        dmaStreamSetMemory0(STM32_DMA1_STREAM4, PRead);
        // Handle pointer
        uint32_t BytesLeft = UART_TXBUF_SIZE - (PRead - TXBuf);
        if(ICountToSendNext < BytesLeft) {  // Data fits in buffer without split
            dmaStreamSetTransactionSize(STM32_DMA1_STREAM4, ICountToSendNext);
            PRead += ICountToSendNext;
            ICountToSendNext = 0;
        }
        else {  // Some portion of data placed in the beginning
            dmaStreamSetTransactionSize(STM32_DMA1_STREAM4, BytesLeft);
            PRead = TXBuf;  // Set pointer to beginning
            ICountToSendNext -= BytesLeft;
        }
        dmaStreamEnable(STM32_DMA1_STREAM4);    // Restart DMA
    }
}
#endif

