/*
 * current.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: kreyl
 */

#include "power.h"
#include "evt_mask.h"

#if 1 // ============================== Current ================================
Current_t Current;

void Current_t::InitHardware() {
    PinSetupOut(GPIOA, 15, omPushPull); // 12V_enable pin
    DisableHighV();
    // ==== DAC ====
    PinSetupAnalog(GPIOA, 4);
    rccEnableAPB1(RCC_APB1ENR_DACEN, FALSE);
    DAC->CR = DAC_CR_EN1;   // Channel1 enabled, buffer disabled
    DAC->DHR12R1 = 0;

}

void Current_t::On() {
    uint32_t w = ((uA * 3800) - 104000) / 5902;
    DAC->DHR12R1 = w;
}
#endif

#if 1 // ========================= Measurement =================================
void Measure_t::InitHardware() {

}

void Measure_t::StartMeasurement() {

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
    uint32_t Stop  = (Indx + 1) * ADC_SAMPLE_CNT;
    // Average values
    uint32_t Rslt = 0;
    for(uint32_t i = Start; i < Stop; i++) Rslt += IBuf[i];
    return Rslt / ADC_SAMPLE_CNT;
}
#endif
