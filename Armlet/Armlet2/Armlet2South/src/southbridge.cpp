/*
 * bridge.cpp
 *
 *  Created on: 06.02.2013
 *      Author: kreyl
 */

#include "ch.h"
#include "hal.h"
#include "kl_lib_f0.h"
#include "clocking_f0.h"
#include "southbridge.h"
#include "string.h"     // For memcpy
#include "SouthbridgeTxRx.h"

#include "beep.h"

Bridge_t Bridge;

Feeder_t* const PFeeders[] = {
        &Beep,
};
const uint8_t FeederCnt = countof(PFeeders);

static inline void UartInit();

// ================================ Receiver ===================================


// =============================== SouthBridge =================================
void Bridge_t::Init() {
    Transmitter.Init();
    //Rcvr.Init();
    UartInit();
}

void UartInit() {
    // Pins
    PinSetupAlterFunc(GPIOA, 9,  omPushPull,  pudNone,   AF1);
    PinSetupAlterFunc(GPIOA, 10, omOpenDrain, pudPullUp, AF1);

    // ==== USART init ====
    rccEnableUSART1(FALSE);         // Usart1 CLK, no clock in low-power
    USART1->BRR = Clk.APBFreqHz / SB_BAUDRATE;    // Baudrate
    USART1->CR2 = 0;
    USART1->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    USART1->CR1 =
            USART_CR1_TE            // Transmitter enable
            | USART_CR1_RE          // Receiver enable
            | USART_CR1_RXNEIE;     // RX Irq enable
    USART1->ICR = 0xFFFFFFFF;
    nvicEnableVector(STM32_USART1_NUMBER, CORTEX_PRIORITY_MASK(STM32_SERIAL_USART1_PRIORITY));

    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (SB_DMA_STREAM, 1, IrqSBTxCompleted, NULL);
    dmaStreamSetPeripheral(SB_DMA_STREAM, &USART1->TDR);
    dmaStreamSetMode      (SB_DMA_STREAM,
              STM32_DMA_CR_PL(0b10)     // Priority is high
            | STM32_DMA_CR_MSIZE_BYTE
            | STM32_DMA_CR_PSIZE_BYTE
            | STM32_DMA_CR_MINC         // Memory pointer increase
            | STM32_DMA_CR_DIR_M2P      // Direction is memory to peripheral
            | STM32_DMA_CR_TCIE         // Enable Transmission Complete IRQ
            );
    USART1->CR1 |= USART_CR1_UE;        // Enable USART
}
