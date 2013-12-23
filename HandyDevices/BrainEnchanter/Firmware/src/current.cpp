/*
 * current.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: kreyl
 */

#include "current.h"

Current_t Current;

void Current_t::Init() {
    PinSetupOut(GPIOA, 15, omPushPull); // 12enable pin
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
