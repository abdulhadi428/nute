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

// ================================ PWM pin ====================================
void PwmPin_t::Init(GPIO_TypeDef *GPIO, uint16_t N, uint8_t TimN, uint8_t Chnl, uint16_t TopValue, bool Inverted) {
    switch(TimN) {
        case 1:
            Tim = TIM1;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
            rccEnableTIM1(FALSE);
            break;
        case 2:
            Tim = TIM2;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
            rccEnableTIM2(FALSE);
            break;

        case 3:
            Tim = TIM3;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM3(FALSE);
            break;
        case 4:
            Tim = TIM4;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM4(FALSE);
            break;
        case 5:
            Tim = TIM5;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM5(FALSE);
            break;

        case 8:
            Tim = TIM8;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableTIM8(FALSE);
            break;
        case 9:
            Tim = TIM9;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableAPB2(RCC_APB2ENR_TIM9EN, FALSE);
            break;
        case 10:
            Tim = TIM10;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableAPB2(RCC_APB2ENR_TIM10EN, FALSE);
            break;
        case 11:
            Tim = TIM11;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
            rccEnableAPB2(RCC_APB2ENR_TIM11EN, FALSE);
            break;

        case 12:
            Tim = TIM12;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
            rccEnableAPB1(RCC_APB1ENR_TIM12EN, FALSE);
            break;
        case 13:
            Tim = TIM13;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
            rccEnableAPB1(RCC_APB1ENR_TIM13EN, FALSE);
            break;
        case 14:
            Tim = TIM14;
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
            rccEnableAPB1(RCC_APB1ENR_TIM14EN, FALSE);
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

void PwmPin_t::SetFreqHz(uint32_t FreqHz) {
    uint32_t divider = Tim->ARR * FreqHz;
    if(divider == 0) return;
    uint32_t FPrescaler = Clk.APB1FreqHz / divider;
    if(FPrescaler != 0) FPrescaler--;   // do not decrease in case of high freq
    Tim->PSC = (uint16_t)FPrescaler;
}

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
        dmaStreamSetMemory0(UART_DMA, TXBuf);
        dmaStreamSetTransactionSize(UART_DMA, Cnt);
        dmaStreamEnable(UART_DMA);
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
    PinSetupAlterFunc(UART_GPIO, UART_TX_PIN, omPushPull, pudNone, UART_AF);

    // ==== USART configuration ====
    UART_RCC_ENABLE();
    UART->BRR = Clk.APB2FreqHz / ABaudrate;
    UART->CR2 = 0;
    UART->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    UART->CR1 = USART_CR1_TE;     // Transmitter enabled

    // ==== DMA ====
    // Here only unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (UART_DMA, 1, DbgUartIrq, NULL);
    dmaStreamSetPeripheral(UART_DMA, &UART->DR);
    dmaStreamSetMode      (UART_DMA,
            STM32_DMA_CR_CHSEL(UART_DMA_CHNL) |
            DMA_PRIORITY_LOW |
            STM32_DMA_CR_MSIZE_BYTE |
            STM32_DMA_CR_PSIZE_BYTE |
            STM32_DMA_CR_MINC |         // Memory pointer increase
            STM32_DMA_CR_DIR_M2P |      // Direction is memory to peripheral
            STM32_DMA_CR_TCIE           // Enable Transmission Complete IRQ
             );
    UART->CR1 |= USART_CR1_UE;        // Enable USART
}


// ==== IRQs ====
void DbgUart_t::IRQDmaTxHandler() {
    dmaStreamDisable(UART_DMA);    // Registers may be changed only when stream is disabled
    if(ICountToSendNext == 0) IDmaIsIdle = true;
    else {  // There is something to transmit more
        dmaStreamSetMemory0(UART_DMA, PRead);
        // Handle pointer
        uint32_t BytesLeft = UART_TXBUF_SIZE - (PRead - TXBuf);
        if(ICountToSendNext < BytesLeft) {  // Data fits in buffer without split
            dmaStreamSetTransactionSize(UART_DMA, ICountToSendNext);
            PRead += ICountToSendNext;
            ICountToSendNext = 0;
        }
        else {  // Some portion of data placed in the beginning
            dmaStreamSetTransactionSize(UART_DMA, BytesLeft);
            PRead = TXBuf;  // Set pointer to beginning
            ICountToSendNext -= BytesLeft;
        }
        dmaStreamEnable(UART_DMA);    // Restart DMA
    }
}
