/*
 * main.h
 *
 *  Created on: 18.07.2011
 *      Author: Kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <inttypes.h>
#include <avr/wdt.h>

#include "delay_util.h"

// ================================== Light ====================================
// Timings
#define LED_STEP_DELAY  36  // ms

// Ports & pins
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define RED_P       PD6
#define GREEN_P     PD5
#define BLUE_P      PD3

#define LED_RED_DISABLE()   TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0));
#define LED_RED_ENABLE()    TCCR0A |=  ((1<<COM0A1)|(0<<COM0A0));
#define LED_GREEN_DISABLE() TCCR0A &= ~((1<<COM0B1)|(1<<COM0B0));
#define LED_GREEN_ENABLE()  TCCR0A |=  ((1<<COM0B1)|(0<<COM0B0));
#define LED_BLUE_DISABLE()  TCCR2A &= ~((1<<COM2B1)|(1<<COM2B0));
#define LED_BLUE_ENABLE()   TCCR2A |=  ((1<<COM2B1)|(0<<COM2B0));


struct Color_t {
    uint8_t Red, Green, Blue;
    //bool operator == (const Color_t AColor) { return ((this->Red == AColor.Red) && (this->Green == AColor.Green) && (this->Blue == AColor.Blue)); }
    //bool operator != (const Color_t AColor) { return ((this->Red != AColor.Red) || (this->Green != AColor.Green) || (this->Blue != AColor.Blue)); }
    const bool IsOn(void) const { return (Red || Green || Blue); }
};
#define clBlack     {0, 0, 0}

struct Light_t {
    uint16_t Timer;
    struct Color_t DesiredColor, CurrentColor;
};

// ============================== Prototypes ===================================
void GeneralInit(void);

extern Light_t ELight;

#endif /* MAIN_H_ */
