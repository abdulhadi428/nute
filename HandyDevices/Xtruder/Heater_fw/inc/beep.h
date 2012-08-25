/*
 * sound.h
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "kl_lib.h"

#define BEEP_ON_LEN     45
#define BEEP_OFF_LEN    99
#define BEEP_TIM        TIM3
#define BEEP_CHANNEL    TIM3->CCR3
#define BEEP_TOP_VALUE  200     // Top value of PWM = 200, full volume = 100(%)

class Beep_t {
private:
    uint32_t ICounter;
    bool IsOn;
    uint8_t IVolume;
    uint16_t ITimer;
public:
    void On(void)  { BEEP_CHANNEL = IVolume; IsOn = true; }
    void Off(void) { BEEP_CHANNEL = 0; IsOn = false;}
    void SetFreqHz(uint32_t AFreq);
    void Init(void);
    void Squeak(uint32_t ANumber, uint8_t AVolume);
    void Task(void);
};

extern Beep_t Beep;


#endif /* SOUND_H_ */
