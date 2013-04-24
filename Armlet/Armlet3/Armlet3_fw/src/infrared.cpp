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
    dmaStreamDisable(IR_TX_DMA_STREAM);
    IR.IStopModulator(); // Stop master timer
    IR.IsBusy = false;
}
} // "C"

void Infrared_t::Init() {
    // ==== Carrier timer ====
    Carrier.Init();
    Carrier.Enable();
    Carrier.PwmInit(IR_CARRIER_GPIO, IR_CARRIER_PIN, IR_CARRIER_CHNL, invNotInverted);
    Carrier.SetUpdateFrequency(IR_CARRIER_HZ);
    MaxPower = Carrier.GetTopValue() / 2;
    // Setup master-slave
    Carrier.SetTriggerInput(IR_CARRIER_TRG_IN);  // Slave using TIM5 as input (ITR2)
    Carrier.SlaveModeSelect(smReset);            // Slave mode: reset on trigger
    Carrier.MasterModeSelect(mmReset);           // Master mode: trigger output on Reset
    Carrier.DmaOnTriggerEnable();                // Request DMA on trigger output == req on Reset = req on input
    //Carrier.PwmSet(MaxPower);   // Debug

    // ==== Modulation timer ====
    Modulator.Init();
    Modulator.Enable();
    Modulator.SetTopValue(IR_TICK_US-1);    // Delay in us
    Modulator.MasterModeSelect(mmUpdate);   // Master mode: Update is used as TRGO
    Modulator.SetupPrescaler(1000000);      // Input Freq: 1 MHz => one tick = 1 uS
    Modulator.Disable();

    // Debug
//    PinSetupAlterFunc(GPIOA, 0, omPushPull, pudNone, AF2);  // Debug pin
//    TIM5->CCMR1 = (0b110<<4);   // PWM mode1
//    TIM5->CCR1 = IR_TICK_US/2;
//    TIM5->CCER = TIM_CCER_CC1E; // Enable output1

    // ==== DMA ==== Here only the unchanged parameters of the DMA are configured
    dmaStreamAllocate     (IR_TX_DMA_STREAM, IRQ_PRIO_LOW, IrTxcIrq, NULL);
    dmaStreamSetPeripheral(IR_TX_DMA_STREAM, Carrier.PCCR);
    dmaStreamSetMemory0   (IR_TX_DMA_STREAM, TxPwrBuf);
//    //dmaStreamSetPeripheral(IR_TX_DMA_STR, &TIM1->CCR1);
    dmaStreamSetMode      (IR_TX_DMA_STREAM,
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

uint8_t Infrared_t::TransmitWord(uint16_t wData, uint8_t PwrPercent) {
//    Uart.Printf("T\r");
    if(IsBusy) return BUSY;
    IsBusy = true;
    // Calculate power
    uint32_t Pwr = (MaxPower * PwrPercent) / 100;
    // Fill buffer with powers depending on data
    uint16_t *p = TxPwrBuf;
    // Header
    *p++ = Pwr;
    *p++ = Pwr;
    *p++ = Pwr;
    *p++ = Pwr;
    // Data
    for(uint8_t i=0; i<IR_BIT_CNT; i++) {
        *p++ = 0;                       // Off = Space
        *p++ = Pwr;                     // '0' is single ON
        if(wData & 0x2000) *p++ = Pwr;  // '1' is double ON
        wData <<= 1;
    }
    *p++ = 0;                           // Off - finishing delay

    // Enable DMA
    uint8_t CountToTx = p - TxPwrBuf;
    dmaStreamSetTransactionSize(IR_TX_DMA_STREAM, CountToTx);
    dmaStreamEnable(IR_TX_DMA_STREAM);
    // Start master timer
    Modulator.GenerateUpdateEvt();  // Generate update: clear counter and request DMA
    Modulator.Enable();
    return OK;
}


