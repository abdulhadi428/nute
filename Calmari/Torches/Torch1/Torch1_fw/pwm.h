/*
 * pwm.h
 *
 *  Created on: 12.09.2011
 *      Author: Kreyl
 */

#ifndef PWM_H_
#define PWM_H_

#include "inttypes.h"
#include "stm32l1xx_tim.h"

#define PWM_MAX 250

class Pwm_t {
private:
    uint16_t IPwm, IPwmDesired;
    uint32_t ITimer;
    void Set(uint8_t APWM) { TIM_SetCompare4(TIM4, APWM); }
    bool MayChange(void);
    void Init(void);
public:
    void RampUp(void)   { IPwmDesired = PWM_MAX; }
    void RampDown(void) { IPwmDesired = 0; }
    bool IsReached(void) { return (IPwm == IPwmDesired); }
    void Task(void);
};

extern Pwm_t Pwm;

#endif /* PWM_H_ */
