/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led_smooth.h"
#include "clocking_f0.h"

LedSmooth_t Led;

// ==== LED Thread ====
static WORKING_AREA(waLedThread, 128);
static msg_t LedThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Lcd");
    while(1) Led.Task();
    return 0;
}

void LedSmooth_t::Init() {
    // ==== Gpio ====
    PinSetupAlterFunc(GPIOA, 1, omPushPull, pudNone, AF2);
    // ==== Timer ====
    LED_RCC_EN();
    // Timebase and general
    LED_TIM->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    LED_TIM->CR2 = 0;          // Output Idle State
    LED_TIM->PSC = Clk.APBFreqHz / (LED_PWM_FREQ_HZ * LED_TOP_VALUE);
    LED_TIM->ARR = LED_TOP_VALUE;
    // Outputs
    //LED_TIM->BDTR = 0x8000;    // Enable output
    LED_TIM->CCMR1 = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
    LED_TIM->CCER = TIM_CCER_CC2E;

    // ==== Variables ====
    ICurrentValue = 0;
    INeededValue = 0;
    // Thread
    PThread = chThdCreateStatic(waLedThread, sizeof(waLedThread), NORMALPRIO, LedThread, NULL);
}

void LedSmooth_t::Task() {
    if(ICurrentValue == INeededValue) {
        chSysLock();
        IsSleeping = true;
        chSchGoSleepS(THD_STATE_SUSPENDED);
        chSysUnlock();
    }
    else {
        chThdSleepMilliseconds(SmoothDelay(ICurrentValue));
        if (INeededValue < ICurrentValue) ICurrentValue--;
        else ICurrentValue++;
        Set(ICurrentValue);
    }
}

void LedSmooth_t::SetSmoothly(uint16_t AValue) {
    INeededValue = AValue;
    if(IsSleeping) {
        chSysLock();
        chSchWakeupS(PThread, 0);
        IsSleeping = false;
        chSysUnlock();
    }
}
