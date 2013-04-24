/*
 * infrared.cpp
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
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
    MaxPower = tmp / 2;
    Carrier.SetReloadValue(tmp);
    //Carrier.Init(GPIOC, 7, IR_CARRIER_TMR_N, IR_CARRIER_CHNL, tmp);
    Carrier.SetTriggerInput(IR_CARRIER_TRG_IN);  // Slave using TIM5 as input (ITR2)
    Carrier.SlaveModeSelect(smReset);            // SLave mode: reset on trigger
    Carrier.MasterModeSelect(mmReset);           // Master mode: trigger output on Reset
    Carrier.DmaOnTriggerEnable();                // Request DMA on trigger output == req on Reset = req on input
    //Carrier.On(MaxPower);   // Debug

    // ==== Modulation timer ====
    IR_MODULATION_TMR_RCC_EN();
//    TIM5->ARR = IR_TICK_US-1;   // Delay
//    TIM5->CR2 = (0b010<<4);     // Master mode: Update is used as TRGO
//    TIM5->PSC = (Clk.APB1FreqHz / 1000000) - 1; // Input Freq: 1 MHz => one tick = 1 uS

    // Debug
//    PinSetupAlterFunc(GPIOA, 0, omPushPull, pudNone, AF2);  // Debug pin
//    TIM5->CCMR1 = (0b110<<4);   // PWM mode1
//    TIM5->CCR1 = IR_TICK_US/2;
//    TIM5->CCER = TIM_CCER_CC1E; // Enable output1

    // ==== DMA ==== Here only the unchanged parameters of the DMA are configured
//    dmaStreamAllocate     (IR_TX_DMA_STR, 1, IrTxcIrq, NULL);
//    dmaStreamSetPeripheral(IR_TX_DMA_STR, Carrier.PCCR);
//    dmaStreamSetMemory0(IR_TX_DMA_STR, Buf);
//    //dmaStreamSetPeripheral(IR_TX_DMA_STR, &TIM1->CCR1);
//    dmaStreamSetMode      (IR_TX_DMA_STR,
//            STM32_DMA_CR_CHSEL(IR_TX_DMA_CHNL)
//            | DMA_PRIORITY_MEDIUM
//            | STM32_DMA_CR_MSIZE_HWORD
//            | STM32_DMA_CR_PSIZE_HWORD
//            | STM32_DMA_CR_MINC         // Memory pointer increase
//            | STM32_DMA_CR_DIR_M2P      // Direction is memory to peripheral
//           // | STM32_DMA_CR_CIRC         // Enable circular buffer
//            | STM32_DMA_CR_TCIE         // Enable Transmission Complete IRQ
//            );

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


