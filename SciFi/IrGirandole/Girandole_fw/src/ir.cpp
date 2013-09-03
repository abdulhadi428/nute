/*
 * ir.cpp
 *
 *  Created on: 04.07.2013
 *      Author: kreyl
 */

#include "ir.h"
#include "stm32f10x.h"

//#define DAC_CONST   0

void ir_t::Init() {
    chEvtInit(&IEvtSrcTxEnd);
    // ==== GPIO ====
#if IR_DAC
    // Once the DAC channel is enabled, the corresponding GPIO pin is automatically
    // connected to the DAC converter. In order to avoid parasitic consumption,
    // the GPIO pin should be configured in analog.
    PinSetupAnalog(GPIOA, 4);
    // ==== DAC ====
    rccEnableAPB1(RCC_APB1ENR_DACEN, FALSE);
    DAC->CR = DAC_CR_EN1 | DAC_CR_DMAEN1 | (0b010 << 3) | DAC_CR_TEN1;   // TIM7, enable trigger;
    // ZeroArr
    for(uint8_t i=0; i<CARRIER_PERIOD_CNT; i++) ZeroArr[i] = 0;
    // ==== DMA ====
    dmaStreamAllocate     (STM32_DMA1_STREAM3, IRQ_PRIO_MEDIUM, NULL, NULL);
    dmaStreamSetPeripheral(STM32_DMA1_STREAM3, &DAC->DHR12R1);
    dmaStreamSetMode      (STM32_DMA1_STREAM3,
            DMA_PRIORITY_HIGH
            | STM32_DMA_CR_MSIZE_HWORD
            | STM32_DMA_CR_PSIZE_HWORD
            | STM32_DMA_CR_MINC          // Memory pointer increase
            | STM32_DMA_CR_DIR_M2P       // Direction is memory to peripheral
            | STM32_DMA_CR_CIRC);
    dmaStreamSetMemory0(STM32_DMA1_STREAM3, ZeroArr);
    dmaStreamSetTransactionSize(STM32_DMA1_STREAM3, CARRIER_PERIOD_CNT);
    // ==== Sampling timer ====
    SamplingTmr.Init(TIM7);
    SamplingTmr.SetUpdateFrequency(SAMPLING_FREQ_HZ);
    SamplingTmr.MasterModeSelect(mmUpdate);
    SamplingTmr.Enable();
#endif // DAC

#if IR_PWM
    // ==== Carrier generator ====
    CarrierTmr.Init(TIM1);
    CarrierTmr.PwmInit(GPIOA, 8, 1, invNotInverted);
    CarrierTmr.SetUpdateFrequency(IR_CARRIER_HZ);
    CarrierTmr.PwmSet(0);   // Set to zero to output nothing
    CarrierTmr.Enable();    // Start timer-based carrier
#endif
    // ==== Chunk timer ====
//    PinSetupOut(GPIOB, 7, omPushPull);  // DEBUG
    ChunkTmr.Init(TIM16);
    ChunkTmr.SetupPrescaler(1000000);   // Period = 1us
    ChunkTmr.SetTopValue(600);          // Initial, do not care
    ChunkTmr.IrqOnUpdateEnable();
    nvicEnableVector(TIM1_UP_TIM16_IRQn, CORTEX_PRIORITY_MASK(IRQ_PRIO_MEDIUM));
}

void ir_t::TransmitWord(uint16_t wData
#if IR_DAC
        , DacCurrent_t ACurrent
#endif
#if IR_PWM
        , uint8_t PwmPercent
#endif
        ) {
//    if(Busy) return;
    Busy = true;
#if IR_PWM
    // Calculate power
    CarrierTmrPwr = CarrierTmr.GetTopValue() / 2;
    CarrierTmrPwr = (CarrierTmrPwr * PwmPercent) / 100;
#endif
    // ==== Fill buffer depending on data ====
    PChunk = TxBuf;
    *PChunk++ = {1, 2400}; // }
    *PChunk++ = {0, 600};  // } Header
    // Data
    for(uint8_t i=0; i<IR_BIT_CNT; i++) {
        if(wData & 0x8000) *PChunk++ = {1, 1200};  // 1
        else               *PChunk++ = {1, 600};   // 0
        *PChunk++ = {0, 600};                      // space
        wData <<= 1;
    }
//    for(uint8_t i=0; i<CHUNK_CNT; i++) Uart.Printf("%u %u\r", TxBuf[i].Pwr, TxBuf[i].Duration);
//    Uart.Printf("\r");
    // ==== Start transmission ====
    PChunk = TxBuf;
    // Start chunk timer
    ChunkTmr.SetCounter(0);
    ChunkTmr.SetTopValue(PChunk->Duration);
    ChunkTmr.Enable();
#if IR_DAC
    // Fill carrier array
    CarrierArr[0] = (uint16_t)ACurrent;
    CarrierArr[1] = 0;
    IDacCarrierEnable();    // Start DAC-based carrier
#endif
#if IR_PWM
    CarrierTmr.PwmSet(CarrierTmrPwr);
#endif
}

void ir_t::IChunkTmrHandler() {
    // Check if last chunk
    if((PChunk - TxBuf) >= CHUNK_CNT-1) {
        Busy = false;
        ChunkTmr.Disable();
#if IR_DAC
        IDacCarrierDisable(); // Stop Dac
#endif
#if IR_PWM
        CarrierTmr.PwmSet(0);
#endif
        chEvtBroadcast(&IEvtSrcTxEnd);
    }
    else {
        PChunk++;
        ChunkTmr.SetTopValue(PChunk->Duration);
        if(PChunk->On) {
            //PinSet(GPIOB, 7);
#if IR_PWM
            CarrierTmr.PwmSet(CarrierTmrPwr);
#endif
            IDacCarrierEnable();
        }
        else {
            //PinClear(GPIOB, 7);
#if IR_PWM
            CarrierTmr.PwmSet(0);
#endif
            IDacCarrierDisable();
        }
    }
}

extern "C" {
CH_IRQ_HANDLER(VectorA4) {  // TIM16 global irq
    CH_IRQ_PROLOGUE();
    TIM16->SR &= ~TIM_SR_UIF;   // Clear flag
    ir.IChunkTmrHandler();
    CH_IRQ_EPILOGUE();
}
} // extern C

