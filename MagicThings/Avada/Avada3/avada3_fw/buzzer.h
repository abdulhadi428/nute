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
#define BUZZER_GPIO         GPIOA
#define BUZZER_PIN          8

// Sound
#define BUZZ_DELAY_MS       5
#define PERIOD_MAX          3600    // Lowest frequency
#define PERIOD_MIN          315     // Highest frequency
// Sound volume
#define VOLUME_NORMAL       15
#define VOLUME_MAX          153

class Buzz_t {
private:
    VirtualTimer ITmr;
    PwmPin_t IPin;
public:
    uint16_t IPeriod;
    void Init() { IPin.Init(BUZZER_GPIO, BUZZER_PIN, BUZZER_TIM_N, BUZZER_TIM_CH, PERIOD_MAX); }
    void BuzzUp();
    void SetVolume(uint16_t AVolume) { IPin.Set(AVolume); }
    void Off() {
        chVTReset(&ITmr);
        IPin.Off();
    }
    bool IsOnTop() { return (IPeriod <= PERIOD_MIN); }
    // Inner use
    void ITmrCallback();
};

extern Buzz_t Buzzer;

#endif /* BUZZER_H_ */
