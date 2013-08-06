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

// ================================ PWM pin ====================================
void PwmPin_t::Init(GPIO_TypeDef *GPIO, uint16_t N, uint8_t TimN, uint8_t Chnl, uint16_t TopValue, bool Inverted) {
    PinSetupAlterFuncOutput(GPIO, N, omPushPull);
    switch(TimN) {
        case 1:
            Tim = TIM1;
            rccEnableTIM1(FALSE);
            break;
        case 2:
            Tim = TIM2;
            rccEnableTIM2(FALSE);
            break;
        case 3:
            Tim = TIM3;
            rccEnableTIM3(FALSE);
            break;
        case 4:
            Tim = TIM4;
            rccEnableTIM4(FALSE);
            break;
        case 15:
            Tim = TIM15;
            rccEnableAPB2(RCC_APB2ENR_TIM15EN, FALSE);
            break;
        case 16:
            Tim = TIM16;
            rccEnableAPB2(RCC_APB2ENR_TIM16EN, FALSE);
            break;
        case 17:
            Tim = TIM17;
            rccEnableAPB2(RCC_APB2ENR_TIM17EN, FALSE);
            break;
        default: return; break;
    }

    // Common
    Tim->CR1 = TIM_CR1_CEN; // Enable timer, set clk division to 0, AutoReload not buffered
    Tim->CR2 = 0;
    Tim->ARR = TopValue;
    Tim->BDTR = TIM_BDTR_MOE | TIM_BDTR_AOE;

    // Output
    uint16_t tmp = Inverted? 0b111 : 0b110; // PWM mode 1 or 2
    switch(Chnl) {
        case 1:
            PCCR = &Tim->CCR1;
            Tim->CCMR1 |= (tmp << 4);
            Tim->CCER  |= TIM_CCER_CC1E;
            break;

        case 2:
            PCCR = &Tim->CCR2;
            Tim->CCMR1 |= (tmp << 12);
            Tim->CCER  |= TIM_CCER_CC2E;
            break;

        case 3:
            PCCR = &Tim->CCR3;
            Tim->CCMR2 |= (tmp << 4);
            Tim->CCER  |= TIM_CCER_CC3E;
            break;

        case 4:
            PCCR = &Tim->CCR4;
            Tim->CCMR2 |= (tmp << 12);
            Tim->CCER  |= TIM_CCER_CC4E;
            break;
        default: break;
    }
    *PCCR = 0;
}


// ================================ Timer ======================================
void Timer_t::Init(TIM_TypeDef* PTmr) {
    ITmr = PTmr;
    if     (ITmr == TIM1)  { rccEnableTIM1(FALSE); }
    else if(ITmr == TIM2)  { rccEnableTIM2(FALSE); }
    else if(ITmr == TIM3)  { rccEnableTIM3(FALSE); }
    else if(ITmr == TIM4)  { rccEnableTIM4(FALSE); }
    else if(ITmr == TIM6)  { rccEnableAPB1(RCC_APB1ENR_TIM6EN,  FALSE); }
    else if(ITmr == TIM7)  { rccEnableAPB1(RCC_APB1ENR_TIM7EN,  FALSE); }
    else if(ITmr == TIM15) { rccEnableAPB2(RCC_APB2ENR_TIM15EN, FALSE); }
    else if(ITmr == TIM16) { rccEnableAPB2(RCC_APB2ENR_TIM16EN, FALSE); }
    else if(ITmr == TIM17) { rccEnableAPB2(RCC_APB2ENR_TIM17EN, FALSE); }
    // Clock src
    if(ANY_OF_4(ITmr, TIM1, TIM15, TIM16, TIM17)) PClk = &Clk.APB2FreqHz;
    else PClk = &Clk.APB1FreqHz;
}

void Timer_t::PwmInit(GPIO_TypeDef *GPIO, uint16_t N, uint8_t Chnl, Inverted_t Inverted) {
    // GPIO
    PinSetupAlterFuncOutput(GPIO, N, omPushPull, ps50MHz);  // Timer output
    // Enable outputs for advanced timers
    ITmr->BDTR = TIM_BDTR_MOE | TIM_BDTR_AOE;
    // Output
    uint16_t tmp = (Inverted == invInverted)? 0b111 : 0b110; // PWM mode 1 or 2
    switch(Chnl) {
        case 1:
            PCCR = &ITmr->CCR1;
            ITmr->CCMR1 |= (tmp << 4);
            ITmr->CCER  |= TIM_CCER_CC1E;
            break;

        case 2:
            PCCR = &ITmr->CCR2;
            ITmr->CCMR1 |= (tmp << 12);
            ITmr->CCER  |= TIM_CCER_CC2E;
            break;

        case 3:
            PCCR = &ITmr->CCR3;
            ITmr->CCMR2 |= (tmp << 4);
            ITmr->CCER  |= TIM_CCER_CC3E;
            break;

        case 4:
            PCCR = &ITmr->CCR4;
            ITmr->CCMR2 |= (tmp << 12);
            ITmr->CCER  |= TIM_CCER_CC4E;
            break;

        default: break;
    }
}


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
        dmaStreamSetMemory0(PDMAStream, TXBuf);
        dmaStreamSetTransactionSize(PDMAStream, Cnt);
        dmaStreamEnable(PDMAStream);
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
void DbgUart_t::Init(USART_TypeDef *PUart, uint32_t ABaudrate) {
    PWrite = TXBuf;
    PRead = TXBuf;
    ICountToSendNext = 0;
    IDmaIsIdle = true;
    IUart = PUart;

    if(IUart == USART1) {
        PDMAStream = STM32_DMA1_STREAM4;
        PinSetupAlterFuncOutput(GPIOA, 9, omPushPull);      // TX1
        rccEnableUSART1(FALSE);     // UART clock
    }
    else if(IUart == USART2) {
        PDMAStream = STM32_DMA1_STREAM7;
        PinSetupAlterFuncOutput(GPIOA, 2, omPushPull);      // TX2
        rccEnableUSART2(FALSE);     // UART clock
    }

    // ==== USART configuration ====
    IUart->BRR = Clk.APB2FreqHz / ABaudrate;
    IUart->CR2 = 0;
    IUart->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    IUart->CR1 = USART_CR1_TE;     // Transmitter enabled

    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (PDMAStream, IRQ_PRIO_MEDIUM, DbgUartIrq, NULL);
    dmaStreamSetPeripheral(PDMAStream, &IUart->DR);
    dmaStreamSetMode      (PDMAStream,
            DMA_PRIORITY_HIGH |
            STM32_DMA_CR_MSIZE_BYTE |
            STM32_DMA_CR_PSIZE_BYTE |
            STM32_DMA_CR_MINC |         // Memory pointer increase
            STM32_DMA_CR_DIR_M2P |      // Direction is memory to peripheral
            STM32_DMA_CR_TCIE           // Enable Transmission Complete IRQ
             );
    IUart->CR1 |= USART_CR1_UE;        // Enable USART
}


// ==== IRQs ====
void DbgUart_t::IRQDmaTxHandler() {
    dmaStreamDisable(PDMAStream);    // Registers may be changed only when stream is disabled
    if(ICountToSendNext == 0) IDmaIsIdle = true;
    else {  // There is something to transmit more
        dmaStreamSetMemory0(PDMAStream, PRead);
        // Handle pointer
        uint32_t BytesLeft = UART_TXBUF_SIZE - (PRead - TXBuf);
        if(ICountToSendNext < BytesLeft) {  // Data fits in buffer without split
            dmaStreamSetTransactionSize(PDMAStream, ICountToSendNext);
            PRead += ICountToSendNext;
            ICountToSendNext = 0;
        }
        else {  // Some portion of data placed in the beginning
            dmaStreamSetTransactionSize(PDMAStream, BytesLeft);
            PRead = TXBuf;  // Set pointer to beginning
            ICountToSendNext -= BytesLeft;
        }
        dmaStreamEnable(PDMAStream);    // Restart DMA
    }
}
#endif

