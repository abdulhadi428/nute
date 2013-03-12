/*
 * infrared.cpp
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

#include "infrared.h"

Infrared_t IR;

void Infrared_t::Init() {
    // ==== Carrier timer ====
    uint16_t tmp = (uint16_t)(Clk.AHBFreqHz / IR_CARRIER_HZ);   // Top timer value
    Carrier.Init(GPIOA, 8, 1, 1, tmp);
    MaxPower = tmp / 2;
    Carrier.On(MaxPower);

    // ==== Modulation timer ====
    PinSetupAlterFunc(GPIOA, 0, omPushPull, pudNone, AF2);  // Debug pin
    rccEnableTIM5(FALSE);
    TIM5->CNT = 0;              // Reset timer
    TIM5->ARR = BufDelays[0];   // First delay
    TIM5->CR1 = TIM_CR1_CEN;    // Enable timer, set clk division to 0, AutoReload not buffered
    // Master mode: Compare1 is used as TRGO
    TIM5->CR2 = 0b100<<4


    // First delay is always header
    BufDelays[0] = IR_HEADER_US;
}

void Infrared_t::TransmitWord(uint16_t wData, uint16_t Power) {
    // Fill buffer with delays depending on data
    for(uint8_t i=0; i<IR_BIT_NUMBER; i++) {
        if(wData & 0x2000) BufDelays[i+1] = IR_ONE_US;
        else BufDelays[i+1] = IR_ZERO_US;
        wData <<= 1;
    }
    // Enable DMA

    // Setup carrier
    if(Power > MaxPower) Power = MaxPower;
    // Start master timer
}


