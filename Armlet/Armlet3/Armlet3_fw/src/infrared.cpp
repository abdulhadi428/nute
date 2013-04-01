/*
 * infrared.cpp
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

/*
 *  TIM5 is used as modulation timer (ModTmr), PwmPin - as carrier timer (CarTmr).
 *  PwmPin generates carrier frequency.
 *  ModTmr outputs a TRGO at every Update event, i.e., every 600us.
 *  CarTmr as slave receives signal from ModTmr and uses it to Reset.
 *  CarTmr outputs a TRGO at every Reset event - i.e., every 600us (since it is
 *  Reset by ModTmr).
 *  CarTmr uses TRGO as DMA request; i.e. DMA request generated at every 600us.
 *  Buffer contains power level for every 600us chunk of packet;
 *  i.e. "PWM on for 2400us, off for 600us" translates to {Pwr, Pwr, Pwr, Pwr, 0}
 *  in Buf. (Pwr is power level).
 */

#include "infrared.h"

Infrared_t IR;

// Wrapper for IRQ
extern "C" {
void IrTxcIrq(void *p, uint32_t flags) {
    dmaStreamDisable(IR_TX_DMA_STR);
    TIM5->CR1 = 0;              // Stop master timer
    IR.IsBusy = false;
}
} // "C"

void Infrared_t::Init() {
    // ==== Carrier timer ====
    uint16_t tmp = (uint16_t)(Clk.AHBFreqHz / IR_CARRIER_HZ);   // Top timer value
    Carrier.Init(GPIOA, 8, 1, 1, tmp);
    IR_CAR_TMR->SMCR = (0b000 << 4) | 0b100;  // Slave using TIM5 as input (ITR0), Reset mode
    IR_CAR_TMR->CR2 = 0;                      // Reset is trigger output
    IR_CAR_TMR->DIER = TIM_DIER_TDE;          // Request DMA on trigger
    MaxPower = tmp / 2;
    //Carrier.On(MaxPower);   // Debug

    // ==== Modulation timer ====
    rccEnableTIM5(FALSE);
    TIM5->ARR = IR_TICK_US-1;   // Delay
    TIM5->CR2 = (0b010<<4);     // Master mode: Update is used as TRGO
    TIM5->PSC = (Clk.APB1FreqHz / 1000000) - 1; // Input Freq: 1 MHz => one tick = 1 uS

    // Debug
//    PinSetupAlterFunc(GPIOA, 0, omPushPull, pudNone, AF2);  // Debug pin
//    TIM5->CCMR1 = (0b110<<4);   // PWM mode1
//    TIM5->CCR1 = IR_TICK_US/2;
//    TIM5->CCER = TIM_CCER_CC1E; // Enable output1

    // ==== DMA ==== Here only the unchanged parameters of the DMA are configured
    dmaStreamAllocate     (IR_TX_DMA_STR, 1, IrTxcIrq, NULL);
    dmaStreamSetPeripheral(IR_TX_DMA_STR, Carrier.PCCR);
    dmaStreamSetMemory0(IR_TX_DMA_STR, Buf);
    //dmaStreamSetPeripheral(IR_TX_DMA_STR, &TIM1->CCR1);
    dmaStreamSetMode      (IR_TX_DMA_STR,
            STM32_DMA_CR_CHSEL(IR_TX_DMA_CHNL)
            | DMA_PRIORITY_MEDIUM
            | STM32_DMA_CR_MSIZE_HWORD
            | STM32_DMA_CR_PSIZE_HWORD
            | STM32_DMA_CR_MINC         // Memory pointer increase
            | STM32_DMA_CR_DIR_M2P      // Direction is memory to peripheral
           // | STM32_DMA_CR_CIRC         // Enable circular buffer
            | STM32_DMA_CR_TCIE         // Enable Transmission Complete IRQ
            );

    // ==== Variables ====
    IsBusy = false;
}

void Infrared_t::TransmitWord(uint16_t wData, uint8_t PwrPercent) {
//    Uart.Printf("T\r");
    if(IsBusy) return;
    IsBusy = true;
    // Calculate power
    uint32_t Pwr = (MaxPower * PwrPercent) / 100;
    // Fill buffer with powers depending on data
    uint16_t *p = Buf;
    for(uint8_t i=0; i<4; i++) *p++ = Pwr;    // Header
    // Data
    for(uint8_t i=0; i<IR_BIT_CNT; i++) {
        *p++ = 0;                       // Off = Space
        *p++ = Pwr;                     // '0' is single ON
        if(wData & 0x2000) *p++ = Pwr;  // '1' is double ON
        wData <<= 1;
    }
    *p++ = 0;                           // Off - finishing delay

    // Enable DMA
    uint8_t CountToTx = p - Buf;
    dmaStreamSetTransactionSize(IR_TX_DMA_STR, CountToTx);
    dmaStreamEnable(IR_TX_DMA_STR);

    // Start master timer
    TIM5->EGR = TIM_EGR_UG;     // Generate update: clear counter and request DMA
    TIM5->CR1 = TIM_CR1_CEN;    // Enable timer, set clk division to 0, AutoReload not buffered
}


