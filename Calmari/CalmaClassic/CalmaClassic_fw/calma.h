/*
 * calma.h
 *
 *  Created on: 22.07.2011
 *      Author: Kreyl
 */

#ifndef CALMA_H_
#define CALMA_H_

#include <avr/io.h>

// Pins
#define KEY_P   PB2
#define LED_P   PB1

// =================================== PWM =====================================
// Timings
#define PWMDelayLong    108  // Low brightness
#define PWMDelayMid     72  // Mid brightness
#define PWMDelayFast    18  // High brightness

// Light constants
#define PWM_MAX         250
#define PWM_START_VALUE 45  // Initial LED brightness
#define PWM_STEPOVER1   36  // Where to switch to quick PWM change mode
#define PWM_STEPOVER2   54

#define KEY_POLL_TIMEOUT    99 // ms
#define KEY_IS_DOWN()       bit_is_set (PINB, KEY_P)

// ================================== ADC ======================================
#define ADC_TIMEOUT     99
// Number of measures as power of 2: 0 means 1, 1 means 2, 3 means 8 and so on
// Needed for averaging of values
#define ADC_POWER_OF_MEASURES   3   // Power of 2
#define ADC_NUMBER_OF_MEASURES  (1<<ADC_POWER_OF_MEASURES)

//#define ILedNominal     400 // 20 mA @ 1.1V reference
#define ILedNominal     504 // 25 mA @ 1.1V reference
//#define ILedNominal     640 // 800 mA @ 2.56V reference
//#define ILedNominal     128 // 160 mA @ 2.56V reference

// ============================ Prototypes =====================================
void GeneralInit(void);

void ADC_Start(void);

// PWM
bool MayChangePWM(void);
void PWMEnable(void);
void PWMDisable(void);

// Tasks
void Key_Task(void);
void LED_Task(void);
void Sleep_Task(void);
void ADC_Task(void);

// Events
void EVENT_ADC_Done(void);
void EVENT_KeyPressed(void);


#endif /* CALMA_H_ */
