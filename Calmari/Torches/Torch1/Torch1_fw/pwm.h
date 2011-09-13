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
#include "system_stm32l1xx.h"

// Timer setup constants
#define PWM_MAX         2000     // Equals to Period
#define PWM_FREQ        999     // Hz
#define PWM_PRESCALER   (uint16_t)((SystemCoreClock / (PWM_FREQ * PWM_MAX)) - 1)


enum PwmState_t {psRampUp, psRampDown};

class Pwm_t {
private:
    uint16_t IPwm, IPwmDesired;
    uint32_t ITicker;
    void Set(uint16_t APWM) { TIM_SetCompare4(TIM4, APWM); }

public:
    PwmState_t State;
    void Init(void);
    void RampUp(void)    { IPwmDesired = PWM_MAX; State = psRampUp;   }
    void RampDown(void)  { IPwmDesired = 0;       State = psRampDown; }
    bool IsReached(void) { return (IPwm == IPwmDesired); }
    void IrqHandler(void);
};

extern Pwm_t Pwm;

// Declare Timer IRQ. Use externC to make it visible from asm file.
extern "C" {
void TIM4_IRQHandler(void);
}

#endif /* PWM_H_ */
