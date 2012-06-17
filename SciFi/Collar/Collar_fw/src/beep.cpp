/*
 * beep.cpp
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#include "beep.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

Beep_t Beep;

void Beep_t::Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    IPwm.Init(TIM4, 200, 1600, 4, TIM_OCPolarity_High);
    IPwm.Disable();
    // Setup GPIO
    klGpioSetupByN(GPIOB, 9, GPIO_Mode_AF_PP);
}

void Beep_t::On() {
    IsOn = true;
    IPwm.Set(Volume);
    IPwm.SetFreqHz(Freq);
#ifndef BEEP_DISABLE
    IPwm.Enable();
#endif
}
