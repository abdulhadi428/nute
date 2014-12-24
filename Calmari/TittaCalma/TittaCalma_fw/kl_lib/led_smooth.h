/*
 * led_smooth.h
 *
 *  Created on: 19 dec 2014 ã.
 *      Author: Kreyl
 */

#ifndef LED_SMOOTH_H_
#define LED_SMOOTH_H_

#include "hal.h"
#ifdef STM32F0XX
#include "kl_lib_f0.h"
#elif defined STM32L
#include "kl_lib_L15x.h"
#endif
// ==== LED's ports, pins and Tmr ====
struct LedChnl_t {
    GPIO_TypeDef *PGpio;
    uint16_t Pin;
    TIM_TypeDef *PTimer;
    uint32_t TmrChnl;
    PinAF_t AF;
    void Set(const uint8_t AValue) const { *(uint32_t*)(&PTimer->CCR1 + TmrChnl - 1) = AValue; }    // CCR[N] = AValue
    void Init() const;
};

// Port, pin and timer settings. Edit this properly.
#define LED_TOP_VALUE       255
#define LED_INVERTED_PWM    FALSE
const LedChnl_t LedCh = {GPIOA, 15, TIM2, 1, AF2};

// TimeToWaitBeforeNextAdjustment = SmoothVar / (N+4) + 1, where N - current LED brightness.
class LedSmooth_t {
private:
    uint32_t ISmoothVar=360;
    uint32_t ICalcDelay(uint32_t CurrentBrightness, uint32_t SmoothVar) { return (uint32_t)((SmoothVar / (CurrentBrightness+4)) + 1); }
    VirtualTimer ITmr;
public:
    uint32_t DesiredBrt=0, CurrentBrt=0;
    void Init() { LedCh.Init(); }
    void FadeIn(const uint32_t ASmoothVar);
    void FadeOut(const uint32_t ASmoothVar);
    // Inner use
    void IAdjustBrightness();
};

extern LedSmooth_t Led;

#endif /* LED_SMOOTH_H_ */
