/*
 * File:   motor.h
 * Author: Kreyl Laurelindo
 *
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include <avr/io.h>

// ============================== Defines ======================================
#define MOTOR_DDR   DDRD
#define MOTOR_PORT  PORTD
#define MOTOR_P     PD4

#define MAX_COUNT_TO_FLINCH 4
// Motor timings, ms
#define MOTOR_ON_TIME       18
#define MOTOR_OFF_TIME      150
#define MOTOR_UPDATE_TIME   999

//#define MOTOR_ON()  MOTOR_PORT &= ~(1<<MOTOR_P)
//#define MOTOR_OFF() MOTOR_PORT |=  (1<<MOTOR_P)
#define MOTOR_ON()  MOTOR_DDR |=  (1<<MOTOR_P)
#define MOTOR_OFF() MOTOR_DDR &= ~(1<<MOTOR_P)

// =========================== Prototypes ======================================
void MotorInit(void);
void MotorSetCount(uint8_t FlinchCount);

void Motor_TASK(void);


#endif	/* MOTOR_H */

