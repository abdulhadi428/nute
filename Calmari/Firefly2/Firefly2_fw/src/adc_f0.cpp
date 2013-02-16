/*
 * adc_f0.cpp
 *
 *  Created on: 16.02.2013
 *      Author: kreyl
 */

#include "adc_f0.h"
#include "hal.h"
#include "kl_lib_f0.h"

void Adc_t::Init() {
    rccEnableADC1(FALSE);
    PinSetupAnalog(GPIOB, 1, pudNone);

    ChannelConfig(ADC_Channel_9, ADC_SampleTime_55_5Cycles);
    Calibrate();    // Must be performed when ADC is disabled
    Enable();       // May configure only enabled ADC
}

void Adc_t::ChannelConfig(uint32_t Channel, uint32_t SampleTime) {
    ADC1->CHSELR |= (uint32_t)Channel;  // Configure the ADC Channel
    ADC1->SMPR = SampleTime;
}

void Adc_t::Calibrate() {
    ADC1->CR |= (uint32_t)ADC_CR_ADCAL;
    // Wait until ADC calibration is completed
    while(ADC1->CR & ADC_CR_ADCAL);
}
