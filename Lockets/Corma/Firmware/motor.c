/* 
 * File:   motor.c
 * Author: Kreyl Laurelindo
 *
 * Created on 27 Июнь 2010 г., 11:24
 */

#include <util/delay.h>
#include "common/common.h"
#include "motor.h"
#include "time_utils.h"

enum MotorState_t {M_Idle, M_On, M_Off};
struct {
    uint8_t Count;
    uint8_t PWMcounter;
    uint16_t Timer;
    enum MotorState_t State;
} EMotor;

FORCE_INLINE void MotorInit(uint8_t FlinchCount) {
    MOTOR_PORT &= ~(1<<MOTOR_P);
    EMotor.State = M_Idle;
    EMotor.Count = 0;
    // Motor blink at power-on
    for (uint8_t i=0; i<FlinchCount; i++){
        MOTOR_ON();
        _delay_ms(MOTOR_ON_TIME);
        MOTOR_OFF();
        _delay_ms(MOTOR_OFF_TIME);
    }
}

FORCE_INLINE void MotorSetCount(uint8_t FlinchCount) {
    if (FlinchCount > MAX_COUNT_TO_FLINCH) EMotor.Count = MAX_COUNT_TO_FLINCH;
    else EMotor.Count = FlinchCount;
}

void Motor_TASK(void) {
    if (EMotor.Count == 0) return; // Nothing to do here
    // Handle flinch
    switch (EMotor.State) {
        case M_Idle:    // First came here
            TimerResetDelay(&EMotor.Timer);
            MOTOR_ON();
            EMotor.State = M_On;
            break;

        case M_On:
            if (TimerDelayElapsed(&EMotor.Timer, MOTOR_ON_TIME)) {
                MOTOR_OFF();
                EMotor.State = M_Off;
            }
            break;

        case M_Off:
            if (TimerDelayElapsed(&EMotor.Timer, MOTOR_OFF_TIME)) {
                // Blink ended, check if this one was the last
                EMotor.Count--;
                if (EMotor.Count > 0){
                    MOTOR_ON();
                    EMotor.State = M_On;
                }
                else EMotor.State = M_Idle;
            }
            break;
    } //switch
}
