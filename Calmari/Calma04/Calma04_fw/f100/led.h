/*
 * led.h
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"
#include "ch.h"
#include "hal.h"
#include "kl_lib_f100.h"

#define LED_GPIO            GPIOA
#define LED_GPIO_N          1
#define LED_TIM_N           2
#define LED_CCR_N           2
#define LED_TOP_VALUE       1998
#define LED_BOTTOM_VALUE    0
#define LED_PWM_FREQ_HZ     9999

enum SmLedState_t {slsNone, slsGlimmer};
class LedSmooth_t {
private:
    Thread *IPEvtThd;
    VirtualTimer ITmr;
    PwmPin_t IPin;
    SmLedState_t IState;
    uint16_t IMax, IMin;
    uint16_t ICurrentValue, INeededValue;
    uint32_t ICalculateDelay(uint16_t AValue) {
        return (uint32_t)((810 / (AValue+4)) + 1);
    }
public:
    void Init(uint16_t InitialValue, Thread *PEvtThd);
    void Set(uint16_t AValue) { IPin.Set(AValue); ICurrentValue = AValue; }
    void SetSmoothly(uint16_t AValue);
    bool IsEqOrAbove(uint16_t AValue) { return (ICurrentValue >= AValue); }
    bool IsEqOrBelow(uint16_t AValue) { return (ICurrentValue <= AValue); }
    void Glimmer(uint16_t AMax, uint16_t AMin);
    // Inner use
    void IIrqHandler();
};

extern LedSmooth_t Led;

#endif /* LED_H_ */
