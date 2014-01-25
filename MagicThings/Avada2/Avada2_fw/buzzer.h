/*
 * buzzer.h
 *
 *  Created on: 25 џэт. 2014 у.
 *      Author: Kreyl
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "kl_lib_f100.h"

#define BUZZER_TIM_N        1 // TIM1
#define BUZZER_TIM_CH       1
#define BUZZER_TIM_TOP      10
#define BUZZER_TIM_VALUE    1
#define BUZZER_GPIO         GPIOA
#define BUZZER_PIN          8

// Sound
#define BUZZ_DELAY  5
#define PERIOD_MAX  3600    // Lowest frequency
#define PERIOD_MIN  315     // Highest frequency
// Sound volume
#define VOLUME      7

class Buzz_t {
private:
    uint16_t IFreq, IPeriod, IVolume;
    VirtualTimer ITmr;
    PwmPin_t IPin;
    uint32_t Delay() {
        if(IPeriod > 900) return 1;
        else if(IPeriod > 600) return 4;
        else return 9;
    }
public:
    void Init() {
        IPin.Init(BUZZER_GPIO, BUZZER_PIN, BUZZER_TIM_N, BUZZER_TIM_CH, BUZZER_TIM_TOP);
    }
    void BuzzUp();
    void Off() {
        // TODO: reset tmr
        IPin.Off();
    }
    // Inner use
    void ITmrCallback();
};

extern Buzz_t Buzzer;

#endif /* BUZZER_H_ */
