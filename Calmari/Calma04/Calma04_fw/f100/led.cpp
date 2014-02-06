/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led.h"
#include "evt_mask.h"

LedSmooth_t Led;

// Timer callback
static inline void LedTmrCallback(void *p) { Led.IIrqHandler(); }

void LedSmooth_t::Init(uint16_t InitialValue, Thread *PEvtThd) {
    IPEvtThd = PEvtThd;
    IPin.Init(LED_GPIO, LED_GPIO_N, LED_TIM_N, LED_CCR_N, LED_TOP_VALUE, false);
    IPin.SetFreqHz(LED_PWM_FREQ_HZ);
    // Initial value
    Set(InitialValue);
    INeededValue = InitialValue;
    IState = slsNone;
}

void LedSmooth_t::SetSmoothly(uint16_t AValue) {
    chVTReset(&ITmr);
    IState = slsNone;
    if(INeededValue == AValue) return;
    INeededValue = AValue;
    uint32_t Delay = ICalculateDelay(ICurrentValue);
    chVTSet(&ITmr, MS2ST(Delay), LedTmrCallback, NULL);
}

void LedSmooth_t::Glimmer(uint16_t AMax, uint16_t AMin) {
    chVTReset(&ITmr);
    IState = slsGlimmer;
    IMax = AMax;
    IMin = AMin;
    if(ICurrentValue < IMax) INeededValue = IMax;
    else INeededValue = IMin;
    uint32_t Delay = ICalculateDelay(ICurrentValue);
    chVTSet(&ITmr, MS2ST(Delay), LedTmrCallback, NULL);
}

void LedSmooth_t::IIrqHandler() {
    if (INeededValue < ICurrentValue) ICurrentValue--;
    else ICurrentValue++;
    IPin.Set(ICurrentValue);

    // if equal and glimmer needed, switch to another
    if(IState == slsGlimmer) {
        if(ICurrentValue == IMax) INeededValue = IMin;
        else if(ICurrentValue == IMin) INeededValue = IMax;
    }

    if(ICurrentValue != INeededValue) {
        uint32_t Delay = ICalculateDelay(ICurrentValue);
        chSysLockFromIsr();
        chVTSetI(&ITmr, MS2ST(Delay), LedTmrCallback, NULL);
        chSysUnlockFromIsr();
    }

    // Send event if off
    if((ICurrentValue == 0) and (INeededValue == 0) and (IState != slsGlimmer)) {
        if(IPEvtThd != nullptr) {
            chSysLock();
            chEvtSignalI(IPEvtThd, EVTMSK_LED_OFF);
            chSysUnlock();
        }
    }
}
