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
    klGpioSetupByN(GPIOB, 0, GPIO_Mode_AF_PP);
    // ==== Timer ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    // Clock
    // ==== Timebase and general ====
    BEEP_TIM->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    BEEP_TIM->CR2 = 0;
    BEEP_TIM->ARR = BEEP_TOP_VALUE; // Autoreload register
    SetFreqHz(1603);
    // ==== Output ====
    BEEP_TIM->CCMR1 = 0;        // Outputs disabled
    BEEP_TIM->CCMR2 = 0x0060;   // Ch3 is output, PWM mode 1
    BEEP_TIM->CCER = 0x0100;    // Ch3 output enabled, active high
}

void Beep_t::SetFreqHz(uint32_t AFreq) {
    uint32_t FPrescaler = SystemCoreClock / (BEEP_TOP_VALUE * AFreq);
    if (FPrescaler != 0) FPrescaler--;   // do not decrease in case of high freq
    BEEP_TIM->PSC = (uint16_t)FPrescaler;
}

void Beep_t::Task() {
    if(ICounter != 0) {
        if(IsOn) {
            if(Delay.Elapsed(&ITimer, BEEP_ON_LEN)) {
                Off();
                ICounter--;
            } // if delay
        } // if on
        else {
            if(Delay.Elapsed(&ITimer, BEEP_OFF_LEN)) {
                On();
            } // if delay
        }
    } // if ICounter
}

void Beep_t::Squeak(uint32_t ANumber, uint8_t AVolume) {
    ICounter = ANumber;
    IVolume = AVolume;
    On();
    Delay.Reset(&ITimer);
}
