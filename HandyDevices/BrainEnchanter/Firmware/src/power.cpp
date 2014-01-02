/*
 * current.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: kreyl
 */

#include "power.h"
#include "evt_mask.h"
#include "cmd_uart.h"

#if 1 // ============================== Current ================================
Current_t Current;

void Current_t::InitHardware() {
    PinSetupOut(GPIOA, 15, omPushPull); // 12V_enable pin
    HighVDisable();
    // ==== DAC ====
    rccEnableAPB1(RCC_APB1ENR_DACEN, FALSE);
    DAC->CR = DAC_CR_EN1 | DAC_CR_BOFF1;   // Channel1 enabled, buffer disabled
    DAC->DHR12R1 = 0;
    Off();
}

void Current_t::On() {
    PinSetupAnalog(GPIOA, 4);
    //uint32_t w = ((uA * 3800) - 104000) / 5902;   // with dac buf enabled
    //uint32_t w = uA;
    uint32_t w = ((uA * 2500) - 104000) / 2452; // With dac buf disabled
    DAC->DHR12R1 = w;
}

void Current_t::Off() {
    DAC->DHR12R1 = 0;
    PinSetupOut(GPIOA, 4, omPushPull, pudNone);
    PinClear(GPIOA, 4);
}

#endif

#if 1 // ========================= Measurement =================================
//class Adc_t {
//private:
//    inline void SetupClk(ADCDiv_t Div) { ADC->CCR |= (uint32_t)Div; }
//    void SetChannelCount(uint32_t Count);
//    void StartConversion() { ADC1->CR2 |= ADC_CR2_SWSTART; }
//    void Enable() { ADC1->CR2 = ADC_CR2_ADON; }
//    void ChannelConfig(AdcChnl_t ACfg);
////    void ContModeEnable() { ADC1->CFGR1 = ADC_CFGR1_CONT; }  // Enable continuos conversion
//    inline bool ConversionCompleted() { return (ADC1->SR & ADC_SR_EOC); }
//    inline uint16_t IResult() { return ADC1->DR; }
//public:
//    uint16_t Result[ADC_BUF_SZ];
//    void Init();
//    void Measure();
//    void Disable();
//    void ClockOff() { rccDisableADC1(FALSE); }
//};
Measure_t Measure;

// Measurement completed IRQ
extern "C" {
void AdcTxIrq(void *p, uint32_t flags) {
    dmaStreamDisable(ADC_DMA);
    Measure.StopAdc();
    // Resume thread if any
    if(Measure.PThreadToSignal != nullptr) {
        chSysLockFromIsr();
        chEvtSignalI(Measure.PThreadToSignal, EVTMSK_MEASUREMENT_DONE);
        chSysUnlockFromIsr();
    }
}
} // extern C

// ==== Implementation ====
void Measure_t::InitHardware() {
    rccEnableADC1(FALSE);   // Enable digital clock
    Clk.HSIEnable();        // Enable HSI, as ADC clocks from HSI
    SetupClk(adcDiv4);      // Setup ADCCLK
    // Setup channels
    SetSequenceLength(ADC_SEQ_LEN);
    uint8_t SeqIndx = 1;    // First sequence item is 1, not 0
    for(uint8_t i=0; i < ADC_CHANNEL_CNT; i++) {
        SetChannelSampleTime(AdcChannels[i], ADC_SAMPLE_TIME);
        for(uint8_t j=0; j<ADC_SAMPLE_CNT; j++) SetSequenceItem(SeqIndx++, AdcChannels[i]);
    }
    // DMA
    dmaStreamAllocate     (ADC_DMA, IRQ_PRIO_LOW, AdcTxIrq, NULL);
    dmaStreamSetPeripheral(ADC_DMA, &ADC1->DR);
    dmaStreamSetMode      (ADC_DMA, ADC_DMA_MODE);
}

void Measure_t::SetSequenceLength(uint32_t ALen) {
    ADC1->SQR1 &= ~ADC_SQR1_L;  // Clear count
    ADC1->SQR1 |= (ALen - 1) << 20;
}
void Measure_t::SetChannelSampleTime(uint32_t AChnl, AdcSampleTime_t ASampleTime) {
    uint32_t Offset;
    if(AChnl <= 9) {
        Offset = AChnl * 3;
        ADC1->SMPR3 &= ~((uint32_t)0b111 << Offset);    // Clear bits
        ADC1->SMPR3 |= (uint32_t)ASampleTime << Offset; // Set new bits
    }
    else if(AChnl <= 19) {
        Offset = (AChnl - 10) * 3;
        ADC1->SMPR2 &= ~((uint32_t)0b111 << Offset);    // Clear bits
        ADC1->SMPR2 |= (uint32_t)ASampleTime << Offset; // Set new bits
    }
    else if(AChnl <= 29) {
        Offset = (AChnl - 20) * 3;
        ADC1->SMPR1 &= ~((uint32_t)0b111 << Offset);    // Clear bits
        ADC1->SMPR1 |= (uint32_t)ASampleTime << Offset; // Set new bits
    }
    else {
        Offset = (AChnl - 30) * 3;
        ADC1->SMPR0 &= ~((uint32_t)0b111 << Offset);    // Clear bits
        ADC1->SMPR0 |= (uint32_t)ASampleTime << Offset; // Set new bits
    }
}
void Measure_t::SetSequenceItem(uint8_t SeqIndx, uint32_t AChnl) {
    uint32_t Offset;
    if(SeqIndx <= 6) {
        Offset = (SeqIndx - 1) * 5;
        ADC1->SQR5 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR5 |= (uint32_t)(AChnl << Offset);
    }
    else if(SeqIndx <= 12) {
        Offset = (SeqIndx - 7) * 5;
        ADC1->SQR4 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR4 |= (uint32_t)(AChnl << Offset);
    }
    else if(SeqIndx <= 18) {
        Offset = (SeqIndx - 13) * 5;
        ADC1->SQR3 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR3 |= (uint32_t)(AChnl << Offset);
    }
    else if(SeqIndx <= 24) {
        Offset = (SeqIndx - 19) * 5;
        ADC1->SQR2 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR2 |= (uint32_t)(AChnl << Offset);
    }
    else if(SeqIndx <= 27) {    // 28 in high and medium density, 27 in others
        Offset = (SeqIndx - 25) * 5;
        ADC1->SQR1 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR1 |= (uint32_t)(AChnl << Offset);
    }
}

void Measure_t::StartMeasurement() {
    // DMA
    dmaStreamSetMemory0(ADC_DMA, IBuf);
    dmaStreamSetTransactionSize(ADC_DMA, ADC_SEQ_LEN);
    dmaStreamSetMode(ADC_DMA, ADC_DMA_MODE);
    dmaStreamEnable(ADC_DMA);
    // ADC
    ADC1->CR1 = ADC_CR1_SCAN;
    ADC1->CR2 = ADC_CR2_DMA | ADC_CR2_ADON;
    while(!(ADC1->SR & ADC_SR_ADONS));  // Wait ADC to start
    StartConversion();
}

void Measure_t::StopAdc() {
    ADC1->CR2 = 0;
}

uint32_t Measure_t::GetResult(uint8_t AChannel) {
    uint32_t Indx = 0;
#if (ADC_CHANNEL_CNT > 1)
    // Find Channel indx
    for(uint32_t i=0; i < ADC_CHANNEL_CNT; i++) {
        if(AdcChannels[i] == AChannel) {
            Indx = i;
            break;
        }
    }
#endif
    // Find bounds
    uint32_t Start = Indx * ADC_SAMPLE_CNT;
    uint32_t Stop  = Start + ADC_SAMPLE_CNT;
    // Average values
    uint32_t Rslt = 0;
    for(uint32_t i = Start; i < Stop; i++) Rslt += IBuf[i];
    return Rslt / ADC_SAMPLE_CNT;
}
#endif
