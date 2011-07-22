/*
 * File:   motor.c
 * Author: Kreyl Laurelindo
 *
 * Created on...
 */

#include "common.h"
#include "motor.h"
#include "time_utils.h"

enum MotorState_t {M_Idle, M_On, M_Off};
struct {
    uint8_t Count;
    uint8_t PWMcounter;
    uint16_t Timer;
    enum MotorState_t State;
} EMotor;

FORCE_INLINE void MotorInit(void) {
    MOTOR_PORT &= ~(1<<MOTOR_P);
    EMotor.State = M_Idle;
    EMotor.Count = 0;
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
