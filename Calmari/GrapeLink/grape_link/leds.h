/* 
 * File:   ledc.h
 * Author: Kreyl Laurelindo
 *
 * Created on 21 Февраль 2011 г., 23:03
 */

#ifndef LEDC_H
#define	LEDC_H

#include <avr/io.h>
#include <inttypes.h>
#include <stdbool.h>

// Timings
#define LED_STEP_DELAY  8  // ms
// Reds
#define RED_DDR     DDRD
#define RED_PORT    PORTD
#define RED_COUNT   4
// Greens
#define GREEN_DDR   DDRA
#define GREEN_PORT  PORTA
#define GREEN_COUNT 6
// Common
#define PWM_MAX     250

// Types
enum LedColor_t {Red, Green};
enum LedState_t {On, Off};

struct Leds_t {
    uint16_t Timer;
    enum LedColor_t LedColor;
    uint8_t LedGreenNumber, LedRedNumber;
    uint8_t CurrentPWM, DesiredPWM;
};

extern struct Leds_t ELeds;
extern uint8_t Reds  [RED_COUNT];
extern uint8_t Greens[GREEN_COUNT];

// Prototypes
void LedsInit(void);
void LedSwitch(enum LedState_t AState);

void PWMStop(void);
void PWMStart(void);
void PWMSet(uint8_t APWM);

#endif	/* LEDC_H */

