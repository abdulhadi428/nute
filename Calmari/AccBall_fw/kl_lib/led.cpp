/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led.h"

#if LED_SMOOTH // ===================== LedSmooth_t ============================
LedSmooth_t LedSmooth;

// Timer callback
static void LedTmrCallback(void *p) {
    LedSmooth.IrqHandlerI();
}

void LedSmooth_t::Init() {
    IPins[0].Init(LED_GPIO, 0, 3,  3, LED_TOP_VALUE, false);
    IPins[1].Init(LED_GPIO, 1, 3,  4, LED_TOP_VALUE, false);
    IPins[2].Init(LED_GPIO, 8, 16, 1, LED_TOP_VALUE, false);
    IPins[3].Init(LED_GPIO, 9, 17, 1, LED_TOP_VALUE, false);

    IPins[0].SetFreqHz(LED_PWM_FREQ_HZ);
    IPins[1].SetFreqHz(LED_PWM_FREQ_HZ);
    IPins[2].SetFreqHz(LED_PWM_FREQ_HZ);
    IPins[3].SetFreqHz(LED_PWM_FREQ_HZ);
    // Initial value
    Set(LED_INITIAL_VALUE);
    INeededValue = LED_INITIAL_VALUE;
    IState = slsNone;
}

void LedSmooth_t::SetSmoothly(uint16_t AValue) {
    IState = slsNone;
    if(INeededValue == AValue) return;
    chVTReset(&ITmr);
    INeededValue = AValue;
    uint32_t Delay = ISetupDelay(ICurrentValue);
    chVTSet(&ITmr, MS2ST(Delay), LedTmrCallback, NULL);
}

void LedSmooth_t::Glimmer(uint16_t AMax, uint16_t AMin) {
    chVTReset(&ITmr);
    IState = slsGlimmer;
    IMax = AMax;
    IMin = AMin;
    if(ICurrentValue < IMax) INeededValue = IMax;
    else INeededValue = IMin;
    uint32_t Delay = ISetupDelay(ICurrentValue);
    chVTSet(&ITmr, MS2ST(Delay), LedTmrCallback, NULL);
}

void LedSmooth_t::IrqHandlerI() {
    if (INeededValue < ICurrentValue) ICurrentValue--;
    else ICurrentValue++;
    IPins[0].Set(ICurrentValue);
    IPins[1].Set(ICurrentValue);
    IPins[2].Set(ICurrentValue);
    IPins[3].Set(ICurrentValue);

    // if equal and glimmer needed, switch to another
    if(IState == slsGlimmer) {
        if(ICurrentValue == IMax) INeededValue = IMin;
        else if(ICurrentValue == IMin) INeededValue = IMax;
    }

    if(ICurrentValue != INeededValue) {
        uint32_t Delay = ISetupDelay(ICurrentValue);
        chSysLockFromIsr();
        chVTSetI(&ITmr, MS2ST(Delay), LedTmrCallback, NULL);
        chSysUnlockFromIsr();
    }
}
#endif



#if LED_RGB_ENABLE

LedRGB_t Led;

// ==== LED Thread ====
static WORKING_AREA(waLedThread, 128);
static void LedThread(void *arg) {
    chRegSetThreadName("Lcd");
    while(true) Led.Task();
}

void LedRGB_t::Init() {
    // ==== GPIO ====
	PinSetupAlterFuncOutput(GPIOA, 1, omPushPull);
	PinSetupAlterFuncOutput(GPIOA, 2, omPushPull);
	PinSetupAlterFuncOutput(GPIOA, 3, omPushPull);
    // ==== Timer ====
	LED_RCC_EN();
    // ==== Timebase and general ====
	LED_TIM->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
	LED_TIM->CR2 = 0;          // Output Idle State
	LED_TIM->PSC = 0;          // No clock division
	LED_TIM->ARR = 255;        // Autoreload register: top value of PWM
    // ==== Outputs ====
	//LED_TIM->BDTR = 0x8000;    // Enable output
	const uint16_t OutCmpBits = 0b01100000;   // output, PWM1
	LED_TIM->CCMR1 = OutCmpBits << 8;   // CCR2
	LED_TIM->CCMR2 = (OutCmpBits << 8) | OutCmpBits;
	LED_TIM->CCER = TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
    // Initial values
    SetColor(clBlack);
    INeededColor = clBlack;
    // Thread
    PThread = chThdCreateStatic(waLedThread, sizeof(waLedThread), NORMALPRIO, (tfunc_t)LedThread, NULL);
}

void LedRGB_t::SetColorSmoothly(Color_t AColor) {
    INeededColor = AColor;
    if(IsSleeping) {
        chSysLock();
        chSchWakeupS(PThread, 0);
        IsSleeping = false;
        chSysUnlock();
    }
}

void LedRGB_t::Task(void) {
    if(ICurrentColor == INeededColor) {
        chSysLock();
        IsSleeping = true;
        chSchGoSleepS(THD_STATE_SUSPENDED);
        chSysUnlock();
    }
    else {
        chThdSleepMilliseconds(LED_SETUP_DELAY_MS);
        // Red channel
        if (ICurrentColor.Red != INeededColor.Red) {
            if(INeededColor.Red < ICurrentColor.Red) ICurrentColor.Red--;
            else ICurrentColor.Red++;
        }
        // Green channel
        if (ICurrentColor.Green != INeededColor.Green) {
            if(INeededColor.Green < ICurrentColor.Green) ICurrentColor.Green--;
            else ICurrentColor.Green++;
        }
        // Blue channel
        if (ICurrentColor.Blue != INeededColor.Blue) {
            if(INeededColor.Blue < ICurrentColor.Blue) ICurrentColor.Blue--;
            else ICurrentColor.Blue++;
        }
        SetColor(ICurrentColor);
    }
}
#endif
