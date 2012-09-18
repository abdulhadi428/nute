/*
 * vibro.h
 *
 *  Created on: 18.09.2012
 *      Author: kreyl
 */

#ifndef VIBRO_H_
#define VIBRO_H_

#include "kl_lib.h"

// Motor timings, ms
#define VIBRO_ON_TIME   36      // ms
#define VIBRO_OFF_TIME  150     // ms

class Vibro_t {
private:
    uint8_t ICounter;
    uint32_t ITimer;
    bool IsOn;
    void On()  { klGpioSetByMsk(GPIOA, GPIO_Pin_2);   IsOn = true;}
    void Off() { klGpioClearByMsk(GPIOA, GPIO_Pin_2); IsOn = false;}
public:
    void Init() { klGpioSetupByN(GPIOA, 2, GPIO_Mode_Out_PP); }
    void Task();
    void Flinch(uint8_t ACount) {
        ICounter = ACount;
        On();
        Delay.Reset(&ITimer);
    }
};

extern Vibro_t Vibro;

#endif /* VIBRO_H_ */
