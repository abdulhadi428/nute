/*
 * led.h
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f0xx.h"
#include "ch.h"
#include "hal.h"
#include "kl_lib_f0.h"

#define LED_TIM             TIM2
#define LED_CCR             CCR2
#define LED_RCC_EN()        rccEnableTIM2()
#define LED_TOP_VALUE       1998
#define LED_PWM_FREQ_HZ     999             // No more than 3 kHz

// Adjust delay depending on value
static inline uint32_t SmoothDelay(uint16_t AValue) {
    return (uint32_t)((810 / (AValue+4)) + 1);
//    uint32_t D;
//    if     (AValue < 72 ) D = 36;
//    else if(AValue < 207) D = 4;
//    else D = 2;
//    return D;
}

// Smooth on & off.
class LedSmooth_t {
private:
    Thread *PThread;
    bool IsSleeping;
    uint16_t ICurrentValue, INeededValue;
public:
    void Init();
    void Task();
    void Set(uint16_t AValue) { LED_TIM->LED_CCR = AValue; ICurrentValue = AValue; }
    void SetSmoothly(uint16_t AValue);
};

extern LedSmooth_t Led;

#endif /* LED_H_ */
